module cache(
	input clk,
	input resetn,
	
	//cache cpu
	input 			valid,
	input 			op,
	input [7:0] 	index,
	input [19:0] 	tlb_tag,
	input [3:0] 	offset,
	input [3:0] 	wstrb,
	input [31:0] 	wdata,
	output 			addr_ok,
	output 			data_ok,
	output [31:0] 	rdata,
	
	//cache axi read
	output 			rd_req,
	output [2:0]	rd_type,
	output [31:0]	rd_addr,
	input			rd_rdy,
	input 			ret_valid,
	input  			ret_last,
	input  [31:0]   ret_data,
	//cache axi write
	output 			wr_req,
	output [2:0]	wr_type,
	output [31:0]	wr_addr,
	output [3:0]	wr_wstrb,
	output [127:0]	wr_data,
	input 			wr_rdy
);

reg	[255:0] cache_D0;
reg [255:0] cache_D1;


parameter IDLE = 5'b00001;
parameter LOOKUP = 5'b00010;
parameter MISS =  5'b00100;
parameter REPLACE = 5'b01000;
parameter REFILL = 5'b10000;
reg [4:0] cstate;

wire Way0_TV_en;
wire [7:0] Way0_TV_addr;
wire [20:0] Way0_TV_wdata;
wire [20:0] Way0_TV_rdata;

wire Way1_TV_en;
wire [7:0] Way1_TV_addr;
wire [20:0] Way1_TV_wdata;
wire [20:0] Way1_TV_rdata;

wire [3:0] Way0_bank0_en;
wire [7:0] Way0_bank0_addr;
wire [31:0] Way0_bank0_wdata;
wire [31:0] Way0_bank0_rdata;

wire [3:0] Way0_bank1_en;
wire [7:0] Way0_bank1_addr;
wire [31:0] Way0_bank1_wdata;
wire [31:0] Way0_bank1_rdata;

wire [3:0] Way0_bank2_en;
wire [7:0] Way0_bank2_addr;
wire [31:0] Way0_bank2_wdata;
wire [31:0] Way0_bank2_rdata;

wire [3:0] Way0_bank3_en;
wire [7:0] Way0_bank3_addr;
wire [31:0] Way0_bank3_wdata;
wire [31:0] Way0_bank3_rdata;

wire [3:0] Way1_bank0_en;
wire [7:0] Way1_bank0_addr;
wire [31:0] Way1_bank0_wdata;
wire [31:0] Way1_bank0_rdata;

wire [3:0] Way1_bank1_en;
wire [7:0] Way1_bank1_addr;
wire [31:0] Way1_bank1_wdata;
wire [31:0] Way1_bank1_rdata;

wire [3:0] Way1_bank2_en;
wire [7:0] Way1_bank2_addr;
wire [31:0] Way1_bank2_wdata;
wire [31:0] Way1_bank2_rdata;

wire [3:0] Way1_bank3_en;
wire [7:0] Way1_bank3_addr;
wire [31:0] Way1_bank3_wdata;
wire [31:0] Way1_bank3_rdata;


reg [3:0] wstrb_r;
reg [31:0] wdata_r;
reg [7:0] index_r;
reg [19:0] tlb_tag_r;
reg [3:0] offset_r;
reg op_r;
reg index_tra;
reg cache_write_refill;


