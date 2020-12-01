`include "mycpu.h"

module if_stage(
    input                          clk            ,
    input                          reset          ,
    //allwoin
    input                          ds_allowin     ,
    //brbus
    input  [`BR_BUS_WD       -1:0] br_bus         ,
    //to ds
    output                         fs_to_ds_valid ,
    output [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus   ,
    // inst sram interface
    output        inst_sram_en   ,
    output [ 3:0] inst_sram_wen  ,
    output [31:0] inst_sram_addr ,
    output [31:0] inst_sram_wdata,
    input  [31:0] inst_sram_rdata,
    
    //lab8
    input [31:0]  cp0_epc,
    input         eret_flush,
    //input         ds_ex_in,
    //input         es_ex_in,
    //input         ms_ex_in,
    input         ws_ex_in
);

// lab8 append
wire        fs_addr_error;
wire        fs_ex;
wire        fs_bd;
wire [4:0]  fs_excode;
wire [31:0] fs_badvaddr;


reg         fs_valid;
wire        fs_ready_go;
wire        fs_allowin;
wire        to_fs_valid;

wire [31:0] seq_pc;
wire [31:0] nextpc;

wire         br_taken;
wire [ 31:0] br_target;
wire fs_delay_slot;
assign {fs_delay_slot,br_taken,br_target} = br_bus;

wire [31:0] fs_inst;
reg  [31:0] fs_pc;
assign fs_to_ds_bus = {
                        fs_ex,          //102:102
                        fs_bd,          //101:101
                        fs_excode,      //100:96
                        fs_badvaddr,    //95:64                      
                        fs_inst ,       //63:32
                        fs_pc           //31:0
                    };

// pre-IF stage
assign to_fs_valid  = ~reset;
assign seq_pc       = fs_pc + 3'h4;
//assign nextpc = br_taken ? br_target : seq_pc;

assign nextpc = ws_ex_in ? 32'hbfc00380:
                eret_flush ? cp0_epc:
                br_taken ? br_target:
                            seq_pc; 

// IF stage
assign fs_ready_go    = 1'b1;

assign fs_allowin     = !fs_valid || fs_ready_go && ds_allowin;
assign fs_to_ds_valid =  fs_valid && fs_ready_go && !eret_flush && !ws_ex_in;
//assign fs_to_ds_valid =  fs_valid && fs_ready_go;
always @(posedge clk) begin
    if (reset) begin
        fs_valid <= 1'b0;
    end
    else if (fs_allowin) begin
        fs_valid <= to_fs_valid;      
    end

    if (reset) begin
        fs_pc <= 32'hbfbffffc;  //trick: to make nextpc be 0xbfc00000 during reset 
    end
    else if (to_fs_valid && fs_allowin) begin
        fs_pc <= nextpc;
    end
end

assign inst_sram_en    = to_fs_valid && fs_allowin;
assign inst_sram_wen   = 4'h0;
assign inst_sram_addr = nextpc;
assign inst_sram_wdata = 32'b0;
assign fs_inst         = eret_flush ? 32'b0 : inst_sram_rdata;
/*跳转指令跳转到错误地址，即跳转指令更新了PC，但是这个PC不合法。此时当前指令是处于延迟槽中的指令，是延迟槽
后的指令操作，此时标记IF阶段标记的是延迟槽指令而不是跳转目标。*/
reg fs_addr_error_r;
always@(posedge clk)begin
	if(reset)begin
		fs_addr_error_r <= 1'b0;
	end
	else 
		fs_addr_error_r <= (nextpc[1:0] == 2'b0) ? 1'b0 : 1'b1;
end
assign fs_addr_error = fs_addr_error_r;
//assign fs_addr_error = (nextpc[1:0] == 2'b0) ? 1'b0 : 1'b1;
//if exception
assign fs_ex = fs_addr_error && fs_valid;
//the instruction is after branch
//assign fs_bd = br_taken;
assign fs_bd = fs_delay_slot;
assign fs_excode = `EX_ADEL;
//pc

//assign fs_badvaddr = nextpc;
assign fs_badvaddr = fs_pc;
endmodule
