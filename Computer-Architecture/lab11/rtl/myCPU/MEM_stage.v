`include "mycpu.h"

module mem_stage(
    input                          clk           ,
    input                          reset         ,
    //allowin
    input                          ws_allowin    ,
    output                         ms_allowin    ,
    //from es
    input                          es_to_ms_valid,
    input  [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus  ,
    //to ws
    output                         ms_to_ws_valid,
    output [`MS_TO_WS_BUS_WD -1:0] ms_to_ws_bus  ,
    //from data-sram
    input						   data_data_ok,
	input  [31                 :0] data_sram_rdata,

    //pipeline block    
    output [4:0] reg_dest_ms,
    output [31:0] ms_value,
    output ms_mfc0,

    input       ws_ex_in,  
    input       eret_flush, 
    output      ms_ex_out,
	//lab11
	output		ms_lw_valid
);

reg         ms_valid;
wire        ms_ready_go;

reg [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus_r;
wire        ms_res_from_mem;
wire        ms_gr_we;
wire [ 4:0] ms_dest;
wire [31:0] ms_alu_result;
wire [31:0] ms_pc;

// lab8 append
wire [4:0]  ms_cp0_rd;
wire [2:0]  ms_cp0_sel;


wire ms_inst_eret;
wire ms_inst_sys;
wire ms_inst_mfc0;
wire ms_inst_mtc0;

wire        es_to_ms_ex;
wire        es_to_ms_bd;
wire [4:0]  es_to_ms_excode;
wire [31:0] es_to_ms_badvaddr;

wire        ms_ex;
wire        ms_bd;
wire [4:0]  ms_excode;
wire [31:0] ms_badvaddr;


//lab7
wire ms_inst_lb;
wire ms_inst_lbu;
wire ms_inst_lh;
wire ms_inst_lhu;
wire ms_inst_lwl;
wire ms_inst_lwr;
wire [31:0] ms_rt_value;
wire [1:0]n;//地址末尾两位
wire ms_lw;
wire ms_store;
assign {
		ms_store,
        ms_cp0_rd,        //159:155
        es_to_ms_ex,      //154:154
        es_to_ms_bd,      //153:153
        es_to_ms_excode,  //152:148
        es_to_ms_badvaddr,//147:116 
        ms_cp0_sel,       //115:113
        ms_inst_eret,     //112:112
        ms_inst_sys,      //111:111
        ms_inst_mtc0,     //110:110
        ms_inst_mfc0,     //109:109
		ms_rt_value,      //108:77
		ms_inst_lwl,	  //76
		ms_inst_lwr,	  //75
		ms_inst_lhu,      //74
		ms_inst_lh,	      //73
		ms_inst_lbu,	  //72	
		ms_inst_lb,		  //71
        ms_res_from_mem,  //70:70
        ms_gr_we       ,  //69:69
        ms_dest        ,  //68:64
        ms_alu_result  ,  //63:32
        ms_pc             //31:0
       } = es_to_ms_bus_r;

wire [31:0] mem_result;
wire [31:0] ms_final_result;

assign n = ms_alu_result[1:0];

wire [7:0]LB_base;
wire [31:0]LBdata;
wire [31:0]LBUdata;
wire [15:0]LH_base;
wire [31:0]LHdata;
wire [31:0]LHUdata;
wire [31:0]LWLdata;
wire [31:0]LWRdata;
reg  [31:0] data_sram_rdata_r;
reg ms_ready_go_r;
reg [1:0] cstate_data_ok;
/*always @(posedge clk) begin
	if(reset) begin
		ms_ready_go_r <= 1'b0;
		data_sram_rdata_r <= 32'b0;
		cstate_data_ok <= `STATE_ONE;
	end
	else if(cstate_data_ok == `STATE_ONE) begin
		if((ms_store || ms_res_from_mem)) begin
			cstate_data_ok <= `STATE_TWO;
		end
	end
	else if(cstate_data_ok == `STATE_TWO) begin
		
	end
end	
*/
reg ms_lw_valid_r;
always @(posedge clk) begin
	if(reset) begin
		ms_ready_go_r <= 1'b0;
		data_sram_rdata_r <= 32'b0;
		ms_lw_valid_r <= 1'b0;
	end
	else if(data_data_ok) begin
		ms_ready_go_r <= 1'b1;
		ms_lw_valid_r <= 1'b1;
		data_sram_rdata_r <= data_sram_rdata;
	end
	else if(ws_allowin && ms_to_ws_valid) begin
		ms_ready_go_r <= 1'b0;
	end
	if(ms_lw_valid_r && ms_allowin && es_to_ms_valid) begin
		ms_lw_valid_r <= 1'b0;
	end
end
//ms_lw_valid 1时阻塞，0时畅通
assign ms_lw_valid = ms_res_from_mem & ~ms_lw_valid_r & ~ms_ex & ms_valid;



//LB指令
assign LB_base = (n == 2'b00)  ? (data_sram_rdata_r[7:0]):
                 ((n == 2'b01) ? (data_sram_rdata_r[15:8]):
                 ((n == 2'b10) ? (data_sram_rdata_r[23:16]):
                 (data_sram_rdata_r[31:24])));
    
assign LBdata = (LB_base[7] == 1) ? ({{24{1'b1}},LB_base}):({{24{1'b0}},LB_base});      
assign LBUdata = {{24{1'b0}},LB_base};   
//LH指令
assign LH_base = ((ms_alu_result[1] ) ? (data_sram_rdata_r[31:16]):(data_sram_rdata_r[15:0]));
assign LHdata = (LH_base[15]) ? ({{16{1'b1}},LH_base}):({{16{1'b0}},LH_base});
assign LHUdata = {{16{1'b0}},LH_base};   
//LWL指令，读出数据低位从左边�?始写
assign LWLdata = (n ==2'b11) ? (data_sram_rdata_r):
                 ((n == 2'b10) ? ({data_sram_rdata_r[23:0], ms_rt_value[7:0]}):
                 ((n == 2'b01) ? ({data_sram_rdata_r[15:0],ms_rt_value[15:0]}):
                 ({data_sram_rdata_r[7:0],ms_rt_value[23:0]})));
    
//LWR指令，读出数据的高位从寄存器右边�?始写
assign LWRdata = (n ==2'b11) ? ({ms_rt_value[31:8], data_sram_rdata_r[31:24]}):
                 ((n == 2'b10) ? ({ms_rt_value[31:16], data_sram_rdata_r[31:16]}):
                 ((n == 2'b01) ? ({ms_rt_value[31:24],data_sram_rdata_r[31:8]}):
                 (data_sram_rdata_r)));
                                            
assign reg_dest_ms = ms_valid ? ((ms_gr_we) ? (ms_dest):(5'b0)):(5'b0);
assign ms_value = ms_final_result; 

assign ms_to_ws_bus = {
                       ms_cp0_rd,        //120:116
                       ms_ex,            //115:115
                       ms_bd,            //114:114
                       ms_excode,        //113:109
                       ms_badvaddr,      //108:77
                       ms_cp0_sel,       //76:74
                       ms_inst_eret,     //73:73
                       ms_inst_sys,      //72:72
                       ms_inst_mtc0,     //71:71
                       ms_inst_mfc0,     //70:70
                       ms_gr_we       ,  //69:69
                       ms_dest        ,  //68:64
                       ms_final_result,  //63:32
                       ms_pc             //31:0
                      };

reg eret_flush_ms;
reg ws_ex_in_ms;
always @(posedge clk) begin
	if(reset) begin
		eret_flush_ms <= 1'b0;
	end
	else if(eret_flush) begin
		eret_flush_ms <= 1'b1;
	end
	else if(es_to_ms_valid && ms_allowin) begin
		eret_flush_ms <= 1'b0;
	end
end

always @(posedge clk) begin
	if(reset) begin
		ws_ex_in_ms <= 1'b0;
	end
	else if(ws_ex_in) begin
		ws_ex_in_ms <= 1'b1;
	end
	else if(es_to_ms_valid && ms_allowin) begin
		ws_ex_in_ms <= 1'b0;
	end
end
assign ms_ready_go = ((ms_store || ms_res_from_mem) && (~ms_ex)) ? ms_ready_go_r : 1'b1;
assign ms_allowin     = !ms_valid || ms_ready_go && ws_allowin;
assign ms_to_ws_valid =  ms_valid && ms_ready_go && 
						 ~(eret_flush_ms || eret_flush) && 
						 ~(ws_ex_in_ms || ws_ex_in);
always @(posedge clk) begin
    if (reset) begin
        ms_valid <= 1'b0;
		es_to_ms_bus_r <= 32'b0;
    end
    else if (ms_allowin) begin
        ms_valid <= es_to_ms_valid;
    end
	else if(eret_flush || ws_ex_in) begin
		ms_valid <= 1'b0;
	end
    if (es_to_ms_valid && ms_allowin) begin
        es_to_ms_bus_r  <= es_to_ms_bus;
    end
end

// lw inst
assign ms_lw = ms_res_from_mem & (~(ms_inst_lb | ms_inst_lbu | ms_inst_lh | ms_inst_lhu | ms_inst_lwl | ms_inst_lwr)); 
assign mem_result = ({32{ms_inst_lb}} & LBdata) 
					| ({32{ms_inst_lbu}} & LBUdata)
					| ({32{ms_inst_lh}} & LHdata) 
					| ({32{ms_inst_lhu}} & LHUdata) 
					| ({32{ms_inst_lwl}} & LWLdata)
					| ({32{ms_inst_lwr}} & LWRdata)
					| ({32{ms_lw}} & data_sram_rdata_r); 
assign ms_final_result = ms_res_from_mem ? mem_result :
                         ms_inst_mfc0 ? ms_rt_value : 
                                        ms_alu_result;

// lab8 append
assign ms_mfc0 = ms_inst_mfc0 && ms_valid;
//ms例外时，�?要防止EXE阶段的写使能�?
assign ms_ex_out = (ms_ex || ms_inst_eret) && ms_valid;

assign ms_ex = es_to_ms_ex;
assign ms_bd = es_to_ms_bd;
assign ms_excode = es_to_ms_excode;
assign ms_badvaddr = es_to_ms_badvaddr;

				
endmodule
