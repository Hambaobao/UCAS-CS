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
	output		  inst_req,
	output 		  inst_wr,
	output [1:0]  inst_size,
	input		  inst_addr_ok,
	input 		  inst_data_ok,
    output [31:0] inst_sram_addr ,
    output [31:0] inst_sram_wdata,
    input  [31:0] inst_sram_rdata,
    
    //lab8
    input [31:0]  cp0_epc,
    input         eret_flush,
    input         ws_ex_in
);
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

//lab11
reg fs_ready_go_r;
reg [1:0] cstate_fs_readygo;
reg [31:0] inst_sram_rdata_r;

reg inst_req_r;
reg [1:0] cstate_inst_req;
reg to_fs_valid_r;
reg ws_ex_in_fs;
reg eret_flush_fs;

reg buf_valid;
reg [31:0] buf_npc_r;
wire [31:0] buf_npc;

reg pre_IF_ready_go;

reg fs_delay_slot_r;
reg [1:0]ex_inst_get;
reg fs_pc_prepared;

reg [1:0]mark_for_if;
reg [4:0]mark_to_fs_valid;
assign fs_to_ds_bus = {
                        fs_ex,          //102:102
                        fs_bd,          //101:101
                        fs_excode,      //100:96
                        fs_badvaddr,    //95:64                      
                        fs_inst ,       //63:32
                        fs_pc           //31:0
                    };

// pre-IF stage
assign to_fs_valid  = ~reset && to_fs_valid_r;
assign seq_pc       = fs_pc + 3'h4;

assign nextpc = ws_ex_in ? 32'hbfc00380:
                eret_flush ? cp0_epc:
                br_taken ? br_target:
                            seq_pc; 

