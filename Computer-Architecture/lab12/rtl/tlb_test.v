//the test include 2 parts : write / read & search.
module tlb_test #
(
    parameter TLBNUM=16
   ,parameter SIMULATION=1'b0
)
(
    input         resetn, 
    input         clk,

    //------gpio-------
    output [15:0] led,
    output reg [7 :0] num_csn,
    output reg [6 :0] num_a_g
);

clk_pll clk_pll(
    .clk_out1(clk_g),
    .clk_in1(clk)
);

reg       test_error;
reg       tlb_w_test_ok;
reg       tlb_r_test_ok;
reg       tlb_s_test_ok;
reg [3:0] tlb_w_cnt;
reg [3:0] tlb_r_cnt;
reg [3:0] tlb_s_cnt;

// search port 0
wire [18:0] s0_vpn2;
wire        s0_odd_page;
wire [7:0]  s0_asid;
wire        s0_found;
wire [$clog2(TLBNUM)-1:0]s0_index;
wire [19:0] s0_pfn;
wire [2:0]  s0_c;
wire        s0_d;
wire        s0_v;
// search port 1
wire [18:0] s1_vpn2;
wire        s1_odd_page;
wire [7:0]  s1_asid;
wire        s1_found;
wire [$clog2(TLBNUM)-1:0] s1_index;
wire [19:0] s1_pfn;
wire [2:0]  s1_c;
wire        s1_d;
wire        s1_v;
// write port
wire        we;
wire [$clog2(TLBNUM)-1:0] w_index;
wire [18:0] w_vpn2;
wire [7:0]  w_asid;
wire        w_g;
wire [19:0] w_pfn0;
wire [2:0]  w_c0;
wire        w_d0;
wire        w_v0;
wire [19:0] w_pfn1;
wire [2:0]  w_c1;
wire        w_d1;
wire        w_v1;
// read port
wire [$clog2(TLBNUM)-1:0] r_index;
wire [18:0] r_vpn2;
wire [7:0]  r_asid;
wire        r_g;
wire [19:0] r_pfn0;
wire [2:0]  r_c0;
wire        r_d0;
wire        r_v0;
wire [19:0] r_pfn1;
wire [2:0]  r_c1;
wire        r_d1;
wire        r_v1;

wire        r_error;
wire        s0_error;
wire        s1_error;
wire        wait_1s;

