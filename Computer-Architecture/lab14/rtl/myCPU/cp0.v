`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2019/10/27 13:17:54
// Design Name: 
// Module Name: cp0
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
`define CP0_BADV_ADDR       8'b01000_000
`define CP0_COUNT_ADDR      8'b01001_000
`define CP0_COMP_ADDR       8'b01011_000
`define CP0_STATUS_ADDR     8'b01100_000
`define CP0_CAUSE_ADDR      8'b01101_000
`define CP0_EPC_ADDR        8'b01110_000
`define CP0_CONFIG_ADDR     8'b10000_000
// lab13 append
`define CP0_ENTRYHI_ADDR    8'b01010_000
`define CP0_ENTRYLO0_ADDR   8'b00010_000
`define CP0_ENTRYLO1_ADDR   8'b00011_000
`define CP0_INDEX_ADDR      8'b00000_000

module cp0(
    input         clk,
    input         rst, 
    // EXCEPETION
    input         wb_ex,
    input         wb_bd,
    input  [4:0]  wb_excode,
    input  [31:0] wb_pc,
    input  [31:0] wb_badvaddr,
    input         eret_flush,
    input  [5:0]  ext_int_in,   
    // READ PORT
    input  [ 7:0] cp0_addr,
    output [31:0] cp0_rdata,
    // WRITE PORT
    input         mtc0_we,       
    input  [31:0] cp0_wdata,   
    // OUTPUT PORT
    output [31:0] cp0_status_out,
    output [31:0] cp0_cause_out,
    output [31:0] cp0_epc_out,
    output [31:0] cp0_badvaddr_out,
    output [31:0] cp0_count_out,
    output [31:0] cp0_compare_out,
    output [31:0] cp0_config_out,
    output        count_eq_compare_out,

    input         ws_tlb_ex,    
    // lab13 append
    output [31:0] cp0_entryhi,
    output [31:0] cp0_entrylo0,
    output [31:0] cp0_entrylo1,
    output [31:0] cp0_index,
    
    // lab13 TLB
    input        tlbr,
    input        tlbwi,
    input        tlbp,

    input        es_tlbp,
        
    // tlb search port 0
    input         s0_found,
    input [ 3:0]  s0_index,
    input [25:6]  s0_pfn,
    input [ 5:3]  s0_c,
    input         s0_d,
    input         s0_v,

    // tlb search port 1        
    input         s1_found,
    input [ 3:0]  s1_index,
    input [25:6]  s1_pfn,
    input [ 5:3]  s1_c,
    input         s1_d,
    input         s1_v,

    // tlb write port
    output [ 3:0] w_index,

    // tlb read port
    input [              18:0] r_vpn2,     
    input [               7:0] r_asid,     
    input                      r_g,     
    input [              19:0] r_pfn0,     
    input [               2:0] r_c0,     
    input                      r_d0,     
    input                      r_v0,     
    input [              19:0] r_pfn1,     
    input [               2:0] r_c1,     
    input                      r_d1,     
    input                      r_v1            
);

wire count_eq_compare;
// CP0 REGS    
wire [31:0] cp0_status;
wire [31:0] cp0_cause;
wire [31:0] cp0_epc;
wire [31:0] cp0_badvaddr;
wire [31:0] cp0_count;
wire [31:0] cp0_compare;
wire [31:0] cp0_config;

/*
wire [31:0] cp0_entryhi;
wire [31:0] cp0_entrylo0;
wire [31:0] cp0_entrylo1;
wire [31:0] cp0_index;
*/

assign count_eq_compare     = (cp0_count == cp0_compare);
assign count_eq_compare_out = count_eq_compare;


assign cp0_epc_out      = cp0_epc;
assign cp0_status_out   = cp0_status;
assign cp0_cause_out    = cp0_cause;
assign cp0_count_out    = cp0_count;
assign cp0_compare_out  = cp0_compare;
assign cp0_config_out = 32'b0;

assign cp0_config_out = 32'b0;
assign cp0_badvaddr_out = cp0_badvaddr_out;

