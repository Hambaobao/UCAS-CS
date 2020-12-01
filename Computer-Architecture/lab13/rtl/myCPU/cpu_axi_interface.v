`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/11/13
// Design Name: 
// Module Name: cpu_axi_interface
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////

`define ID_INST 4'b0000
`define ID_DATA 4'b0001

`define REQ_INST 2'b01
`define REQ_DATA 2'b10
`define REQ_NO   2'b00

`define STATE_ONE 2'b01
`define STATE_TWO 2'b10


module cpu_axi_interface(
	input				clk,
	input				resetn,
    //inst sram-like 
    input				inst_req,
	input				inst_wr,
	input  [1:0]		inst_size,
	input  [31:0]		inst_addr,
	input  [31:0]		inst_wdata,
	output [31:0]		inst_rdata,
	output	reg			inst_addr_ok,
	output				inst_data_ok,
    //data sram-like 
	input				data_req,
	input				data_wr,
	input  [1:0]		data_size,
	input  [31:0]		data_addr,
	input  [31:0]		data_wdata,
	output [31:0]		data_rdata,
	output reg			data_addr_ok,
	output				data_data_ok,
    //axi
    //ar
	output [3:0]		arid,
	output [31:0]		araddr,
	output [7:0]		arlen,
	output [2:0]		arsize,
	output [1:0]		arburst,
	output [1:0]		arlock,
	output [3:0]		arcache,
	output [2:0]		arprot,
	output				arvalid,
	input				arready,
    //r              
	input [3:0]			rid,
	input [31:0]		rdata,
	input [1:0]			rresp,
	input				rlast,
	input				rvalid,
	output	reg			rready,
    //aw           
	output [3:0]		awid,
	output [31:0]		awaddr,
	output [7:0]		awlen,
	output [2:0]		awsize,
	output [1:0]		awburst,
	output [1:0]		awlock,
	output [3:0]		awcache,
	output [2:0]		awprot,
	output				awvalid,
	input				awready,
    //w          
	output [3:0]		wid,
	output [31:0]		wdata,
	output reg[3:0]		wstrb,
	output				wlast,
	output				wvalid,
	input				wready,
    //b              
	input [3:0]			bid,
	input [1:0]			bresp,
	input				bvalid,
	output reg			bready
);

reg [31:0] read_addr_inst;
reg [2:0] read_size_inst;
reg [31:0] read_addr_data;
reg [2:0] read_size_data;
reg [1:0] cstate_ar_inst;
reg [1:0] cstate_ar_data;
reg arvalid_inst;
reg arvalid_data;
reg data_addr_r_f;
reg inst_addr_r_f;
reg [1:0]has_data_read;
reg [1:0]has_data_write;
reg [1:0] cstate_r;
reg [31:0] rdata_r;
reg [3:0] rid_r;
reg inst_data_ok_r;
reg data_data_ok_r;

reg data_data_ok_w;
reg [1:0]cstate_aw;
reg [31:0] write_addr;
reg [2:0] write_size;
reg awvalid_r;

