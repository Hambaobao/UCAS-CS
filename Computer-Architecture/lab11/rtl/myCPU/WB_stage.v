`include "mycpu.h"

module wb_stage(
    input                           clk           ,
    input                           reset         ,
    //allowin
    output                          ws_allowin    ,
    //from ms
    input                           ms_to_ws_valid,
    input  [`MS_TO_WS_BUS_WD -1:0]  ms_to_ws_bus  ,
    //to rf: for write back
    output [`WS_TO_RF_BUS_WD -1:0]  ws_to_rf_bus  ,
    //trace debug interface
    output [31:0] debug_wb_pc     ,
    output [ 3:0] debug_wb_rf_wen ,
    output [ 4:0] debug_wb_rf_wnum,
    output [31:0] debug_wb_rf_wdata,

    //pipeline block     
    output [4:0] reg_dest_ws,
    output [31:0] ws_value,
    output  ws_mfc0,
    
    //cp0
    output        eret_flush,
    output [31:0] cp0_epc,
    output [31:0] cp0_status,
    output [31:0] cp0_cause,
    output        count_eq_compare,
    output        ws_ex_out
);

// lab8 append
wire            ms_to_ws_ex;
wire            ms_to_ws_bd;
wire [4:0]      ms_to_ws_excode;
wire [31:0]     ms_to_ws_badvaddr;

wire [4:0]      ws_cp0_rd;
wire [2:0]      ws_cp0_sel;
//wire            ws_ex;
wire            ws_bd;
wire [4:0]      ws_excode;
//ws_pc
wire [31:0]     ws_badvaddr;
wire [5:0]      ext_int_in;

wire [31:0]     ws_cp0_epc;
wire [31:0]     ws_cp0_status;
wire [31:0]     ws_cp0_cause;
wire            ws_count_eq_compare;

wire ws_inst_eret;
wire ws_inst_sys;
wire ws_inst_mfc0;
wire ws_inst_mtc0;

wire [7:0]  cp0_addr;
wire [31:0] cp0_rdata;
wire        cp0_we;
wire [31:0] cp0_wdata;


reg         ws_valid;
wire        ws_ready_go;

reg [`MS_TO_WS_BUS_WD -1:0] ms_to_ws_bus_r;
wire        ws_gr_we;
wire [4:0]  ws_dest;
wire [31:0] ws_final_result;
wire [31:0] ws_pc;

assign {
        ws_cp0_rd,        //120:116
        ms_to_ws_ex,      //115:115
        ms_to_ws_bd,      //114:114
        ms_to_ws_excode,  //113:109
        ms_to_ws_badvaddr,//108:77
        ws_cp0_sel,       //76:74
        ws_inst_eret,     //73:73
        ws_inst_sys,      //72:72
        ws_inst_mtc0,     //71:71
        ws_inst_mfc0,     //70:70
        ws_gr_we       ,  //69:69
        ws_dest        ,  //68:64
        ws_final_result,  //63:32
        ws_pc             //31:0
       } = ms_to_ws_bus_r;
wire test;
assign test = ms_to_ws_bus_r[115:115];

wire        rf_we;
wire [4 :0] rf_waddr;
wire [31:0] rf_wdata;
       
assign reg_dest_ws = ws_valid ? ((ws_gr_we) ? (ws_dest):(5'b0)):(5'b0); 
assign ws_value = rf_wdata;

assign ws_to_rf_bus = {
                       rf_we   ,  //37:37
                       rf_waddr,  //36:32
                       rf_wdata   //31:0
                      };

assign ws_ready_go = 1'b1;
assign ws_allowin  = !ws_valid || ws_ready_go;
always @(posedge clk) begin
    if (reset) begin
        ws_valid <= 1'b0;
    end
    else if (ws_allowin) begin
        ws_valid <= ms_to_ws_valid;
    end

    if (ms_to_ws_valid && ws_allowin) begin
        ms_to_ws_bus_r <= ms_to_ws_bus;
    end
end

assign eret_flush = ws_inst_eret & ws_valid;
assign ws_mfc0 = ws_inst_mfc0 & ws_valid;
assign ws_ex_out = ms_to_ws_ex && ws_valid;
assign ext_int_in = 6'b0;

//assign ms_to_ws_ex = ms_to_ws_ex;
assign ws_bd = ms_to_ws_bd;
assign ws_excode = ms_to_ws_excode;
assign ws_badvaddr = ms_to_ws_badvaddr;

assign cp0_we   = ws_inst_mtc0;
assign cp0_addr = {ws_cp0_rd,ws_cp0_sel};
assign cp0_wdata = ws_final_result;

assign ws_cp0_epc = cp0_epc;
assign cp0_status = ws_cp0_status;
assign cp0_cause = ws_cp0_cause;
//assign ws_to_ds_count_eq_compare = ws_count_eq_compare;
assign count_eq_compare = 1'b0;



//assign rf_we    = ws_gr_we && ws_valid;
assign rf_we = ~(ms_to_ws_ex) & ws_gr_we & ws_valid;
//assign rf_we = (ms_to_ws_bus[115 : 115]) ? 1'b0 : (ws_gr_we && ws_valid);
assign rf_waddr = ws_dest;
//assign rf_wdata = ws_final_result;
assign rf_wdata = ws_mfc0 ? cp0_rdata : ws_final_result;
// debug info generate
assign debug_wb_pc       = ws_pc;
assign debug_wb_rf_wen   = {4{rf_we}};
assign debug_wb_rf_wnum  = ws_dest;
assign debug_wb_rf_wdata = rf_wdata;


cp0 u_cp0(
    .clk                (clk),
    .rst                (reset),
    
    .wb_ex              (ms_to_ws_ex),
    .wb_bd              (ws_bd),
    .wb_excode          (ws_excode),
    .wb_pc              (ws_pc),
    .wb_badvaddr        (ws_badvaddr),
    .eret_flush         (eret_flush),
    .ext_int_in         (ext_int_in),

    .cp0_addr           (cp0_addr),
    .cp0_rdata          (cp0_rdata),
    .mtc0_we            (cp0_we),
    .cp0_wdata          (cp0_wdata),
  
    .cp0_epc_out        (cp0_epc),  
    .cp0_status_out     (ws_cp0_status),
    .cp0_cause_out      (ws_cp0_cause),
    .count_eq_compare_out   (ws_count_eq_compare)
);

endmodule