assign cp0_rdata = ({32{cp0_addr == `CP0_STATUS_ADDR}} & cp0_status)
				 | ({32{cp0_addr == `CP0_CAUSE_ADDR}} & cp0_cause)
				 | ({32{cp0_addr == `CP0_EPC_ADDR}} & cp0_epc)
				 | ({32{cp0_addr == `CP0_BADV_ADDR}} & cp0_badvaddr)
				 | ({32{cp0_addr == `CP0_COUNT_ADDR}} & cp0_count)
				 | ({32{cp0_addr == `CP0_COMP_ADDR}} & cp0_compare)
				 | ({32{cp0_addr == `CP0_CONFIG_ADDR}} & cp0_config)
				 | ({32{cp0_addr == `CP0_ENTRYHI_ADDR}} & cp0_entryhi)
				 | ({32{cp0_addr == `CP0_ENTRYLO0_ADDR}} & cp0_entrylo0)
				 | ({32{cp0_addr == `CP0_ENTRYLO1_ADDR}} & cp0_entrylo1)
				 | ({32{cp0_addr == `CP0_INDEX_ADDR}} & cp0_index);

// CP0_STATUS+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// zero_forever [31:23]
reg [8:0]cp0_status_top;
always @(posedge clk)
begin
    if(rst)
        cp0_status_top <= 9'b0;
end

//[22: 22]
reg cp0_status_bev;
always @(posedge clk)
begin
    if (rst)
        cp0_status_bev = 1'b1;
end

//[21:16]
// zero_forever
reg [5:0]cp0_status_mid;
always @(posedge clk)
begin
    if(rst)
        cp0_status_mid <= 6'b0;
end

//[15:8] 
reg [7:0] cp0_status_im;
always @(posedge clk)
begin
    if (mtc0_we && cp0_addr == `CP0_STATUS_ADDR)
        cp0_status_im <= cp0_wdata[15:8];
end
//[7:2]
// zero_forever
reg [5:0] cp0_status_bot;
always @(posedge clk)
begin
    if(rst)
        cp0_status_bot <= 6'b0;
end
//EXL [1:1]
reg cp0_status_exl;
always @(posedge clk)
begin
    if(rst)
        cp0_status_exl <= 1'b0;
    else if(wb_ex)
        cp0_status_exl <= 1'b1;
    else if(eret_flush)
        cp0_status_exl <= 1'b0;
    else if(mtc0_we && cp0_addr == `CP0_STATUS_ADDR)
        cp0_status_exl <= cp0_wdata[1];
end
//IE [0:0]
reg cp0_status_ie;
always @(posedge clk)
begin
    if(rst)
        cp0_status_ie <= 1'b0;
    else if(mtc0_we && cp0_addr == `CP0_STATUS_ADDR)
        cp0_status_ie <= cp0_wdata[0];
end

assign cp0_status = 
{
        cp0_status_top,     //31:23
        cp0_status_bev,     //22:22
        cp0_status_mid,     //21:16
        cp0_status_im,      //15:8
        cp0_status_bot,     //7:2
        cp0_status_exl,     //1:1
        cp0_status_ie       //0:0
};

//CP0_CAUSE++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//[31:31]
reg cp0_cause_bd;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_bd <= 1'b0;
    else if(wb_ex && !cp0_status_exl)
        cp0_cause_bd <= wb_bd;
end
//[30:30]

reg cp0_cause_ti;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_ti <= 1'b0;
    /*else if(mtc0_we && cp0_addr == `CP0_CAUSE_ADDR)
        cp0_cause_ti <= 1'b0;*/
	else if(mtc0_we && cp0_addr == `CP0_COMP_ADDR)
        cp0_cause_ti <= 1'b0;
    else if(count_eq_compare)
        cp0_cause_ti <= 1'b1;
end
//[29:16]
// zero_forever
reg [13:0]cp0_cause_top;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_top <= 14'b0;
end


//[15:10]
// software can read can't write
reg [5:0]cp0_cause_ip_7_2;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_ip_7_2[4:0] <= 6'b0;
    else begin
        cp0_cause_ip_7_2[4:0] <= ext_int_in[4:0];
    end
end

always @(posedge clk)
begin
    if(rst)
        cp0_cause_ip_7_2[5] <= 1'b0;
    else begin
        cp0_cause_ip_7_2[5] <= (ext_int_in[5] | cp0_cause_ti);
    end
end

//[9:8]
//software set 
reg [1:0]cp0_cause_ip_1_0;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_ip_1_0[1:0] <= 2'b0;
    else if(mtc0_we && cp0_addr == `CP0_CAUSE_ADDR)
        cp0_cause_ip_1_0[1:0] <= cp0_wdata[9:8];
end

//[7:7]
// zero_forever
reg cp0_cause_mid;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_mid <= 1'b0;
end

//[6:2]
reg [4:0] cp0_cause_excode;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_excode <= 5'b0;
    else if(wb_ex)
        cp0_cause_excode <= wb_excode;
end

//[1:0]
// zero_forever
reg [1:0]cp0_cause_bot;
always @(posedge clk)
begin
    if(rst)
        cp0_cause_bot <= 2'b0;
end


assign cp0_cause = 
{
        cp0_cause_bd,       //31:31
        cp0_cause_ti,       //30:30
        cp0_cause_top,      //29:16
        cp0_cause_ip_7_2,   //15:10
        cp0_cause_ip_1_0,   //9:8
        cp0_cause_mid,      //7:7
        cp0_cause_excode,   //6:2
        cp0_cause_bot       //1:0
};

//CP0_EPC+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:0] cp0_epc_r;
always @(posedge clk)
begin
    if(wb_ex && !cp0_status_exl)
        cp0_epc_r <= wb_bd ? wb_pc - 32'h4 : wb_pc;
    else if(mtc0_we && cp0_addr == `CP0_EPC_ADDR)
        cp0_epc_r <= cp0_wdata;
end

assign cp0_epc = cp0_epc_r;

//CP0_BADVADDR+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:0] cp0_badvaddr_r;
always @(posedge clk)
begin
    //if(wb_ex && wb_excode == `EX_ADEL)
	
	if(wb_ex && (wb_excode == `EX_ADEL || wb_excode == `EX_ADES || ws_tlb_ex))
        cp0_badvaddr_r <= wb_badvaddr;
end

assign cp0_badvaddr = cp0_badvaddr_r;

//CP0_COUNT+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg tick;
always @(posedge clk)
begin
    if(rst)
        tick <= 1'b0;
    else
        tick <= ~tick;
end

reg [31:0] cp0_count_r;
always @(posedge clk)
begin
    if(rst)
        cp0_count_r <= 32'b0;
    else if(mtc0_we && cp0_addr == `CP0_COUNT_ADDR)
        cp0_count_r <= cp0_wdata;
    else if(tick)
        cp0_count_r <= cp0_count_r + 1'b1;
end

assign cp0_count = cp0_count_r;

//CP0_COMPARE+++++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:0]cp0_compare_r;
always @(posedge clk)
begin
    if(mtc0_we && cp0_addr == `CP0_COMP_ADDR)
        cp0_compare_r <= cp0_wdata;
end

assign cp0_compare = cp0_compare_r;

// CP0_ENTRYHI+++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:13] cp0_entryhi_vpn2;
always @(posedge clk)
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYHI_ADDR)
        cp0_entryhi_vpn2 <= cp0_wdata[31:13];
    else if(tlbr)
        cp0_entryhi_vpn2 <= r_vpn2;
    else if(ws_tlb_ex)
        cp0_entryhi_vpn2 <= wb_badvaddr[31:13];
end

reg [12:8] cp0_entryhi_zero;
always @(posedge clk) 
begin
    if(rst)
        cp0_entryhi_zero <= 5'b0;    
end

reg [7:0] cp0_entryhi_asid;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYHI_ADDR)
        cp0_entryhi_asid <= cp0_wdata[7:0];
    else if(tlbr)
        cp0_entryhi_asid <= r_asid;    