reg cnt;
always @(posedge clk) begin
	if(resetn == 0) begin
		cnt <= 1'b0;
	end
	else if(cnt == 1'b1) begin
		cnt <= 1'b0;
	end
	else if(cnt == 1'b0) begin
		cnt <= 1'b1;
	end
end

//发请求前一定要先选好index_tr
always @(posedge clk)
begin
	if(resetn == 0)begin
		cstate <= IDLE;
		index_tra <= 1'b0;
		op_r <= 1'b0;
		wstrb_r <= 4'b0;
		wdata_r <= 32'b0;
		index_r <= 8'b0;
		tlb_tag_r <= 20'b0;
		offset_r <= 4'b0;		
	end
	else if(cstate == IDLE) begin
		if(valid) begin
			cstate <= LOOKUP;
			op_r <= op;
			wstrb_r <= wstrb;
			wdata_r <= wdata;
			index_r <= index;
			tlb_tag_r <= tlb_tag;
			offset_r <= offset;
			cache_write_refill <= 1'b0;
		end
	end
	else if(cstate == LOOKUP) begin
		if( cache_hit == 1'b0) begin
			cstate <= MISS;
			index_tra <= cnt;
		end
		else if(valid == 1'b0 && cache_hit == 1'b1 && data_ok == 1) begin
			cstate <= IDLE;
		end
		else if(valid == 1'b1 && cache_hit == 1'b1 && data_ok == 1) begin
			cstate <= LOOKUP;
			op_r <= op;
			wstrb_r <= wstrb;
			wdata_r <= wdata;
			index_r <= index;
			tlb_tag_r <= tlb_tag;
			offset_r <= offset;
		end
	end
	//miss要末是tag不对，要么是valid = 0
	//发生MISS需要做的事情包括,随机选取cache行,写回主存，读取所需内容，写入cache
	else if(cstate == MISS) begin
		//cache write back
		if(cache_D0[index_r] && way0_valid && index_tra == 0) begin
			//if(wr_rdy && wr_req == 0 ) begin
			if(mark_for_write == 2'b10 && wr_rdy) begin
				cstate <= REPLACE;	
			end
		end
		else if(cache_D1[index_r] && way1_valid && index_tra == 1) begin
			//if(wr_rdy && wr_req == 0 ) begin
			if(mark_for_write == 2'b10 && wr_rdy) begin
				cstate <= REPLACE;	
			end
		end
		else begin
			cstate <= REPLACE;
		end
	end
	else if(cstate == REPLACE) begin
		//read cache from memory
		if(ret_last == 1'b1)begin
			cstate <= REFILL;
			cache_write_refill <= 1'b1;
		end
	end
	else if(cstate == REFILL) begin
		//写入cache
		if(cache_write_refill) begin
			cache_write_refill <= 1'b0;
		end
		
		if(data_ok == 1'b1) begin
			cstate <= IDLE;
		end
	end
end
//axi read
reg rd_req_r;
reg [2:0]rd_type_r;
reg [31:0]rd_addr_r;
assign rd_req = rd_req_r;
assign rd_type = rd_type_r;
assign rd_addr = rd_addr_r;
reg [1:0]has_axi_read_req;

//axi read
always @(posedge clk) begin
	if(resetn == 0) begin
		rd_req_r <= 1'b0;
		rd_type_r <= 3'b0;
		rd_addr_r <= 32'b0;
		has_axi_read_req <= 2'b00;
	end
	else if(cstate == REPLACE && (has_axi_read_req == 2'b00)) begin
		rd_req_r <= 1'b1;
		rd_type_r <= 3'b100;
		rd_addr_r <= {tlb_tag_r, index_r, 4'b0000};
		has_axi_read_req <= 2'b01;
	end
	else if(rd_rdy && rd_req_r && (has_axi_read_req == 2'b01)) begin
		rd_req_r <= 1'b0;
		has_axi_read_req <= 2'b10;
	end
	else if((ret_last == 1'b1) && (has_axi_read_req == 2'b10))begin
		has_axi_read_req <= 2'b11;
	end
	else if(has_axi_read_req == 2'b11 && cstate == REPLACE) begin
		has_axi_read_req <= 2'b00;
	end
end


reg [1:0] ret_cnt;
wire [31:0] cache_write_data;
assign cache_write_data[31:24] = wstrb_r[3] ? wdata_r[31:24] : ret_data[31:24];
assign cache_write_data[23:16] = wstrb_r[2] ? wdata_r[23:16] : ret_data[23:16];
assign cache_write_data[15:8] = wstrb_r[1] ? wdata_r[15:8] : ret_data[15:8];
assign cache_write_data[7:0] = wstrb_r[0] ? wdata_r[7:0] : ret_data[7:0];

reg [31:0]axi_read_buffer[3:0];
always @(posedge clk) begin
	if(resetn == 0) begin
		ret_cnt <= 2'b0;
	end
	if(ret_valid) begin
		//write miss 直接修改写入cache的数据
		if(op_r && offset_r[3:2] == ret_cnt) begin
			axi_read_buffer[ret_cnt] <= cache_write_data;
		end
		else begin
			axi_read_buffer[ret_cnt] <= ret_data;
		end

		if(ret_cnt == 2'b11) begin
			ret_cnt <= 2'b0;
		end
		else begin
			ret_cnt <= ret_cnt + 1;
		end
	end
end

reg wr_req_r;

assign wr_req = wr_req_r;
assign wr_type = 3'b100;
assign wr_addr = index_tra ? {Way1_TV_rdata[20:1], index_r, 4'b0000} : {Way0_TV_rdata[20:1],index_r, 4'b0000};
assign wr_wstrb = 4'b1111;
assign wr_data = index_tra ? {Way1_bank3_rdata, Way1_bank2_rdata, Way1_bank1_rdata, Way1_bank0_rdata} : 
							 {Way0_bank3_rdata, Way0_bank2_rdata, Way0_bank1_rdata, Way0_bank0_rdata};

//axi write back
/*
always @(posedge clk) begin
	if(resetn == 0) begin
		wr_req_r <= 1'b0;
	end
	else if(cstate == LOOKUP) begin
		if( cache_hit == 1'b0) begin
			if(index_tra == 0) begin
				if(cache_D0[index_r] && way0_valid) begin
					wr_req_r <= 1'b1;
				end
			end
			else if(index_tra == 1) begin
				if(cache_D1[index_r] && way1_valid) begin
					wr_req_r <= 1'b1;
				end
			end	
		end
	end
	else if(cstate == MISS) begin
		
		if(index_tra == 0) begin
			if(cache_D0[index_r] && way0_valid) begin
				wr_req_r <= 1'b1;
			end
		end
		else if(index_tra == 1) begin
			if(cache_D1[index_r] && way1_valid) begin
				wr_req_r <= 1'b1;
			end
		end
		else if(wr_req && wr_rdy) begin
			wr_req_r <= 1'b0;
		end
		
		//if(wr_req && wr_rdy) begin
		//	wr_req_r <= 1'b0;
		//end
	end
end
*/
reg [1:0] mark_for_write;
always @(posedge clk) begin
	if(resetn == 0) begin
		wr_req_r <= 1'b0;
		mark_for_write <= 2'b00;
	end
	else if(cstate == MISS) begin	
		if( cache_hit == 1'b0 && wr_req_r == 0 && mark_for_write == 2'b00) begin
			if(index_tra == 0) begin
				if(cache_D0[index_r] && way0_valid) begin
					wr_req_r <= 1'b1;
					mark_for_write <= 2'b01;
				end
			end
			else if(index_tra == 1) begin
				if(cache_D1[index_r] && way1_valid) begin
					wr_req_r <= 1'b1;
					mark_for_write <= 2'b01;
				end
			end	
		end
		else if(wr_req && wr_rdy ) begin
			wr_req_r <= 1'b0;
			mark_for_write <= 2'b10;
		end
		else if(mark_for_write == 2'b10 && wr_rdy) begin
			mark_for_write <= 2'b00;
		end
	end
end
reg data_ok_r;
reg addr_ok_r;


//addr_ok_r
always @(posedge clk)
begin
	if(resetn == 0)begin
		addr_ok_r <= 1'b0;
	end
	else if(addr_ok) begin
		addr_ok_r <= 1'b0;
	end
	else if(cstate == IDLE) begin
		if(valid) begin
			addr_ok_r <= 1'b1;
		end
	end
	else if(cstate == LOOKUP) begin
		if(valid == 1 && cache_hit == 1 && data_ok) begin
			addr_ok_r <= 1'b1;
		end
	end
end

assign data_ok = data_ok_r;
assign addr_ok = addr_ok_r;

//dirty
always @(posedge clk) begin
	if(resetn == 0) begin
		cache_D0 <= 256'b0;
	end
	else if(way0_hit_write) begin
		cache_D0[index_r] <= 1'b1;
	end 
	else if(cache_write_refill && index_tra == 0 && op_r == 1'b1) begin
		cache_D0[index_r] <= 1'b1;
	end
	else if(cache_write_refill && index_tra == 0 && op_r == 1'b0) begin
		cache_D0[index_r] <= 1'b0;
	end
end

always @(posedge clk) begin
	if(resetn == 0) begin
		cache_D1 <= 256'b0;
	end
	else if(way1_hit_write) begin
		cache_D1[index_r] <= 1'b1;
	end 
	else if(cache_write_refill && index_tra == 1 && op_r == 1'b1) begin
		cache_D1[index_r] <= 1'b1;
	end
	else if(cache_write_refill && index_tra == 1 && op_r == 1'b0) begin
		cache_D1[index_r] <= 1'b0;
	end
end


assign Way0_TV_addr = index_r;
assign Way1_TV_addr = index_r;
assign Way0_TV_en = (cache_write_refill && index_tra == 0) ? 1'b1 : 1'b0;
assign Way1_TV_en = (cache_write_refill && index_tra == 1) ? 1'b1 : 1'b0;
assign Way1_TV_wdata = {tlb_tag_r, 1'b1};
assign Way0_TV_wdata = {tlb_tag_r, 1'b1};
wire way0_hit;
wire way1_hit;
wire cache_hit;
wire way0_valid;
wire way1_valid;
assign way0_valid = Way0_TV_rdata[0];
assign way1_valid = Way1_TV_rdata[0];
assign way0_hit = way0_valid && (Way0_TV_rdata[20:1] == tlb_tag_r);
assign way1_hit = way1_valid && (Way1_TV_rdata[20:1] == tlb_tag_r);
assign cache_hit = way0_hit || way1_hit;


//根据index读出tag和state
Way0_TV Way0_TV(
	.clka(clk), 
	.wea(Way0_TV_en),
	.addra(Way0_TV_addr),//[7:0]
	.dina(Way0_TV_wdata),//[20:0]
	.douta(Way0_TV_rdata)//[20:0]
);

Way1_TV Way1_TV(
	.clka(clk), 
	.wea(Way1_TV_en),
	.addra(Way1_TV_addr),//[7:0]
	.dina(Way1_TV_wdata),//[20:0]
	.douta(Way1_TV_rdata)//[20:0]
);

wire [31:0]way0_load_word;
wire [31:0]way1_load_word;

assign way0_load_word = ({32{offset_r == 4'b0000}} & Way0_bank0_rdata) |
						({32{offset_r == 4'b0100}} & Way0_bank1_rdata) |
						({32{offset_r == 4'b1000}} & Way0_bank2_rdata) |
						({32{offset_r == 4'b1100}} & Way0_bank3_rdata);
assign way1_load_word = ({32{offset_r == 4'b0000}} & Way1_bank0_rdata) |
						({32{offset_r == 4'b0100}} & Way1_bank1_rdata) |
						({32{offset_r == 4'b1000}} & Way1_bank2_rdata) |
						({32{offset_r == 4'b1100}} & Way1_bank3_rdata);
assign rdata = ({32{way0_hit}} & way0_load_word) | 
				   ({32{way1_hit}} & way1_load_word);

//data_ok_r
always @(posedge clk)
begin
	if(resetn == 0)begin
		data_ok_r <= 1'b0;
	end
	else if(data_ok_r) begin
		data_ok_r <= 1'b0;
	end
	else if(cstate == IDLE) begin
		data_ok_r <= 1'b0;
	end
	else if(cstate == LOOKUP) begin
		if(op_r == 1'b1) begin
			if(way0_hit_write_finish || way1_hit_write_finish)begin
				data_ok_r <= 1'b1;
			end
		end
		else if(op_r == 0) begin
			if(cache_hit) begin
				data_ok_r <= 1'b1;
			end
		end
	end
	else if(cstate == REFILL) begin
		if(cache_write_refill == 1'b0) begin
			data_ok_r <= 1'b1;
		end
	end
end

reg way0_hit_write;
reg way0_hit_write_finish;
reg way1_hit_write_finish;
always @(posedge clk) begin
	if(resetn == 0) begin
		way0_hit_write <= 1'b0;
	end
	else if(way0_hit_write  == 1'b1) begin
		way0_hit_write <= 1'b0;
	end
	else if(way0_hit && op_r &&way0_hit_write_finish == 1'b0 && cstate == LOOKUP && data_ok == 0) begin
		way0_hit_write <= 1'b1;
	end
	
	if(resetn == 0) begin
		way0_hit_write_finish <= 1'b0;
	end
	else if(way0_hit_write_finish) begin
		way0_hit_write_finish <= 1'b0;
	end
	else if(way0_hit_write) begin
		way0_hit_write_finish <= 1'b1;
	end
end
reg way1_hit_write;
always @(posedge clk) begin
	if(resetn == 0) begin
		way1_hit_write <= 1'b0;
	end
	else if(way1_hit_write  == 1'b1) begin
		way1_hit_write <= 1'b0;
	end
	else if(way1_hit && op_r && way1_hit_write_finish == 1'b0&& cstate == LOOKUP && data_ok == 0) begin
		way1_hit_write <= 1'b1;
	end
	
	if(resetn == 0) begin
		way1_hit_write_finish <= 1'b0;
	end
	else if(way1_hit_write_finish) begin
		way1_hit_write_finish <= 1'b0;
	end
	else if(way1_hit_write) begin
		way1_hit_write_finish <= 1'b1;
	end
end

assign Way0_bank0_en = (way0_hit_write && offset_r == 4'b0000) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 0) ? 4'b1111 : 4'b0000);
assign Way0_bank0_addr = index_r;
assign Way0_bank0_wdata = (cache_write_refill && index_tra == 0) ? axi_read_buffer[0]: wdata_r;

assign Way0_bank1_en = (way0_hit_write && offset_r == 4'b0100) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 0) ? 4'b1111 : 4'b0000);
assign Way0_bank1_addr = index_r;
assign Way0_bank1_wdata = (cache_write_refill && index_tra == 0) ? axi_read_buffer[1]: wdata_r;

assign Way0_bank2_en = (way0_hit_write && offset_r == 4'b1000) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 0) ? 4'b1111 : 4'b0000);
assign Way0_bank2_addr = index_r;
assign Way0_bank2_wdata = (cache_write_refill && index_tra == 0) ? axi_read_buffer[2]: wdata_r;

assign Way0_bank3_en = (way0_hit_write && offset_r == 4'b1100) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 0) ? 4'b1111 : 4'b0000);
assign Way0_bank3_addr = index_r;
assign Way0_bank3_wdata = (cache_write_refill && index_tra == 0) ? axi_read_buffer[3]: wdata_r;

assign Way1_bank0_en = (way1_hit_write && offset_r == 4'b0000) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 1) ? 4'b1111 : 4'b0000);
assign Way1_bank0_addr = index_r;
assign Way1_bank0_wdata = (cache_write_refill && index_tra == 1) ? axi_read_buffer[0]: wdata_r;

assign Way1_bank1_en = (way1_hit_write && offset_r == 4'b0100) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 1) ? 4'b1111 : 4'b0000);
assign Way1_bank1_addr = index_r;
assign Way1_bank1_wdata = (cache_write_refill && index_tra == 1) ? axi_read_buffer[1]: wdata_r;

assign Way1_bank2_en = (way1_hit_write && offset_r == 4'b1000) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 1) ? 4'b1111 : 4'b0000);
assign Way1_bank2_addr = index_r;
assign Way1_bank2_wdata = (cache_write_refill && index_tra == 1) ? axi_read_buffer[2]: wdata_r;

assign Way1_bank3_en = (way1_hit_write && offset_r == 4'b1100) ? wstrb_r : 
					   ((cache_write_refill && index_tra == 1) ? 4'b1111 : 4'b0000);
assign Way1_bank3_addr = index_r;
assign Way1_bank3_wdata = (cache_write_refill && index_tra == 1) ? axi_read_buffer[3]: wdata_r;

Way0_bank0 Way0_bank0(
	.clka(clk), 
	.wea(Way0_bank0_en),//[3:0]
	.addra(Way0_bank0_addr),//[7:0]
	.dina(Way0_bank0_wdata),//[31:0]
	.douta(Way0_bank0_rdata)//[31:0]
);

Way0_bank1 Way0_bank1(
	.clka(clk), 
	.wea(Way0_bank1_en),//[3:0]
	.addra(Way0_bank1_addr),//[7:0]
	.dina(Way0_bank1_wdata),//[31:0]
	.douta(Way0_bank1_rdata)//[31:0]
);

Way0_bank2 Way0_bank2(
	.clka(clk), 
	.wea(Way0_bank2_en),//[3:0]
	.addra(Way0_bank2_addr),//[7:0]
	.dina(Way0_bank2_wdata),//[31:0]
	.douta(Way0_bank2_rdata)//[31:0]
);


Way0_bank3 Way0_bank3(
	.clka(clk), 
	.wea(Way0_bank3_en),//[3:0]
	.addra(Way0_bank3_addr),//[7:0]
	.dina(Way0_bank3_wdata),//[31:0]
	.douta(Way0_bank3_rdata)//[31:0]
);


Way1_bank0 Way1_bank0(
	.clka(clk), 
	.wea(Way1_bank0_en),//[3:0]
	.addra(Way1_bank0_addr),//[7:0]
	.dina(Way1_bank0_wdata),//[31:0]
	.douta(Way1_bank0_rdata)//[31:0]
);


Way1_bank1 Way1_bank1(
	.clka(clk), 
	.wea(Way1_bank1_en),//[3:0]
	.addra(Way1_bank1_addr),//[7:0]
	.dina(Way1_bank1_wdata),//[31:0]
	.douta(Way1_bank1_rdata)//[31:0]
);


Way1_bank2 Way1_bank2(
	.clka(clk), 
	.wea(Way1_bank2_en),//[3:0]
	.addra(Way1_bank2_addr),//[7:0]
	.dina(Way1_bank2_wdata),//[31:0]
	.douta(Way1_bank2_rdata)//[31:0]
);


Way1_bank3 Way1_bank3(
	.clka(clk), 
	.wea(Way1_bank3_en),//[3:0]
	.addra(Way1_bank3_addr),//[7:0]
	.dina(Way1_bank3_wdata),//[31:0]
	.douta(Way1_bank3_rdata)//[31:0]
);

endmodule