//w
reg [31:0] write_data;
reg wvalid_r;
reg [1:0]success_w;	
reg [1:0] cstate_b;
reg [3:0]bid_r;
//read inst request
reg mark_for_datareq;
always @(posedge clk) begin
	if(~resetn) begin 
		cstate_ar_inst = `STATE_ONE;
		read_addr_inst <= 32'b0;
		read_size_inst <= 3'b0;
		arvalid_inst <= 1'b0;
		inst_addr_r_f <= 1'b0;
		mark_for_datareq <= 1'b0;
	end
	else if(cstate_ar_inst == `STATE_ONE) begin
		if(inst_req && ~inst_wr && inst_addr_ok && ~arvalid_inst && data_addr_r_f) begin
			read_addr_inst <= inst_addr;
			read_size_inst <= {1'b0,inst_size};
			mark_for_datareq <= 1'b1;
		end
		else if(inst_req && ~inst_wr && inst_addr_ok && ~arvalid_inst ) begin
			cstate_ar_inst <= `STATE_TWO;
			arvalid_inst <= 1'b1;
			inst_addr_r_f <= 1'b1;
			read_addr_inst <= inst_addr;
			read_size_inst <= {1'b0,inst_size};
		end
		if(mark_for_datareq && rready && rvalid) begin
			cstate_ar_inst <= `STATE_TWO;
			arvalid_inst <= 1'b1;
			inst_addr_r_f <= 1'b1;
			mark_for_datareq <= 1'b0;
		end
	end
	else if(cstate_ar_inst == `STATE_TWO) begin
		//握手成功
		if(arready && arvalid && (~arvalid_data)) begin
			//cstate_ar_inst <= `STATE_ONE;
			arvalid_inst <= 1'b0;
			//inst_addr_r_f <= 1'b0;
		end
		if(~arvalid_inst && rready && rvalid) begin
			cstate_ar_inst <= `STATE_ONE;
			inst_addr_r_f <= 1'b0;
		end
	end
end
//read data
reg mark_for_instreq;
always @(posedge clk) begin
	if(~resetn) begin 
		cstate_ar_data = `STATE_ONE;
		read_addr_data <= 32'b0;
		read_size_data <= 3'b0;
		arvalid_data <= 1'b0;
		data_addr_r_f <= 1'b0;
		mark_for_instreq <= 1'b0;
	end
	else if(cstate_ar_data == `STATE_ONE) begin
		if(data_req && ~data_wr && data_addr_ok && ~arvalid_data && inst_addr_r_f)begin
			mark_for_instreq <= 1'b1;
			read_addr_data <= data_addr;
			read_size_data <= {1'b0,data_size};
		end
		else if(data_req && ~data_wr && data_addr_ok && ~arvalid_data ) begin
			cstate_ar_data <= `STATE_TWO;
			arvalid_data <= 1'b1;
			data_addr_r_f <= 1'b1;
			read_addr_data <= data_addr;
			read_size_data <= {1'b0,data_size};
			has_data_read <= 1'b00;
		end
		if(rready && rvalid && mark_for_instreq) begin
			cstate_ar_data <= `STATE_TWO;
			arvalid_data <= 1'b1;
			data_addr_r_f <= 1'b1;
			has_data_read <= 1'b00;
			mark_for_instreq <= 1'b0;
		end
	end
	else if(cstate_ar_data == `STATE_TWO) begin
		//握手成功
		if(arready && arvalid) begin
			//是不是应该放在下面，以便阻塞inst的读。
			arvalid_data <= 1'b0;
			has_data_read <= 2'b01;
		end
		//读反馈
		else if(has_data_read == 2'b01 && rready && rvalid) begin
			data_addr_r_f <= 1'b0;
			has_data_read <= 2'b10;
			cstate_ar_data <= `STATE_ONE;
		end
	end
end

always @(posedge clk) begin
	if(!resetn)
		inst_addr_ok <= 1'b0;
	else if(inst_addr_ok)
		inst_addr_ok <= 1'b0;
	else if(inst_req && cstate_ar_inst == `STATE_ONE && ~data_addr_r_f)
		inst_addr_ok <= 1'b1;
end

always @(posedge clk) begin
	if(!resetn)
		data_addr_ok <= 1'b0;
	else if(data_addr_ok)
		data_addr_ok <= 1'b0;
	else if(data_req && (cstate_ar_data == `STATE_ONE && ~inst_addr_r_f) && cstate_aw == `STATE_ONE )
		data_addr_ok <= 1'b1;
end

assign arlen = 8'b0;
assign arburst = 2'b01;
assign arlock = 2'b00;
assign arcache =4'b0;
assign arprot = 3'b0;

/*assign arid = (data_addr_r_f & ~WAR_block_data) ? ({4{arready & arvalid}} & `ID_DATA) : //data 读有效且无冲突 
			  ((inst_addr_r_f & ~WAR_block_inst) ? ({4{arready & arvalid}} & `ID_INST) : 1'b0);

assign araddr = (data_addr_r_f & ~WAR_block_data) ? ({32{arready & arvalid}} & read_addr_data) : //data 读有效且无冲突 
				((inst_addr_r_f & ~WAR_block_inst) ? ({32{arready & arvalid}} & read_addr_inst) : 32'b0);
assign arsize = (data_addr_r_f & ~WAR_block_data) ? ({3{arready & arvalid}} & read_size_data) : //data 读有效且无冲突 
				((inst_addr_r_f & ~WAR_block_inst) ? ({3{arready & arvalid}} & read_size_inst) : 3'b0);
				
assign arvalid = (data_addr_r_f & ~WAR_block_data) ? arvalid_data : //data 读有效且无冲突 
				((inst_addr_r_f & ~WAR_block_inst) ? arvalid_inst : 1'b0);
*/
assign arid = (data_addr_r_f ) ? (`ID_DATA) : //data 读有效且无冲突 
			  ((inst_addr_r_f) ? (`ID_INST) : 1'b0);

assign araddr = (data_addr_r_f) ? (read_addr_data) : //data 读有效且无冲突 
				((inst_addr_r_f) ? (read_addr_inst) : 32'b0);
assign arsize = (data_addr_r_f) ? (read_size_data) : //data 读有效且无冲突 
				((inst_addr_r_f) ? (read_size_inst) : 3'b0);
				
assign arvalid = (data_addr_r_f) ? arvalid_data : //data 读有效且无冲突 
				((inst_addr_r_f) ? arvalid_inst : 1'b0);
//read response
always @(posedge clk) begin
	if(~resetn) begin 
		cstate_r <= `STATE_ONE;
		rid_r <= 4'b0;
		rdata_r <= 32'b0;
	end
	else if(cstate_r == `STATE_ONE) begin
		if(rvalid & rready) begin
			rdata_r <= rdata;
			rid_r <= rid;
			cstate_r <= `STATE_TWO;
		end
	end
	else if(cstate_r == `STATE_TWO) begin
		rid_r <= 4'b1111;
		if(arready && arvalid) begin
			cstate_r <= `STATE_ONE;
		end
	end
end

// read ok

always@(posedge clk)
begin
    if(!resetn)
		rready <= 1'd0;
    if(arready & arvalid)
        rready <= 1'd1;
    else if(rready & rvalid)
        rready <= 1'd0; 
end
always @(posedge clk) begin
	if(!resetn)
		inst_data_ok_r <= 1'b0;
	else if(inst_data_ok_r)
		inst_data_ok_r <= 1'b0;
	else if((rid_r == `ID_INST) & (cstate_r == `STATE_TWO))
		inst_data_ok_r <= 1'b1;
end
always @(posedge clk) begin
	if(!resetn)
		data_data_ok_r <= 1'b0;
	else if(data_data_ok_r)
		data_data_ok_r <= 1'b0;
	else if((rid_r == `ID_DATA) & (cstate_r == `STATE_TWO))
		data_data_ok_r <= 1'b1;
end

assign inst_rdata = {32{inst_data_ok_r}} & rdata_r; 		
assign data_rdata = {32{data_data_ok_r}} & rdata_r;


//write ok	 
always @(posedge clk) begin
	if(~resetn) begin 
		cstate_aw = `STATE_ONE;
		write_addr <= 32'b0;
		write_size <= 3'b0;
		awvalid_r <= 1'b0;
		write_data <= 32'b0;
		wvalid_r <= 1'b0;
		success_w <= 2'b0;
	end
	else if(cstate_aw == `STATE_ONE) begin
		if(data_req && data_wr && data_addr_ok && ~awvalid_r && ~wvalid_r) begin
			cstate_aw <= `STATE_TWO;
			awvalid_r <= 1'b1;
			wvalid_r <= 1'b1;
			write_size <= {1'b0, data_size};
			write_data <= data_wdata;
			write_addr <= data_addr;
			success_w <= 2'b0;
			has_data_write <= 2'b00;
		end
	end
	else if(cstate_aw == `STATE_TWO) begin
		if((awvalid && awready && wvalid && wready) || (awvalid && awready && success_w == 2'b10) || (wvalid && wready && success_w == 2'b01)) begin
			success_w <= 2'b0;
			awvalid_r <= 1'b0;
			wvalid_r <= 1'b0;
			write_size <= 3'b0;
			has_data_write <= 2'b01;
		end
		else if(awvalid && awready) begin
			success_w <= 2'b01;
			awvalid_r <= 1'b0;
		end
		else if(wvalid && wready) begin
			success_w <= 2'b10;
			wvalid_r <= 1'b0;
		end
		else if(bready && bvalid && has_data_write == 2'b01) begin
			has_data_write <= 2'b10;
			write_addr <= 32'b0;
			write_data <= 32'b0;
			cstate_aw <= `STATE_ONE;
		end
	end
end
assign awvalid = awvalid_r;
assign awid = `ID_DATA;
assign awlen = 1'b0;
assign awburst = 2'b01;
assign awlock = 2'b0;
assign awcache = 4'b0;
assign awprot = 3'b0; 
/*assign awaddr =  {32{awready && awvalid}} & write_addr;
assign awsize = {3{awready && awvalid}} & write_size;*/
assign awaddr =   write_addr;
assign awsize =  write_size;
//w
assign wid = `ID_DATA;
assign wlast = 1'b1;
//assign wdata = {32{wready && wvalid}} & write_data;
assign wdata = write_data;
assign wvalid = wvalid_r;

//b write response

always @(posedge clk) begin
	if(~resetn) begin
		cstate_b <= `STATE_ONE;
		bid_r <= 4'b0;
	end
	else if(cstate_b == `STATE_ONE) begin
		if(bvalid && bready) begin			
			cstate_b <= `STATE_TWO;
			bid_r <= bid;
		end
	end
	else if(cstate_b == `STATE_TWO) begin
			bid_r <= 4'b1111;
		if((awvalid && awready && wvalid && wready) || (awvalid && awready && success_w == 2'b10) || (wvalid && wready && success_w == 2'b01)) begin
			cstate_b <= `STATE_ONE;
		end
	end
end

always @(posedge clk)
begin
	if(!resetn)
		bready <= 1'b0;
	else if((awvalid && awready && wvalid && wready) || (awvalid && awready && success_w == 2'b10) || (wvalid && wready && success_w == 2'b01))
		bready <= 1'b1;
	else if(bvalid & bready)
		bready <= 1'b0;
end

always @(posedge clk) begin
	if(!resetn)
		data_data_ok_w <= 1'b0;
	else if(data_data_ok_w)
		data_data_ok_w <= 1'b0;
	else if(cstate_b == `STATE_TWO && bid_r == `ID_DATA)
		data_data_ok_w <= 1'b1;
end


always @(*) begin
	case({write_size, write_addr[1:0]})
	5'b000_00: wstrb = 4'b0001; // SB
    5'b000_01: wstrb = 4'b0010; // SB
    5'b000_10: wstrb = 4'b0100; // SB
    5'b000_11: wstrb = 4'b1000; // SB
    5'b001_00: wstrb = 4'b0011; // SH
    5'b001_10: wstrb = 4'b1100; // SH
    5'b010_00: wstrb = 4'b1111; // SW
    5'b000_00: wstrb = 4'b0001; // SWL
    5'b001_01: wstrb = 4'b0011; // SWL
    5'b010_10: wstrb = 4'b0111; // SWL
    5'b010_11: wstrb = 4'b1111; // SWL
    5'b010_00: wstrb = 4'b1111; // SWR
    5'b010_01: wstrb = 4'b1110; // SWR
    5'b001_10: wstrb = 4'b1100; // SWR
    5'b000_11: wstrb = 4'b1000; // SWR
	default: wstrb = 4'b0;
	endcase
end

assign inst_data_ok = inst_data_ok_r;
assign data_data_ok = data_data_ok_r || data_data_ok_w;


endmodule