end

assign cp0_entryhi = 
{
        cp0_entryhi_vpn2,
        cp0_entryhi_zero,
        cp0_entryhi_asid
};

// CP0_ENTRYLO0+++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:26] cp0_entrylo0_zero;
always @(posedge clk) 
begin
    if(rst)
        cp0_entrylo0_zero <= 6'b0;   
end

reg [25:6] cp0_entrylo0_pfn;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO0_ADDR)
        cp0_entrylo0_pfn <= cp0_wdata[25:6];        
    else if(tlbp & s0_found)
        cp0_entrylo0_pfn <= s0_pfn;
    else if(tlbp & s1_found)
        cp0_entrylo0_pfn <= s1_pfn;
    else if(tlbr)
        cp0_entrylo0_pfn <= r_pfn0;    
end

reg [5:3] cp0_entrylo0_c;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO0_ADDR)
        cp0_entrylo0_c <= cp0_wdata[5:3];   
    else if(tlbp & s0_found)
        cp0_entrylo0_c <= s0_c;
    else if(tlbp & s1_found)
        cp0_entrylo0_c <= s1_c;
    else if(tlbr)
        cp0_entrylo0_c <= r_c0;    
end

reg cp0_entrylo0_d;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO0_ADDR)
        cp0_entrylo0_d <= cp0_wdata[2]; 
    else if(tlbp & s0_found)
        cp0_entrylo0_d <= s0_d;
    else if(tlbp & s1_found)
        cp0_entrylo0_d <= s1_d;
    else if(tlbr)
        cp0_entrylo0_d <= r_d0;    
