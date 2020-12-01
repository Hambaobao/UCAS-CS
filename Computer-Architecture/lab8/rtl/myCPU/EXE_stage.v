`include "mycpu.h"

module exe_stage(
    input                          clk           ,
    input                          reset         ,
    //allowin
    input                          ms_allowin    ,
    output                         es_allowin    ,
    //from ds
    input                          ds_to_es_valid,
    input  [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus  ,
    //to ms
    output                         es_to_ms_valid,
    output [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus  ,
    // data sram interface
    output        data_sram_en   ,
    output [ 3:0] data_sram_wen  ,
    output [31:0] data_sram_addr ,
    output [31:0] data_sram_wdata,

    //pipeline block   
    output [4:0]    reg_dest_es,
    output [31:0]   es_value,
    output          es_lw_valid,
   
   // lab8 append
    input   eret_flush,       
    input   ms_ex_in,
    input   ws_ex_in,
    output  es_mfc0
);


reg         es_valid      ;
wire        es_ready_go   ;

reg  [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus_r;
wire [11:0] es_alu_op     ;
wire        es_load_op    ;
wire        es_src1_is_sa ;  
wire        es_src1_is_pc ;
wire        es_src2_is_imm;
wire        es_src2_is_8  ;
wire        es_gr_we      ;
wire        es_mem_we     ;
wire [ 4:0] es_dest       ;
wire [15:0] es_imm        ;
wire [31:0] es_rs_value   ;
wire [31:0] es_rt_value   ;
wire [31:0] es_pc         ;

// lab8 append
wire [4:0]es_cp0_rd;
wire [2:0]es_cp0_sel;
wire overflow;
wire mem_addr_ex;
wire forbid_store;

wire es_inst_eret;
wire es_inst_sys;
wire es_inst_mfc0;
wire es_inst_mtc0;

wire        ds_to_es_ex;
wire        ds_to_es_bd;
wire [4:0]  ds_to_es_excode;
wire [31:0] ds_to_es_badvaddr;

wire        es_ex;
wire        es_bd;
wire [4:0]  es_excode;
wire [31:0] es_badvaddr;

// lab6 append
wire es_src2_is_0imm; 

wire es_inst_mult;
wire es_inst_multu;
wire es_inst_div;
wire es_inst_divu;
wire es_inst_mfhi;
wire es_inst_mflo;
wire es_inst_mthi;
wire es_inst_mtlo;

//lab7
wire es_inst_lb;
wire es_inst_lbu;
wire es_inst_lh;
wire es_inst_lhu;
wire es_inst_lwl;
wire es_inst_lwr;
wire es_inst_lw;
wire es_inst_sb;
wire es_inst_sh;
wire es_inst_swl;
wire es_inst_swr;
wire es_inst_sw;
wire [3:0]swl_wen;
wire [3:0]swr_wen;
wire [1:0] n;//地址末两�?

wire [31:0]st_data;
wire [31:0]SWL_data;
wire [31:0]SWR_data;
wire [3:0] sb_wen;
wire [3:0] sh_wen;


assign {
        es_cp0_rd,        //205:201
        ds_to_es_ex,      //200:200
        ds_to_es_bd,      //199:199
        ds_to_es_excode,  //198:194
        ds_to_es_badvaddr,//193:162 
        es_cp0_sel,       //161:159
        es_inst_eret,     //158:158
        es_inst_sys,      //157:157
        es_inst_mtc0,     //156:156
        es_inst_mfc0,     //155:155
		es_inst_swl,	  //154:154
		es_inst_swr,	  //153:153
		es_inst_sb,       //152:152
	    es_inst_sh,       //151:151
		es_inst_lwl,	  //150
		es_inst_lwr,	  //149
		es_inst_lhu,      //148
		es_inst_lh,	      //147
		es_inst_lbu,	  //146	
		es_inst_lb,		  //145
        es_inst_mult,     //144:144
        es_inst_multu,    //143:143
        es_inst_div,      //142:142
        es_inst_divu,     //141:141
        es_inst_mfhi,     //140:140
        es_inst_mflo,     //139:139
        es_inst_mthi,     //138:138
        es_inst_mtlo,     //137:137
        es_src2_is_0imm,  //136:136
        es_alu_op      ,  //135:124
        es_load_op     ,  //123:123
        es_src1_is_sa  ,  //122:122
        es_src1_is_pc  ,  //121:121
        es_src2_is_imm ,  //120:120
        es_src2_is_8   ,  //119:119
        es_gr_we       ,  //118:118
        es_mem_we      ,  //117:117
        es_dest        ,  //116:112
        es_imm         ,  //111:96
        es_rs_value    ,  //95 :64
        es_rt_value    ,  //63 :32
        es_pc             //31 :0
       } = ds_to_es_bus_r;

wire [31:0] es_alu_src1   ;
wire [31:0] es_alu_src2   ;
wire [31:0] es_alu_result ;
wire        es_res_from_mem;

assign es_res_from_mem = es_load_op;
assign es_to_ms_bus = {
                       es_cp0_rd,        //159:155
                       es_ex,            //154:154
                       es_bd,            //153:153
                       es_excode,        //152:148
                       es_badvaddr,      //147:116 
                       es_cp0_sel,       //115:113
                       es_inst_eret,     //112:112
                       es_inst_sys,      //111:111
                       es_inst_mtc0,     //110:110
                       es_inst_mfc0,     //109:109
					   es_rt_value,      //108:77
					   es_inst_lwl,		 //76
					   es_inst_lwr,		 //75
					   es_inst_lhu,      //74
					   es_inst_lh,	     //73
					   es_inst_lbu,	     //72	
					   es_inst_lb,		 //71
                       es_load_op,       //70:70
                       es_gr_we       ,  //69:69
                       es_dest        ,  //68:64
                       es_value  ,       //63:32
                       es_pc             //31:0
                      };
//lab7 append
assign n = data_sram_addr[1:0];

assign st_data = es_inst_sb ? ({4{es_rt_value[7:0]}}) :
				(es_inst_sh ? ({2{es_rt_value[15:0]}}) : 
				(es_inst_swl ? SWL_data : 
				(es_inst_swr ? SWR_data : es_rt_value)));  
				  
assign sb_wen = (n == 2'b00)? (4'b0001) : 
				(n == 2'b01 ? (4'b0010) : 
				(n == 2'b10 ? (4'b0100) : (4'b1000))); 
				
assign sh_wen = (n == 2'b00) ? 4'b0011 : 4'b1100;

assign swl_wen = (n == 2'b00)? (4'b0001) : 
				 (n == 2'b01 ? (4'b0011) : 
				 (n == 2'b10 ? (4'b0111) : (4'b1111))); 
				 
assign swr_wen = (n == 2'b00)? (4'b1111) : 
				 (n == 2'b01 ? (4'b1110) : 
				 (n == 2'b10 ? (4'b1100) : (4'b1000))); 
				 		
assign SWL_data = (n == 2'b00)? ({24'b0, es_rt_value[31:24]}) : 
				  (n == 2'b01 ? ({16'b0, es_rt_value[31:16]}) : 
				  (n == 2'b10 ? ({8'b0, es_rt_value[31:8]}) : (es_rt_value)));
				  
assign SWR_data = (n == 2'b00)? (es_rt_value) : 
				  (n == 2'b01 ? ({es_rt_value[23:0], 8'b0}) : 
				  (n == 2'b10 ? ({es_rt_value[15:0], 16'b0}) : ({es_rt_value[7:0], 24'b0}))); 				  

				
assign reg_dest_es = es_valid ? ((es_gr_we) ? (es_dest):(5'b0)):(5'b0);

assign es_lw_valid = es_valid && es_load_op && !eret_flush && !ws_ex_in;                               

//assign es_ready_go    = 1'b1;

assign es_allowin     = !es_valid || es_ready_go && ms_allowin;
assign es_to_ms_valid =  es_valid && es_ready_go && !eret_flush && !ws_ex_in;
//assign es_to_ms_valid =  es_valid && es_ready_go;
always @(posedge clk) begin
    if (reset) begin
        es_valid <= 1'b0;
    end
    else if (es_allowin) begin
        es_valid <= ds_to_es_valid;
    end

    if (ds_to_es_valid && es_allowin) begin
        ds_to_es_bus_r <= ds_to_es_bus;
    end
end

assign es_alu_src1 = es_src1_is_sa  ? {27'b0, es_imm[10:6]} : 
                     es_src1_is_pc  ? es_pc[31:0] :
                                      es_rs_value;
                                   
assign es_alu_src2 = es_src2_is_imm ? {{16{es_imm[15]}},es_imm[15:0]} :
                     es_src2_is_0imm ? {16'b0,es_imm[15:0]} :
                     es_src2_is_8   ? 32'd8 :
                                      es_rt_value;

alu u_alu(
    .alu_op     (es_alu_op    ),
    .alu_src1   (es_alu_src1  ),
    .alu_src2   (es_alu_src2  ),
    .alu_result (es_alu_result),
    .overflow   (overflow)
    );

assign data_sram_en    = 1'b1;

assign data_sram_wen   = es_mem_we && !(forbid_store) && es_valid ? ((es_inst_sb) ? sb_wen : 
											   ((es_inst_sh) ? sh_wen : 
											   ((es_inst_swl) ? swl_wen : 
											   ((es_inst_swr) ? swr_wen : 4'hf)))) : 4'h0;
assign data_sram_addr  = es_alu_result;
//assign data_sram_wdata = es_rt_value;
assign data_sram_wdata = st_data;
// lab6 append mult & div
wire [31:0] mudi_src1;
wire [31:0] mudi_src2;
wire [63:0] signed_prod;
wire [63:0] unsigned_prod;
wire [63:0] div_result;
wire [63:0] divu_result;

reg div_valid;

wire div_ready;
wire div_divisor_ready;
wire div_dividend_ready;
wire div_done;

reg divu_valid;

wire divu_ready;
wire divu_divisor_ready;
wire divu_dividend_ready;
wire divu_done;

reg [31:0] hi;
reg [31:0] lo;

// es_ready_go

assign es_ready_go = es_inst_div & !eret_flush & !ws_ex_in ? div_done :
                     es_inst_divu & !eret_flush & !ws_ex_in ? divu_done :
                                   1'b1;
                                                           
assign es_value = es_inst_mtc0 ? es_rt_value :
                  es_inst_mfhi ? hi :
                  es_inst_mflo ? lo :
                                 es_alu_result;

assign mudi_src1 = es_rs_value;
assign mudi_src2 = es_rt_value;
assign unsigned_prod = mudi_src1 * mudi_src2;
assign signed_prod = $signed(mudi_src1) * $signed(mudi_src2);

assign div_ready = div_divisor_ready & div_dividend_ready;
//assign div_valid = es_inst_div & div_ready & es_valid;

assign divu_ready = divu_divisor_ready & divu_dividend_ready;
//assign divu_valid = es_inst_divu & divu_ready & es_valid;

always @(posedge clk)
begin
    if(reset) begin
        div_valid <= 1'b0;
    end
    else if(div_valid & div_ready) begin
        div_valid <= 1'b0;
    end
    else if(ds_to_es_valid && es_allowin) begin
        div_valid <= ds_to_es_bus[142:142];
    end
end

always @(posedge clk)
begin
    if(reset) begin
        divu_valid <= 1'b0;
    end
    else if(divu_valid & divu_ready) begin
        divu_valid <= 1'b0;
    end
    else if(ds_to_es_valid && es_allowin) begin
        divu_valid <= ds_to_es_bus[141:141];
    end
end

always @(posedge clk)
begin
    if(reset) begin
        hi <= 32'b0;
        lo <= 32'b0;
    end
    else if(es_inst_mult & !forbid_store) begin
        hi <= signed_prod[63:32];
        lo <= signed_prod[31:0];
    end
    else if(es_inst_multu & !forbid_store) begin
        hi <= unsigned_prod[63:32];
        lo <= unsigned_prod[31:0];
    end
    else if(es_inst_div & div_done & !forbid_store) begin
        lo <= div_result[63:32];
        hi <= div_result[31:0];
    end
    else if(es_inst_divu & divu_done & !forbid_store) begin
        lo <= divu_result[63:32];
        hi <= divu_result[31:0];
    end
    else if(es_inst_mthi & !forbid_store) begin
        hi <= es_rs_value;
    end
    else if(es_inst_mtlo & !forbid_store) begin
        lo <= es_rs_value;
    end
end

my_div u_my_div(
      .aclk (clk),
      .s_axis_divisor_tvalid (div_valid),
      .s_axis_divisor_tready  (div_divisor_ready),
      .s_axis_divisor_tdata  (mudi_src2),
      .s_axis_dividend_tvalid (div_valid),
      .s_axis_dividend_tready (div_dividend_ready),
      .s_axis_dividend_tdata (mudi_src1),
      .m_axis_dout_tvalid (div_done),
      .m_axis_dout_tdata (div_result)    
    );
    
my_divu u_my_divu(
      .aclk (clk),
      .s_axis_divisor_tvalid (divu_valid),
      .s_axis_divisor_tready  (divu_divisor_ready),
      .s_axis_divisor_tdata  (mudi_src2),
      .s_axis_dividend_tvalid (divu_valid),
      .s_axis_dividend_tready (divu_dividend_ready),
      .s_axis_dividend_tdata (mudi_src1),
      .m_axis_dout_tvalid (divu_done),
      .m_axis_dout_tdata (divu_result)    
    );
    
// lab8 append
wire        l_ex;
wire        s_ex;
wire [4:0]  mem_excode;
assign es_mfc0 = es_inst_mfc0 && es_valid;
assign forbid_store = ms_ex_in | ws_ex_in;

assign l_ex = ((es_inst_lw) && n != 2'b00) | ((es_inst_lh | es_inst_lhu) && n[0] != 1'b0);
assign s_ex = ((es_inst_sw) && n != 2'b00) | ((es_inst_sh) && n[0] != 1'b0);

assign mem_excode = l_ex ? `EX_ADEL :
                    s_ex ? `EX_ADES : `NO_EX;

assign es_inst_lw = es_load_op & !(es_inst_lwl | es_inst_lwr | es_inst_lh | es_inst_lhu | es_inst_lb | es_inst_lbu);
assign es_inst_sw = es_mem_we & !(es_inst_swl | es_inst_swr | es_inst_sh | es_inst_sb);
assign mem_addr_ex = ((es_inst_lw | es_inst_sw) && n != 2'b00) | ((es_inst_lh | es_inst_lhu | es_inst_sh) && n[0] != 1'b0);

//OverFlow
//assign es_ex = overflow| mem_addr_ex | ds_to_es_ex;
assign es_ex = ds_to_es_ex;
assign es_bd = ds_to_es_bd;
assign es_badvaddr = mem_addr_ex ? data_sram_addr : ds_to_es_badvaddr;
assign es_excode = overflow ? (!ds_to_es_ex) ? `EX_OV : ds_to_es_excode:
                               mem_addr_ex ?mem_excode : ds_to_es_excode;
    
endmodule