always@(posedge clk) begin
	if(reset) begin
		to_fs_valid_r <= 1'b1;
	end
	else if(ws_ex_in || eret_flush) begin
		to_fs_valid_r <= 1'b0;
	end
	else if(mark_to_fs_valid == 5'b00000) begin
		to_fs_valid_r <= 1'b1;
	end
end

always @(posedge clk) begin
	if(reset) begin
		buf_valid <= 1'b0;
	end
	else if(to_fs_valid && fs_allowin && ~ws_ex_in && ~eret_flush) begin
		buf_valid <= 1'b0;
	end
	else if(!buf_valid) 
	begin
		buf_valid <= 1'b1;
	end
	
	if(reset) begin
		buf_npc_r <= 32'b0;
	end
	else if(ws_ex_in) begin
		buf_npc_r <= 32'hbfc00380;
	end
	else if(eret_flush) begin
		buf_npc_r <= cp0_epc;
	end
	else if(!buf_valid) begin
		buf_npc_r <= nextpc;
	end
	
end
assign buf_npc = buf_valid ? buf_npc_r : nextpc;

always@(posedge clk) begin
	if(reset) begin
		cstate_inst_req <= `STATE_ONE;
		pre_IF_ready_go <= 1'b0;
		inst_req_r <= 1'b0;
	end
	else if(cstate_inst_req == `STATE_ONE) begin
		if(to_fs_valid && fs_allowin) begin
			inst_req_r <=(buf_npc[1:0] == 2'b00) ? 1'b1 : 1'b0;
			cstate_inst_req <= (buf_npc[1:0] == 2'b00) ? `STATE_TWO : `STATE_ONE ; 
		end
	end
	else if(cstate_inst_req == `STATE_TWO) begin
		if(inst_addr_ok &&  inst_req) begin
			inst_req_r <= 1'b0;
			pre_IF_ready_go <= 1'b1;
		end
		if(inst_data_ok) begin
			cstate_inst_req <= `STATE_ONE;
			pre_IF_ready_go <= 1'b0;
		end
	end
end
assign inst_req = inst_req_r;
assign inst_sram_addr = fs_pc;
//判断例外到来的情况

always @(posedge clk) begin
	if(reset) begin
		mark_to_fs_valid <= 5'b00000;
	end
	else if(ws_ex_in || eret_flush) begin
		if(fs_addr_error && ~pre_IF_ready_go) begin
			mark_to_fs_valid <= 5'b10000;
		end
		else if(inst_req  && ~pre_IF_ready_go) begin
			mark_to_fs_valid <= 5'b01000; //发出请求但是未收到addr_ok
		end
		else if(pre_IF_ready_go && mark_for_if == 2'b00) begin
			mark_to_fs_valid <= 5'b00001;//刚发出addr_ok
		end
		else if(pre_IF_ready_go && mark_for_if == 2'b01 && inst_data_ok) begin
			mark_to_fs_valid <= 5'b00011;//data_ok和例外一起来
		end
		else if(pre_IF_ready_go && mark_for_if == 2'b01) begin
			mark_to_fs_valid <= 5'b00010;//未等到data_ok
		end
		//刚好赶上握手
		else if(mark_for_if == 2'b10 && (fs_to_ds_valid && ds_allowin)) begin
			mark_to_fs_valid <= 5'b00101;
		end
		else if(mark_for_if == 2'b10) begin
			mark_to_fs_valid <= 5'b00100;//data_ok已经来了,还没有传给id
		end
		else if(~pre_IF_ready_go && mark_for_if == 2'b00) begin
			mark_to_fs_valid <= 5'b00000;//当前指令已经传到ds
		end
	end
	else if(mark_to_fs_valid == 5'b00001 || mark_to_fs_valid == 5'b00010 || mark_to_fs_valid == 5'b01000 || mark_to_fs_valid == 5'b00101) begin
		if(inst_data_ok ) begin//第一个data_ok,无效
			mark_to_fs_valid <= 5'b00100;
		end
	end
	else if(mark_to_fs_valid == 5'b10000) begin
		if(mark_for_fs_ex) begin
			mark_to_fs_valid <= 5'b00100;
		end
	end
	else if(mark_to_fs_valid == 5'b00100) begin
		if(mark_for_if == 2'b00) begin //例外时IF的PC的取指结束，且无效
			mark_to_fs_valid <= 5'b00000;
		end
	end
	else if(mark_to_fs_valid == 5'b00011) begin
		mark_to_fs_valid <= 5'b00000;
	end
	/*else if(mark_to_fs_valid == 5'b01000) begin
		if(inst_data_ok) begin
			mark_to_fs_valid <= 5'b00000
		end
	end*/
end


// IF stage
always @(posedge clk) begin
    if (reset) begin
        fs_valid <= 1'b0;
    end
    else if (fs_allowin) begin
        fs_valid <= to_fs_valid;      
    end

    if (reset) begin
        fs_pc <= 32'hbfbffffc;   
    end
	/*else if(eret_flush) begin
		fs_pc <= cp0_epc;
	end
	else if(ws_ex_in) begin
		fs_pc <= 32'hbfc00380;
	end*/
    else if (to_fs_valid && fs_allowin) begin
        fs_pc <= buf_npc;
    end
end

always @(posedge clk) begin
	if(reset) begin
		ws_ex_in_fs <= 1'b0;
	end
	else if(ws_ex_in) begin
		ws_ex_in_fs <= 1'b1;
	end
	else if(mark_to_fs_valid == 5'b00000) begin
		ws_ex_in_fs <= 1'b0;
	end
end


always @(posedge clk) begin
	if(reset) begin
		eret_flush_fs <= 1'b0;
	end
	else if(eret_flush) begin
		eret_flush_fs <= 1'b1;
	end
	else if(mark_to_fs_valid == 5'b00000) begin
		eret_flush_fs <= 1'b0;
	end
end
reg mark_for_fs_ex;
reg mark_fs_ex_repeat;
always@(posedge clk) begin
	if(reset) begin
		cstate_fs_readygo <= `STATE_ONE;
		fs_ready_go_r <= 1'b0;
		inst_sram_rdata_r <= 32'b0;
		mark_for_if <= 3'b0;
		mark_for_fs_ex <= 1'b0;
		mark_fs_ex_repeat <= 1'b0;
	end
	else if (cstate_fs_readygo == `STATE_ONE) begin
		if(to_fs_valid && fs_allowin) begin//fs_pc更新后无需阻塞
			mark_fs_ex_repeat <= 1'b0;
		end
		
		if(pre_IF_ready_go) begin
			cstate_fs_readygo <= `STATE_TWO;
			mark_for_if <= 2'b01;
			mark_fs_ex_repeat <= 1'b0;
		end
		//地址错误
		else if(fs_addr_error && ~mark_fs_ex_repeat) begin
			cstate_fs_readygo <= `STATE_TWO;
			mark_for_if <= 2'b01;
			mark_for_fs_ex <= 1'b1;
		end
	end
	else if (cstate_fs_readygo == `STATE_TWO) begin
		if(mark_for_fs_ex) begin
			fs_ready_go_r <= 1'b1;
			inst_sram_rdata_r <= 32'b0;
			mark_for_fs_ex <= 1'b0;
		end
		else if(inst_data_ok) begin
			fs_ready_go_r <= 1'b1;
			inst_sram_rdata_r <= inst_sram_rdata;
			mark_for_if <= 2'b10;
		end
		else if((fs_to_ds_valid && ds_allowin && fs_ready_go) || mark_to_fs_valid == 5'b00100 || mark_to_fs_valid == 5'b00011) begin
			fs_ready_go_r <= 1'b0;
			cstate_fs_readygo <= `STATE_ONE;
			mark_for_if <= 2'b00;
			if(mark_to_fs_valid == 5'b00100) begin
				mark_fs_ex_repeat <= 1'b1;//避免例外多次进行
			end
		end
	end
end

assign fs_ready_go = fs_ready_go_r;
assign fs_allowin     = !fs_valid || fs_ready_go && ds_allowin;
assign fs_to_ds_valid =  fs_valid && fs_ready_go && !eret_flush_fs && !ws_ex_in_fs;

assign inst_sram_wdata = 32'b0;
assign fs_inst         = (eret_flush_fs || fs_pc[1:0] != 2'b00) ? 32'b0 : inst_sram_rdata_r;
/*跳转指令跳转到错误地坿，即跳转指令更新了PC，但是这个PC不合法㿂此时当前指令是处于延迟槽中的指令，是延迟槽
后的指令操作，此时标记IF阶段标记的是延迟槽指令迌不是跳转目标*/

always @(posedge clk) begin
	if(reset) begin
		fs_delay_slot_r <= 1'b0;
	end
	else if(fs_delay_slot) begin
		fs_delay_slot_r <= 1'b1;
	end
	else if(fs_to_ds_valid && ds_allowin && fs_delay_slot_r) begin
		fs_delay_slot_r <= 1'b0;
	end
end

assign fs_addr_error = (fs_pc[1:0] == 2'b0) ? 1'b0 : 1'b1;
//if exception
assign fs_ex = fs_addr_error && fs_valid;
//the instruction is after branch
assign fs_bd = fs_delay_slot || fs_delay_slot_r;
assign fs_excode = `EX_ADEL;
//pc
assign fs_badvaddr = fs_pc;

//lab 11
assign inst_sram_wdata = 32'b0;
assign inst_wr = 1'b0;
assign inst_size = 2'b10;

endmodule