end

reg cp0_entrylo0_v;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO0_ADDR)
        cp0_entrylo0_v <= cp0_wdata[1]; 
    else if(tlbp & s0_found)
        cp0_entrylo0_v <= s0_v;
    else if(tlbp & s1_found)
        cp0_entrylo0_v <= s1_v;
    else if(tlbr)
        cp0_entrylo0_v <= r_v0;    
end

reg cp0_entrylo0_g;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO0_ADDR)
        cp0_entrylo0_g <= cp0_wdata[0];
    else if(tlbr)
        cp0_entrylo0_g <= r_g;
end

assign cp0_entrylo0 = 
{
        cp0_entrylo0_zero,
        cp0_entrylo0_pfn,
        cp0_entrylo0_c,
        cp0_entrylo0_d,
        cp0_entrylo0_v,
        cp0_entrylo0_g        
};

// CP0_ENTRYLO1+++++++++++++++++++++++++++++++++++++++++++++++++++++
reg [31:26] cp0_entrylo1_zero;
always @(posedge clk) 
begin
    if(rst)
        cp0_entrylo1_zero <= 6'b0;   
end

reg [25:6] cp0_entrylo1_pfn;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO1_ADDR)
        cp0_entrylo1_pfn <= cp0_wdata[25:6];   
    else if(tlbp & s0_found)
        cp0_entrylo1_pfn <= s0_pfn;
    else if(tlbp & s1_found)
        cp0_entrylo1_pfn <= s1_pfn;
    else if(tlbr)
        cp0_entrylo1_pfn <= r_pfn1;     
end

reg [5:3] cp0_entrylo1_c;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO1_ADDR)
        cp0_entrylo1_c <= cp0_wdata[5:3]; 
    else if(tlbp & s0_found)
        cp0_entrylo1_c <= s0_c;
    else if(tlbp & s1_found)
        cp0_entrylo1_c <= s1_c;
    else if(tlbr)
        cp0_entrylo1_c <= r_c1;    
end

reg cp0_entrylo1_d;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO1_ADDR)
        cp0_entrylo1_d <= cp0_wdata[2];
    else if(tlbp & s0_found)
        cp0_entrylo1_d <= s0_d;
    else if(tlbp & s1_found)
        cp0_entrylo1_d <= s1_d;
    else if(tlbr)
        cp0_entrylo1_d <= r_d1;    
end

reg cp0_entrylo1_v;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO1_ADDR)
        cp0_entrylo1_v <= cp0_wdata[1]; 
    else if(tlbp & s0_found)
        cp0_entrylo1_v <= s0_v;
    else if(tlbp & s1_found)
        cp0_entrylo1_v <= s1_v;
    else if(tlbr)
        cp0_entrylo1_v <= r_v1;    
end

reg cp0_entrylo1_g;
always @(posedge clk) 
begin
    if(mtc0_we && cp0_addr == `CP0_ENTRYLO1_ADDR)
        cp0_entrylo1_g <= cp0_wdata[0];
    else if(tlbr)
        cp0_entrylo1_g <= r_g;
end

assign cp0_entrylo1 = 
{
        cp0_entrylo1_zero,
        cp0_entrylo1_pfn,
        cp0_entrylo1_c,
        cp0_entrylo1_d,
        cp0_entrylo1_v,
        cp0_entrylo1_g        
};

// CP0_INDEX+++++++++++++++++++++++++++++++++++++++++++++++++++++++
reg        cp0_index_p;
always @(posedge clk)
begin
    if(rst)
    begin
        cp0_index_p     <= 1'b0;
    end
    else if(tlbp)
        cp0_index_p <= (!(s0_found | s1_found));
end

reg [30:4] cp0_index_zero;
always @(posedge clk)
begin
    if(rst)
        cp0_index_zero  <= 27'b0;
end

reg [ 3:0] cp0_index_index;
always @(posedge clk)
begin
    if(rst)
    begin
        cp0_index_index <= 4'b0;
    end
    else if(mtc0_we && cp0_addr == `CP0_INDEX_ADDR)
        cp0_index_index <= cp0_wdata[3:0];
    else if(tlbp)
    begin
        if(s1_found)
            cp0_index_index <= s1_index;   
        else if(s0_found)
            cp0_index_index <= s0_index;
         
    end
end

assign cp0_index = 
{
        cp0_index_p,
        cp0_index_zero,
        cp0_index_index        
};

endmodule