//wait_1s
reg [26:0] wait_cnt;
assign wait_1s = wait_cnt==27'd0;
always @(posedge clk_g)
begin
    if (!resetn ||  wait_1s)
    begin
        wait_cnt <= (SIMULATION == 1'b1) ? 27'd5 : 27'd30_000_000;
    end
    else
    begin
        wait_cnt <= wait_cnt - 1'b1;
    end
end
tlb #(.TLBNUM(16)) tlb_0
(
    .clk(clk_g),
    .s0_vpn2(s0_vpn2),
    .s0_odd_page(s0_odd_page),
    .s0_asid(s0_asid),
    .s0_found(s0_found),
    .s0_index(s0_index),
    .s0_pfn(s0_pfn),
    .s0_c(s0_c),
    .s0_d(s0_d),
    .s0_v(s0_v),
    .s1_vpn2(s1_vpn2),
    .s1_odd_page(s1_odd_page),
    .s1_asid(s1_asid),
    .s1_found(s1_found),
    .s1_index(s1_index),
    .s1_pfn(s1_pfn),
    .s1_c(s1_c),
    .s1_d(s1_d),
    .s1_v(s1_v),
    .we(we),
    .w_index(w_index),
    .w_vpn2(w_vpn2),
    .w_asid(w_asid),
    .w_g(w_g),
    .w_pfn0(w_pfn0),
    .w_c0(w_c0),
    .w_d0(w_d0),
    .w_v0(w_v0),
    .w_pfn1(w_pfn1),
    .w_c1(w_c1),
    .w_d1(w_d1),
    .w_v1(w_v1),
    .r_index(r_index),
    .r_vpn2(r_vpn2),
    .r_asid(r_asid),
    .r_g(r_g),
    .r_pfn0(r_pfn0),
    .r_c0(r_c0),
    .r_d0(r_d0),
    .r_v0(r_v0),
    .r_pfn1(r_pfn1),
    .r_c1(r_c1),
    .r_d1(r_d1),
    .r_v1(r_v1)
);

// input data
// write \ read

//  index  |  vpn2  |  asid  |  g  |  pfn0  |  c0 d0 v0  |  pfn1  |  c1 d1 v1  |
//   0     |  0x000 |   0x0  |  0  |  0x111 |     3,1,1  |  0x022 |  3,1,1  | 
//   1     |  0x111 |   0x1  |  1  |  0x222 |     3,1,1  |  0x033 |  3,1,1  | 
//   2     |  0x222 |   0x2  |  0  |  0x333 |     3,1,1  |  0x044 |  3,1,1  | 
//   3     |  0x333 |   0x3  |  1  |  0x444 |     3,1,1  |  0x055 |  3,1,1  | 
//   4     |  0x444 |   0x4  |  0  |  0x555 |     3,1,1  |  0x066 |  3,1,1  | 
//   5     |  0x444 |   0x5  |  0  |  0x666 |     3,1,1  |  0x077 |  3,1,1  | 
//   6     |  0x666 |   0x6  |  0  |  0x777 |     3,1,1  |  0x088 |  3,1,1  | 
//   7     |  0x666 |   0x7  |  0  |  0x888 |     3,1,1  |  0x099 |  3,1,1  | 
//   8     |  0x888 |   0x8  |  0  |  0x999 |     3,1,1  |  0x0aa |  3,1,1  | 
//   9     |  0x999 |   0x9  |  0  |  0xaaa |     3,1,1  |  0x0bb |  3,1,1  | 
//   10    |  0xaaa |   0xa  |  0  |  0xbbb |     3,1,1  |  0x0cc |  3,1,1  | 
//   11    |  0xbbb |   0xb  |  0  |  0xccc |     3,1,1  |  0x0dd |  3,1,1  | 
//   12    |  0xccc |   0xc  |  0  |  0xddd |     3,1,1  |  0x0ee |  3,1,1  | 
//   13    |  0xddd |   0xd  |  0  |  0xeee |     3,1,1  |  0x0ff |  3,1,1  | 
//   14    |  0xeee |   0xe  |  0  |  0xfff |     3,1,1  |  0x000 |  3,1,1  | 
//   15    |  0xfff |   0xf  |  0  |  0x000 |     3,1,1  |  0x011 |  3,1,1  | 

wire [18:0] tlb_vpn2 [15:0];
wire [7 :0] tlb_asid [15:0];
wire        tlb_g    [15:0];
wire [19:0] tlb_pfn0 [15:0];
wire [2 :0] tlb_c0   [15:0];
wire        tlb_d0   [15:0];
wire        tlb_v0   [15:0];
wire [19:0] tlb_pfn1 [15:0];
wire [2 :0] tlb_c1   [15:0];
wire        tlb_d1   [15:0];
wire        tlb_v1   [15:0];

assign tlb_vpn2[ 0] = 19'h000;
assign tlb_vpn2[ 1] = 19'h111;
assign tlb_vpn2[ 2] = 19'h222;
assign tlb_vpn2[ 3] = 19'h333;
assign tlb_vpn2[ 4] = 19'h444;
assign tlb_vpn2[ 5] = 19'h444;
assign tlb_vpn2[ 6] = 19'h666;
assign tlb_vpn2[ 7] = 19'h666;
assign tlb_vpn2[ 8] = 19'h888;
assign tlb_vpn2[ 9] = 19'h999;
assign tlb_vpn2[10] = 19'haaa;
assign tlb_vpn2[11] = 19'hbbb;
assign tlb_vpn2[12] = 19'hccc;
assign tlb_vpn2[13] = 19'hddd;
assign tlb_vpn2[14] = 19'heee;
assign tlb_vpn2[15] = 19'hfff;

assign tlb_asid[ 0] = 8'h0;
assign tlb_asid[ 1] = 8'h1;
assign tlb_asid[ 2] = 8'h2;
assign tlb_asid[ 3] = 8'h3;
assign tlb_asid[ 4] = 8'h4;
assign tlb_asid[ 5] = 8'h5;
assign tlb_asid[ 6] = 8'h6;
assign tlb_asid[ 7] = 8'h7;
assign tlb_asid[ 8] = 8'h8;
assign tlb_asid[ 9] = 8'h9;
assign tlb_asid[10] = 8'ha;
assign tlb_asid[11] = 8'hb;
assign tlb_asid[12] = 8'hc;
assign tlb_asid[13] = 8'hd;
assign tlb_asid[14] = 8'he;
assign tlb_asid[15] = 8'hf;

assign tlb_g[ 0] = 1'h0;
assign tlb_g[ 1] = 1'h1;
assign tlb_g[ 2] = 1'h0;
assign tlb_g[ 3] = 1'h1;
assign tlb_g[ 4] = 1'h0;
assign tlb_g[ 5] = 1'h0;
assign tlb_g[ 6] = 1'h0;
assign tlb_g[ 7] = 1'h0;
assign tlb_g[ 8] = 1'h0;
assign tlb_g[ 9] = 1'h0;
assign tlb_g[10] = 1'h0;
assign tlb_g[11] = 1'h0;
assign tlb_g[12] = 1'h0;
assign tlb_g[13] = 1'h0;
assign tlb_g[14] = 1'h0;
assign tlb_g[15] = 1'h0;

assign tlb_pfn0[ 0] = 20'h111;
assign tlb_pfn0[ 1] = 20'h222;
assign tlb_pfn0[ 2] = 20'h333;
assign tlb_pfn0[ 3] = 20'h444;
assign tlb_pfn0[ 4] = 20'h555;
assign tlb_pfn0[ 5] = 20'h666;
assign tlb_pfn0[ 6] = 20'h777;
assign tlb_pfn0[ 7] = 20'h888;
assign tlb_pfn0[ 8] = 20'h999;
assign tlb_pfn0[ 9] = 20'haaa;
assign tlb_pfn0[10] = 20'hbbb;
assign tlb_pfn0[11] = 20'hccc;
assign tlb_pfn0[12] = 20'hddd;
assign tlb_pfn0[13] = 20'heee;
assign tlb_pfn0[14] = 20'hfff;
assign tlb_pfn0[15] = 20'h000;

assign tlb_c0[ 0] = 3'h3;
assign tlb_c0[ 1] = 3'h3;
assign tlb_c0[ 2] = 3'h3;
assign tlb_c0[ 3] = 3'h3;
assign tlb_c0[ 4] = 3'h3;
assign tlb_c0[ 5] = 3'h3;
assign tlb_c0[ 6] = 3'h3;
assign tlb_c0[ 7] = 3'h3;
assign tlb_c0[ 8] = 3'h3;
assign tlb_c0[ 9] = 3'h3;
assign tlb_c0[10] = 3'h3;
assign tlb_c0[11] = 3'h3;
assign tlb_c0[12] = 3'h3;
assign tlb_c0[13] = 3'h3;
assign tlb_c0[14] = 3'h3;
assign tlb_c0[15] = 3'h3;

assign tlb_d0[ 0] = 1'h1;
assign tlb_d0[ 1] = 1'h1;
assign tlb_d0[ 2] = 1'h1;
assign tlb_d0[ 3] = 1'h1;
assign tlb_d0[ 4] = 1'h1;
assign tlb_d0[ 5] = 1'h1;
assign tlb_d0[ 6] = 1'h1;
assign tlb_d0[ 7] = 1'h1;
assign tlb_d0[ 8] = 1'h1;
assign tlb_d0[ 9] = 1'h1;
assign tlb_d0[10] = 1'h1;
assign tlb_d0[11] = 1'h1;
assign tlb_d0[12] = 1'h1;
assign tlb_d0[13] = 1'h1;
assign tlb_d0[14] = 1'h1;
assign tlb_d0[15] = 1'h1;

assign tlb_v0[ 0] = 1'h1;
assign tlb_v0[ 1] = 1'h1;
assign tlb_v0[ 2] = 1'h1;
assign tlb_v0[ 3] = 1'h1;
assign tlb_v0[ 4] = 1'h1;
assign tlb_v0[ 5] = 1'h1;
assign tlb_v0[ 6] = 1'h1;
assign tlb_v0[ 7] = 1'h1;
assign tlb_v0[ 8] = 1'h1;
assign tlb_v0[ 9] = 1'h1;
assign tlb_v0[10] = 1'h1;
assign tlb_v0[11] = 1'h1;
assign tlb_v0[12] = 1'h1;
assign tlb_v0[13] = 1'h1;
assign tlb_v0[14] = 1'h1;
assign tlb_v0[15] = 1'h1;

assign tlb_pfn1[ 0] = 20'h022;
assign tlb_pfn1[ 1] = 20'h033;
assign tlb_pfn1[ 2] = 20'h044;
assign tlb_pfn1[ 3] = 20'h055;
assign tlb_pfn1[ 4] = 20'h066;
assign tlb_pfn1[ 5] = 20'h077;
assign tlb_pfn1[ 6] = 20'h088;
assign tlb_pfn1[ 7] = 20'h099;
assign tlb_pfn1[ 8] = 20'h0aa;
assign tlb_pfn1[ 9] = 20'h0bb;
assign tlb_pfn1[10] = 20'h0cc;
assign tlb_pfn1[11] = 20'h0dd;
assign tlb_pfn1[12] = 20'h0ee;
assign tlb_pfn1[13] = 20'h0ff;
assign tlb_pfn1[14] = 20'h000;
assign tlb_pfn1[15] = 20'h011;

assign tlb_c1[ 0] = 3'h3;
assign tlb_c1[ 1] = 3'h3;
assign tlb_c1[ 2] = 3'h3;
assign tlb_c1[ 3] = 3'h3;
assign tlb_c1[ 4] = 3'h3;
assign tlb_c1[ 5] = 3'h3;
assign tlb_c1[ 6] = 3'h3;
assign tlb_c1[ 7] = 3'h3;
assign tlb_c1[ 8] = 3'h3;
assign tlb_c1[ 9] = 3'h3;
assign tlb_c1[10] = 3'h3;
assign tlb_c1[11] = 3'h3;
assign tlb_c1[12] = 3'h3;
assign tlb_c1[13] = 3'h3;
assign tlb_c1[14] = 3'h3;
assign tlb_c1[15] = 3'h3;

assign tlb_d1[ 0] = 1'h1;
assign tlb_d1[ 1] = 1'h1;
assign tlb_d1[ 2] = 1'h1;
assign tlb_d1[ 3] = 1'h1;
assign tlb_d1[ 4] = 1'h1;
assign tlb_d1[ 5] = 1'h1;
assign tlb_d1[ 6] = 1'h1;
assign tlb_d1[ 7] = 1'h1;
assign tlb_d1[ 8] = 1'h1;
assign tlb_d1[ 9] = 1'h1;
assign tlb_d1[10] = 1'h1;
assign tlb_d1[11] = 1'h1;
assign tlb_d1[12] = 1'h1;
assign tlb_d1[13] = 1'h1;
assign tlb_d1[14] = 1'h1;
assign tlb_d1[15] = 1'h1;

assign tlb_v1[ 0] = 1'h1;
assign tlb_v1[ 1] = 1'h1;
assign tlb_v1[ 2] = 1'h1;
assign tlb_v1[ 3] = 1'h1;
assign tlb_v1[ 4] = 1'h1;
assign tlb_v1[ 5] = 1'h1;
assign tlb_v1[ 6] = 1'h1;
assign tlb_v1[ 7] = 1'h1;
assign tlb_v1[ 8] = 1'h1;
assign tlb_v1[ 9] = 1'h1;
assign tlb_v1[10] = 1'h1;
assign tlb_v1[11] = 1'h1;
assign tlb_v1[12] = 1'h1;
assign tlb_v1[13] = 1'h1;
assign tlb_v1[14] = 1'h1;
assign tlb_v1[15] = 1'h1;

//search
// s_vpn2  |  s_odd_page  | s_asid  |  s_found  |  s_index  |  s_pfn  |  s_c s_d s_v  |
//  0x000  |         0x0  |    0x0  |        1  |        0  |  0x111  |  3,1,1  |
//  0x000  |         0x1  |    0x0  |        1  |        0  |  0x022  |  3,1,1  |
//  0x000  |         0x1  |    0x1  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x111  |         0x1  |    0x0  |        1  |        1  |  0x033  |  3,1,1  |
//  0x111  |         0x0  |    0x1  |        1  |        1  |  0x222  |  3,1,1  |
//  0x222  |         0x0  |    0x0  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x222  |         0x0  |    0x2  |        1  |        2  |  0x333  |  3,1,1  |
//  0x333  |         0x0  |    0x3  |        1  |        3  |  0x444  |  3,1,1  |
//  0x333  |         0x1  |    0x4  |        1  |        3  |  0x055  |  3,1,1  |
//  0x444  |         0x0  |    0x4  |        1  |        4  |  0x555  |  3,1,1  |
//  0x444  |         0x0  |    0x5  |        1  |        5  |  0x666  |  3,1,1  |
//  0x555  |         0x1  |    0x5  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x666  |         0x0  |    0x6  |        1  |        6  |  0x777  |  3,1,1  |
//  0x666  |         0x1  |    0x7  |        1  |        7  |  0x099  |  3,1,1  |
//  0x666  |         0x1  |    0x8  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x777  |         0x0  |    0x7  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x888  |         0x0  |    0x8  |        1  |        8  |  0x999  |  3,1,1  |
//  0x999  |         0x1  |    0x9  |        1  |        9  |  0x0bb  |  3,1,1  |
//  0xaaa  |         0x0  |    0xa  |        1  |       10  |  0xbbb  |  3,1,1  |
//  0xbbb  |         0x1  |    0xb  |        1  |       11  |  0x0dd  |  3,1,1  |
//  0xccc  |         0x0  |    0xc  |        1  |       12  |  0xddd  |  3,1,1  |
//  0xddd  |         0x1  |    0xd  |        1  |       13  |  0x0ff  |  3,1,1  |
//  0xeee  |         0x0  |    0xe  |        1  |       14  |  0xfff  |  3,1,1  |
//  0xfff  |         0x1  |    0xf  |        1  |       15  |  0x011  |  3,1,1  |
//  0xabc  |         0x0  |    0xf  |        0  |        x  |  0xxxx  |  x,x,x  |
//  0x123  |         0x1  |    0x3  |        0  |        x  |  0xxxx  |  x,x,x  |

wire [18:0] s_test_vpn2 [25:0];
wire        s_test_odd_page [25:0];
wire [ 7:0] s_test_asid [25:0];
wire        s_test_found [25:0];
wire [ 3:0] s_test_index [25:0];
wire [19:0] s_test_pfn [25:0];
wire [ 2:0] s_test_c [25:0];
wire        s_test_d [25:0];
wire        s_test_v [25:0];

assign s_test_vpn2[ 0] = 19'h000;
assign s_test_vpn2[ 1] = 19'h000;
assign s_test_vpn2[ 2] = 19'h000;
assign s_test_vpn2[ 3] = 19'h111;
assign s_test_vpn2[ 4] = 19'h111;
assign s_test_vpn2[ 5] = 19'h222;
assign s_test_vpn2[ 6] = 19'h222;
assign s_test_vpn2[ 7] = 19'h333;
assign s_test_vpn2[ 8] = 19'h333;
assign s_test_vpn2[ 9] = 19'h444;
assign s_test_vpn2[10] = 19'h444;
assign s_test_vpn2[11] = 19'h555;
assign s_test_vpn2[12] = 19'h666;
assign s_test_vpn2[13] = 19'h666;
assign s_test_vpn2[14] = 19'h666;
assign s_test_vpn2[15] = 19'h777;
assign s_test_vpn2[16] = 19'h888;
assign s_test_vpn2[17] = 19'h999;
assign s_test_vpn2[18] = 19'haaa;
assign s_test_vpn2[19] = 19'hbbb;
assign s_test_vpn2[20] = 19'hccc;
assign s_test_vpn2[21] = 19'hddd;
assign s_test_vpn2[22] = 19'heee;
assign s_test_vpn2[23] = 19'hfff;
assign s_test_vpn2[24] = 19'habc;
assign s_test_vpn2[25] = 19'h123;

assign s_test_odd_page[ 0] = 1'h0;
assign s_test_odd_page[ 1] = 1'h1;
assign s_test_odd_page[ 2] = 1'h1;
assign s_test_odd_page[ 3] = 1'h1;
assign s_test_odd_page[ 4] = 1'h0;
assign s_test_odd_page[ 5] = 1'h0;
assign s_test_odd_page[ 6] = 1'h0;
assign s_test_odd_page[ 7] = 1'h0;
assign s_test_odd_page[ 8] = 1'h1;
assign s_test_odd_page[ 9] = 1'h0;
assign s_test_odd_page[10] = 1'h0;
assign s_test_odd_page[11] = 1'h1;
assign s_test_odd_page[12] = 1'h0;
assign s_test_odd_page[13] = 1'h1;
assign s_test_odd_page[14] = 1'h1;
assign s_test_odd_page[15] = 1'h0;
assign s_test_odd_page[16] = 1'h0;
assign s_test_odd_page[17] = 1'h1;
assign s_test_odd_page[18] = 1'h0;
assign s_test_odd_page[19] = 1'h1;
assign s_test_odd_page[20] = 1'h0;
assign s_test_odd_page[21] = 1'h1;
assign s_test_odd_page[22] = 1'h0;
assign s_test_odd_page[23] = 1'h1;
assign s_test_odd_page[24] = 1'h0;
assign s_test_odd_page[25] = 1'h1;

assign s_test_asid[ 0] = 8'h0;
assign s_test_asid[ 1] = 8'h0;
assign s_test_asid[ 2] = 8'h1;
assign s_test_asid[ 3] = 8'h0;
assign s_test_asid[ 4] = 8'h1;
assign s_test_asid[ 5] = 8'h0;
assign s_test_asid[ 6] = 8'h2;
assign s_test_asid[ 7] = 8'h3;
assign s_test_asid[ 8] = 8'h4;
assign s_test_asid[ 9] = 8'h4;
assign s_test_asid[10] = 8'h5;
assign s_test_asid[11] = 8'h5;
assign s_test_asid[12] = 8'h6;
assign s_test_asid[13] = 8'h7;
assign s_test_asid[14] = 8'h8;
assign s_test_asid[15] = 8'h7;
assign s_test_asid[16] = 8'h8;
assign s_test_asid[17] = 8'h9;
assign s_test_asid[18] = 8'ha;
assign s_test_asid[19] = 8'hb;
assign s_test_asid[20] = 8'hc;
assign s_test_asid[21] = 8'hd;
assign s_test_asid[22] = 8'he;
assign s_test_asid[23] = 8'hf;
assign s_test_asid[24] = 8'hf;
assign s_test_asid[25] = 8'h3;

assign s_test_found[ 0] = 1'h1;
assign s_test_found[ 1] = 1'h1;
assign s_test_found[ 2] = 1'h0;
assign s_test_found[ 3] = 1'h1;
assign s_test_found[ 4] = 1'h1;
assign s_test_found[ 5] = 1'h0;
assign s_test_found[ 6] = 1'h1;
assign s_test_found[ 7] = 1'h1;
assign s_test_found[ 8] = 1'h1;
assign s_test_found[ 9] = 1'h1;
assign s_test_found[10] = 1'h1;
assign s_test_found[11] = 1'h0;
assign s_test_found[12] = 1'h1;
assign s_test_found[13] = 1'h1;
assign s_test_found[14] = 1'h0;
assign s_test_found[15] = 1'h0;
assign s_test_found[16] = 1'h1;
assign s_test_found[17] = 1'h1;
assign s_test_found[18] = 1'h1;
assign s_test_found[19] = 1'h1;
assign s_test_found[20] = 1'h1;
assign s_test_found[21] = 1'h1;
assign s_test_found[22] = 1'h1;
assign s_test_found[23] = 1'h1;
assign s_test_found[24] = 1'h0;
assign s_test_found[25] = 1'h0;

assign s_test_index[ 0] = 4'h0;
assign s_test_index[ 1] = 4'h0;
assign s_test_index[ 2] = 4'hx;
assign s_test_index[ 3] = 4'h1;
assign s_test_index[ 4] = 4'h1;
assign s_test_index[ 5] = 4'hx;
assign s_test_index[ 6] = 4'h2;
assign s_test_index[ 7] = 4'h3;
assign s_test_index[ 8] = 4'h3;
assign s_test_index[ 9] = 4'h4;
assign s_test_index[10] = 4'h5;
assign s_test_index[11] = 4'hx;
assign s_test_index[12] = 4'h6;
assign s_test_index[13] = 4'h7;
assign s_test_index[14] = 4'hx;
assign s_test_index[15] = 4'hx;
assign s_test_index[16] = 4'h8;
assign s_test_index[17] = 4'h9;
assign s_test_index[18] = 4'ha;
assign s_test_index[19] = 4'hb;
assign s_test_index[20] = 4'hc;
assign s_test_index[21] = 4'hd;
assign s_test_index[22] = 4'he;
assign s_test_index[23] = 4'hf;
assign s_test_index[24] = 4'hx;
assign s_test_index[25] = 4'hx;

assign s_test_pfn[ 0] = 20'h111;
assign s_test_pfn[ 1] = 20'h022;
assign s_test_pfn[ 2] = 20'hxxx;
assign s_test_pfn[ 3] = 20'h033;
assign s_test_pfn[ 4] = 20'h222;
assign s_test_pfn[ 5] = 20'hxxx;
assign s_test_pfn[ 6] = 20'h333;
assign s_test_pfn[ 7] = 20'h444;
assign s_test_pfn[ 8] = 20'h055;
assign s_test_pfn[ 9] = 20'h555;
assign s_test_pfn[10] = 20'h666;
assign s_test_pfn[11] = 20'hxxx;
assign s_test_pfn[12] = 20'h777;
assign s_test_pfn[13] = 20'h099;
assign s_test_pfn[14] = 20'hxxx;
assign s_test_pfn[15] = 20'hxxx;
assign s_test_pfn[16] = 20'h999;
assign s_test_pfn[17] = 20'h0bb;
assign s_test_pfn[18] = 20'hbbb;
assign s_test_pfn[19] = 20'h0dd;
assign s_test_pfn[20] = 20'hddd;
assign s_test_pfn[21] = 20'h0ff;
assign s_test_pfn[22] = 20'hfff;
assign s_test_pfn[23] = 20'h011;
assign s_test_pfn[24] = 20'hxxx;
assign s_test_pfn[25] = 20'hxxx;

assign s_test_c[ 0] = 3'h3;
assign s_test_c[ 1] = 3'h3;
assign s_test_c[ 2] = 3'hx;
assign s_test_c[ 3] = 3'h3;
assign s_test_c[ 4] = 3'h3;
assign s_test_c[ 5] = 3'hx;
assign s_test_c[ 6] = 3'h3;
assign s_test_c[ 7] = 3'h3;
assign s_test_c[ 8] = 3'h3;
assign s_test_c[ 9] = 3'h3;
assign s_test_c[10] = 3'h3;
assign s_test_c[11] = 3'hx;
assign s_test_c[12] = 3'h3;
assign s_test_c[13] = 3'h3;
assign s_test_c[14] = 3'hx;
assign s_test_c[15] = 3'hx;
assign s_test_c[16] = 3'h3;
assign s_test_c[17] = 3'h3;
assign s_test_c[18] = 3'h3;
assign s_test_c[19] = 3'h3;
assign s_test_c[20] = 3'h3;
assign s_test_c[21] = 3'h3;
assign s_test_c[22] = 3'h3;
assign s_test_c[23] = 3'h3;
assign s_test_c[24] = 3'hx;
assign s_test_c[25] = 3'hx;

assign s_test_d[ 0] = 3'h1;
assign s_test_d[ 1] = 3'h1;
assign s_test_d[ 2] = 3'hx;
assign s_test_d[ 3] = 3'h1;
assign s_test_d[ 4] = 3'h1;
assign s_test_d[ 5] = 3'hx;
assign s_test_d[ 6] = 3'h1;
assign s_test_d[ 7] = 3'h1;
assign s_test_d[ 8] = 3'h1;
assign s_test_d[ 9] = 3'h1;
assign s_test_d[10] = 3'h1;
assign s_test_d[11] = 3'hx;
assign s_test_d[12] = 3'h1;
assign s_test_d[13] = 3'h1;
assign s_test_d[14] = 3'hx;
assign s_test_d[15] = 3'hx;
assign s_test_d[16] = 3'h1;
assign s_test_d[17] = 3'h1;
assign s_test_d[18] = 3'h1;
assign s_test_d[19] = 3'h1;
assign s_test_d[20] = 3'h1;
assign s_test_d[21] = 3'h1;
assign s_test_d[22] = 3'h1;
assign s_test_d[23] = 3'h1;
assign s_test_d[24] = 3'hx;
assign s_test_d[25] = 3'hx;

assign s_test_v[ 0] = 3'h1;
assign s_test_v[ 1] = 3'h1;
assign s_test_v[ 2] = 3'hx;
assign s_test_v[ 3] = 3'h1;
assign s_test_v[ 4] = 3'h1;
assign s_test_v[ 5] = 3'hx;
assign s_test_v[ 6] = 3'h1;
assign s_test_v[ 7] = 3'h1;
assign s_test_v[ 8] = 3'h1;
assign s_test_v[ 9] = 3'h1;
assign s_test_v[10] = 3'h1;
assign s_test_v[11] = 3'hx;
assign s_test_v[12] = 3'h1;
assign s_test_v[13] = 3'h1;
assign s_test_v[14] = 3'hx;
assign s_test_v[15] = 3'hx;
assign s_test_v[16] = 3'h1;
assign s_test_v[17] = 3'h1;
assign s_test_v[18] = 3'h1;
assign s_test_v[19] = 3'h1;
assign s_test_v[20] = 3'h1;
assign s_test_v[21] = 3'h1;
assign s_test_v[22] = 3'h1;
assign s_test_v[23] = 3'h1;
assign s_test_v[24] = 3'hx;
assign s_test_v[25] = 3'hx;
// write
always @(posedge clk_g) begin
    if(~resetn) begin
        tlb_w_test_ok <= 1'b0;
        tlb_w_cnt <= 4'b0;
    end
    else if(tlb_w_cnt==4'hf) begin
        tlb_w_test_ok <= 1'b1;
    end
    else if(~tlb_w_test_ok && wait_1s) begin
        tlb_w_cnt <= tlb_w_cnt + 1;
    end
end

// read
always @(posedge clk_g) begin
    if(~resetn) begin
        tlb_r_test_ok <= 1'b0;
        tlb_r_cnt <= 4'b0;
    end
    else if(tlb_r_cnt==4'hf && ~r_error) begin
        tlb_r_test_ok <= 1'b1;
    end
    else if(tlb_w_test_ok && ~tlb_r_test_ok && ~test_error && ~r_error && wait_1s) begin
        tlb_r_cnt <= tlb_r_cnt + 1;
    end
end

// search
always @(posedge clk_g) begin
    if(~resetn) begin
        tlb_s_test_ok <= 1'b0;
        tlb_s_cnt <= 4'b0;
    end
    else if(tlb_s_cnt==4'hc && ~s0_error && ~s1_error) begin
        tlb_s_test_ok <= 1'b1;
    end
    else if(tlb_w_test_ok && ~tlb_s_test_ok && ~test_error && ~s0_error && ~s1_error && wait_1s) begin
        tlb_s_cnt <= tlb_s_cnt + 1;
    end
end

assign we = ~tlb_w_test_ok;
assign w_index = tlb_w_cnt;
assign w_vpn2 = tlb_vpn2[tlb_w_cnt];
assign w_asid = tlb_asid[tlb_w_cnt];
assign w_g    = tlb_g   [tlb_w_cnt];
assign w_pfn0 = tlb_pfn0[tlb_w_cnt];
assign w_c0   = tlb_c0  [tlb_w_cnt];
assign w_d0   = tlb_d0  [tlb_w_cnt];
assign w_v0   = tlb_v0  [tlb_w_cnt];
assign w_pfn1 = tlb_pfn1[tlb_w_cnt];
assign w_c1   = tlb_c1  [tlb_w_cnt];
assign w_d1   = tlb_d1  [tlb_w_cnt];
assign w_v1   = tlb_v1  [tlb_w_cnt];

assign r_index = tlb_r_cnt;
assign r_error = (r_vpn2 != tlb_vpn2[tlb_r_cnt])
               | (r_asid != tlb_asid[tlb_r_cnt])
               | (r_g    != tlb_g   [tlb_r_cnt])
               | (r_pfn0 != tlb_pfn0[tlb_r_cnt])
               | (r_c0   != tlb_c0  [tlb_r_cnt])
               | (r_d0   != tlb_d0  [tlb_r_cnt])
               | (r_v0   != tlb_v0  [tlb_r_cnt])
               | (r_pfn1 != tlb_pfn1[tlb_r_cnt])
               | (r_c1   != tlb_c1  [tlb_r_cnt])
               | (r_d1   != tlb_d1  [tlb_r_cnt])
               | (r_v1   != tlb_v1  [tlb_r_cnt]);

wire [4:0] s0_test_id,s1_test_id;
assign s0_test_id = {tlb_s_cnt,1'b0};
assign s1_test_id = {tlb_s_cnt,1'b1};

assign s0_vpn2     = s_test_vpn2[s0_test_id];
assign s0_odd_page = s_test_odd_page[s0_test_id];
assign s0_asid     = s_test_asid[s0_test_id];
assign s1_vpn2     = s_test_vpn2[s1_test_id];
assign s1_odd_page = s_test_odd_page[s1_test_id];
assign s1_asid     = s_test_asid[s1_test_id];

assign s0_error = (s_test_found[s0_test_id] ^ s0_found) || (s_test_found[s0_test_id] &&
                ( ~s0_found |
               | (s0_pfn != s_test_pfn[s0_test_id])
               | (s0_c   != s_test_c  [s0_test_id])
               | (s0_d   != s_test_d  [s0_test_id])
               | (s0_v   != s_test_v  [s0_test_id])));
                   
assign s1_error = (s_test_found[s1_test_id] ^ s1_found) || (s_test_found[s1_test_id] &&
                ( ~s1_found |
               | (s1_pfn != s_test_pfn[s1_test_id])
               | (s1_c   != s_test_c  [s1_test_id])
               | (s1_d   != s_test_d  [s1_test_id])
               | (s1_v   != s_test_v  [s1_test_id])));
                   
always @(posedge clk_g) begin
    if(~resetn) begin
        test_error <= 1'b0;
    end
    else if(tlb_w_test_ok && ~tlb_r_test_ok && r_error) begin
        test_error <= 1'b1;
    end
    else if(tlb_w_test_ok && ~tlb_s_test_ok && (s0_error || s1_error)) begin
        test_error <= 1'b1;
    end
end

reg [19:0] count;
always @(posedge clk_g)
begin
    if(!resetn)
    begin
        count <= 20'd0;
    end
    else
    begin
        count <= count + 1'b1;
    end
end
//scan data
reg [3:0] scan_data;
always @ ( posedge clk_g )  
begin
    if ( !resetn )
    begin
        scan_data <= 32'd0;  
        num_csn   <= 8'b1111_1111;
    end
    else
    begin
        case(count[19:17])
            3'b000 : scan_data <= {3'b0,s1_test_id[4]};
            3'b001 : scan_data <= s1_test_id[3:0];
            3'b010 : scan_data <= {3'b0,s0_test_id[4]};
            3'b011 : scan_data <= s0_test_id[3:0];
            3'b100 : scan_data <= 4'b0;
            3'b101 : scan_data <= tlb_r_cnt;
            3'b110 : scan_data <= 4'b0;
            3'b111 : scan_data <= tlb_w_cnt;
        endcase

        case(count[19:17])
            3'b000 : num_csn <= 8'b0111_1111;
            3'b001 : num_csn <= 8'b1011_1111;
            3'b010 : num_csn <= 8'b1101_1111;
            3'b011 : num_csn <= 8'b1110_1111;
            3'b100 : num_csn <= 8'b1111_0111;
            3'b101 : num_csn <= 8'b1111_1011;
            3'b110 : num_csn <= 8'b1111_1101;
            3'b111 : num_csn <= 8'b1111_1110;
        endcase
    end
end

always @(posedge clk_g)
begin
    if ( !resetn )
    begin
        num_a_g <= 7'b0000000;
    end
    else
    begin
        case ( scan_data )
            4'd0 : num_a_g <= 7'b1111110;   //0
            4'd1 : num_a_g <= 7'b0110000;   //1
            4'd2 : num_a_g <= 7'b1101101;   //2
            4'd3 : num_a_g <= 7'b1111001;   //3
            4'd4 : num_a_g <= 7'b0110011;   //4
            4'd5 : num_a_g <= 7'b1011011;   //5
            4'd6 : num_a_g <= 7'b1011111;   //6
            4'd7 : num_a_g <= 7'b1110000;   //7
            4'd8 : num_a_g <= 7'b1111111;   //8
            4'd9 : num_a_g <= 7'b1111011;   //9
            4'd10: num_a_g <= 7'b1110111;   //a
            4'd11: num_a_g <= 7'b0011111;   //b
            4'd12: num_a_g <= 7'b1001110;   //c
            4'd13: num_a_g <= 7'b0111101;   //d
            4'd14: num_a_g <= 7'b1001111;   //e
            4'd15: num_a_g <= 7'b1000111;   //f
        endcase
    end
end

assign led = {~test_error,12'hfff,~tlb_w_test_ok,~tlb_r_test_ok,~tlb_s_test_ok};

endmodule
