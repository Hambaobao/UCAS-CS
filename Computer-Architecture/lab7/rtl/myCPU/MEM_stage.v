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
    input  [31                 :0] data_sram_rdata,

    //pipeline block    
    output [4:0] reg_dest_ms,
    output [31:0] ms_value
);

reg         ms_valid;
wire        ms_ready_go;

reg [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus_r;
wire        ms_res_from_mem;
wire        ms_gr_we;
wire [ 4:0] ms_dest;
wire [31:0] ms_alu_result;
wire [31:0] ms_pc;
//lab7
wire ms_inst_lb;
wire ms_inst_lbu;
wire ms_inst_lh;
wire ms_inst_lhu;
wire ms_inst_lwl;
wire ms_inst_lwr;
wire [31:0] ms_rt_value;
wire [1:0]n;//地址末尾两位
assign {
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

//lab7
assign n = ms_alu_result[1:0];

wire [7:0]LB_base;
wire [31:0]LBdata;
wire [31:0]LBUdata;
wire [15:0]LH_base;
wire [31:0]LHdata;
wire [31:0]LHUdata;
wire [31:0]LWLdata;
wire [31:0]LWRdata;
//LB指令
assign LB_base = (n == 2'b00)  ? (data_sram_rdata[7:0]):
                 ((n == 2'b01) ? (data_sram_rdata[15:8]):
                 ((n == 2'b10) ? (data_sram_rdata[23:16]):
                 (data_sram_rdata[31:24])));
    
assign LBdata = (LB_base[7] == 1) ? ({{24{1'b1}},LB_base}):({{24{1'b0}},LB_base});      
assign LBUdata = {{24{1'b0}},LB_base};   
//LH指令
assign LH_base = ((ms_alu_result[1]) ? (data_sram_rdata[31:16]):(data_sram_rdata[15:0]));
assign LHdata = (LH_base[15]) ? ({{16{1'b1}},LH_base}):({{16{1'b0}},LH_base});
assign LHUdata = {{16{1'b0}},LH_base};   
//LWL指令，读出数据低位从左边开始写
assign LWLdata = (n ==2'b11) ? (data_sram_rdata):
                 ((n == 2'b10) ? ({data_sram_rdata[23:0], ms_rt_value[7:0]}):
                 ((n == 2'b01) ? ({data_sram_rdata[15:0],ms_rt_value[15:0]}):
                 ({data_sram_rdata[7:0],ms_rt_value[23:0]})));
    
//LWR指令，读出数据的高位从寄存器右边开始写
assign LWRdata = (n ==2'b11) ? ({ms_rt_value[31:8], data_sram_rdata[31:24]}):
                 ((n == 2'b10) ? ({ms_rt_value[31:16], data_sram_rdata[31:16]}):
                 ((n == 2'b01) ? ({ms_rt_value[31:24],data_sram_rdata[31:8]}):
                 (data_sram_rdata)));                
assign reg_dest_ms = ms_valid ? ((ms_gr_we) ? (ms_dest):(5'b0)):(5'b0);
assign ms_value = ms_final_result; 

assign ms_to_ws_bus = {

                       ms_gr_we       ,  //69:69
                       ms_dest        ,  //68:64
                       ms_final_result,  //63:32
                       ms_pc             //31:0
                      };

assign ms_ready_go    = 1'b1;
assign ms_allowin     = !ms_valid || ms_ready_go && ws_allowin;
assign ms_to_ws_valid = ms_valid && ms_ready_go;
always @(posedge clk) begin
    if (reset) begin
        ms_valid <= 1'b0;
    end
    else if (ms_allowin) begin
        ms_valid <= es_to_ms_valid;
    end

    if (es_to_ms_valid && ms_allowin) begin
        es_to_ms_bus_r  <= es_to_ms_bus;
    end
end

//assign mem_result = data_sram_rdata;
assign mem_result = (ms_inst_lb) ? LBdata : 
					((ms_inst_lbu) ? LBUdata : 
					((ms_inst_lh) ? LHdata : 
					((ms_inst_lhu) ? LHUdata : 
					((ms_inst_lwl) ? LWLdata : 
					((ms_inst_lwr) ? LWRdata : data_sram_rdata)))));
assign ms_final_result = ms_res_from_mem ? mem_result
                                         : ms_alu_result;

endmodule