`include "mycpu.h"

module wb_stage
#(
    parameter TLBNUM = 16
)
(
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
    output        ws_ex_out,
    
    // lab13
    output     ws_write_entryhi,
    output     ws_tlbwir_out,
    output     flush_pipe
);

// lab13 append
wire ws_tlbp;
wire ws_tlbr;
wire ws_tlbwi;

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

//lab13
wire ws_tlbwir_ex;

assign {
        ws_tlbwir_ex,      //124:124
        ws_tlbp,          //123:123
        ws_tlbr,          //122:122
        ws_tlbwi,         //121:121
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

assign flush_pipe = ws_tlbwir_ex & ws_valid;
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
assign rf_we = ~ms_to_ws_ex & ~ws_tlbwir_ex & ws_gr_we & ws_valid;
//assign rf_we = (ms_to_ws_bus[115 : 115]) ? 1'b0 : (ws_gr_we && ws_valid);
assign rf_waddr = ws_dest;
//assign rf_wdata = ws_final_result;
assign rf_wdata = ws_mfc0 ? cp0_rdata : ws_final_result;
// debug info generate
assign debug_wb_pc       = ws_pc;
assign debug_wb_rf_wen   = {4{rf_we}};
assign debug_wb_rf_wnum  = ws_dest;
assign debug_wb_rf_wdata = rf_wdata;

// lab13 append
wire [              31:0]cp0_entryhi;
wire [              31:0]cp0_entrylo0;
wire [              31:0]cp0_entrylo1;
wire [              31:0]cp0_index;

// lab13
assign ws_write_entryhi = ws_valid && ws_inst_mtc0 &&  ({ws_cp0_rd,ws_cp0_sel} == `CP0_ENTRYHI_ADDR);

assign ws_tlbwir_out = ws_tlbwir_ex;

// OUTPUT
// search port 0
wire [              18:0] s0_vpn2;     
wire                      s0_odd_page;     
wire [               7:0] s0_asid;     
wire                      s0_found;     
wire [$clog2(TLBNUM)-1:0] s0_index;   
wire [              19:0] s0_pfn;     
wire [               2:0] s0_c;     
wire                      s0_d;     
wire                      s0_v;

// INPUT
assign s0_vpn2 = cp0_entryhi[31:13];
assign s0_odd_page = cp0_entryhi[12:12];
assign s0_asid = cp0_entryhi[7:0];

// OUTPUT
// search port 1
wire [              18:0] s1_vpn2;     
wire                      s1_odd_page;     
wire [               7:0] s1_asid;     
wire                      s1_found;     
wire [$clog2(TLBNUM)-1:0] s1_index;   
wire [              19:0] s1_pfn;     
wire [               2:0] s1_c;     
wire                      s1_d;     
wire                      s1_v;
// INPUT
assign s1_vpn2 = cp0_entryhi[31:13];
assign s1_odd_page = cp0_entryhi[12:12];
assign s1_asid = cp0_entryhi[7:0];


// write port
wire                       we;     
//w(rite) e(nable)     
wire  [$clog2(TLBNUM)-1:0] w_index;     
wire  [              18:0] w_vpn2;     
wire  [               7:0] w_asid;     
wire                       w_g;     
wire  [              19:0] w_pfn0;     
wire  [               2:0] w_c0;     
wire                       w_d0;
wire                       w_v0;     
wire  [              19:0] w_pfn1;     
wire  [               2:0] w_c1;     
wire                       w_d1;     
wire                       w_v1;

assign we = ws_tlbwi;
// ENTRYHI
assign w_index = cp0_index[$clog2(TLBNUM)-1:0];
assign w_vpn2 = cp0_entryhi[31:13];
assign w_asid = cp0_entryhi[7:0];
assign w_g    = cp0_entrylo0[0] 
              & cp0_entrylo1[0];
// ENTRYLO0
assign w_pfn0 = cp0_entrylo0[25:6];
assign w_c0   = cp0_entrylo0[5:3];
assign w_d0   = cp0_entrylo0[2];
assign w_v0   = cp0_entrylo0[1];
// ENTRYLO1
assign w_pfn1 = cp0_entrylo1[25:6];
assign w_c1   = cp0_entrylo1[5:3];
assign w_d1   = cp0_entrylo1[2];
assign w_v1   = cp0_entrylo1[1];  

// read port
wire  [$clog2(TLBNUM)-1:0] r_index;     
wire  [              18:0] r_vpn2;     
wire  [               7:0] r_asid;     
wire                       r_g;     
wire  [              19:0] r_pfn0;     
wire  [               2:0] r_c0;     
wire                       r_d0;
wire                       r_v0;     
wire  [              19:0] r_pfn1;     
wire  [               2:0] r_c1;     
wire                       r_d1;     
wire                       r_v1;
// INPUT
assign r_index = cp0_index[7:0];

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
    .count_eq_compare_out   (ws_count_eq_compare),

    // lab13 TLB
    .cp0_entryhi        (cp0_entryhi),
    .cp0_entrylo0       (cp0_entrylo0),
    .cp0_entrylo1       (cp0_entrylo1),
    .cp0_index          (cp0_index),
    
    // lab13 TLB    
    .tlbp               (ws_tlbp),
    .tlbr               (ws_tlbr),
    .tlbwi              (ws_tlbwi),

    // tlb search port 0
    .s0_found           (s0_found),
    .s0_index           (s0_index),
    .s0_pfn             (s0_pfn),
    .s0_c               (s0_c),
    .s0_d               (s0_d),
    .s0_v               (s0_v),

    // tlb search port 1
    .s1_found           (s1_found),
    .s1_index           (s1_index),
    .s1_pfn             (s1_pfn),
    .s1_c               (s1_c),
    .s1_d               (s1_d),
    .s1_v               (s1_v),    

    // tlb write port
    .w_index            (w_index),

    // tlb read port
    .r_vpn2             (r_vpn2),
    .r_asid             (r_asid),
    .r_g                (r_g),
    .r_pfn0             (r_pfn0),
    .r_c0               (r_c0),
    .r_d0               (r_d0),
    .r_v0               (r_v0),
    .r_pfn1             (r_pfn1),
    .r_c1               (r_c1),
    .r_d1               (r_d1),
    .r_v1               (r_v1)
);

tlb u_tlb(
    .clk                (clk),  
    // search port 0
    .s0_vpn2            (s0_vpn2),       
    .s0_odd_page        (s0_odd_page),     
    .s0_asid            (s0_asid),     
    .s0_found           (s0_found),     
    .s0_index           (s0_index),   
    .s0_pfn             (s0_pfn),     
    .s0_c               (s0_c),     
    .s0_d               (s0_d),     
    .s0_v               (s0_v), 
 
    // search port 1     
    .s1_vpn2            (s1_vpn2),     
    .s1_odd_page        (s1_odd_page),     
    .s1_asid            (s1_asid),     
    .s1_found           (s1_found),     
    .s1_index           (s1_index),      
    .s1_pfn             (s1_pfn),     
    .s1_c               (s1_c),     
    .s1_d               (s1_d),     
    .s1_v               (s1_v), 
 
    // write port     
    .we                 (we),     
    //w(rite) e(nable)     
    .w_index            (w_index),     
    .w_vpn2             (w_vpn2),     
    .w_asid             (w_asid),     
    .w_g                (w_g),     
    .w_pfn0             (w_pfn0),     
    .w_c0               (w_c0),     
    .w_d0               (w_d0),
    .w_v0               (w_v0),     
    .w_pfn1             (w_pfn1),     
    .w_c1               (w_c1),     
    .w_d1               (w_d1),     
    .w_v1               (w_v1), 
 
    // read port     
    .r_index            (r_index),     
    .r_vpn2             (r_vpn2),     
    .r_asid             (r_asid),     
    .r_g                (r_g),     
    .r_pfn0             (r_pfn0),     
    .r_c0               (r_c0),     
    .r_d0               (r_d0),     
    .r_v0               (r_v0),     
    .r_pfn1             (r_pfn1),     
    .r_c1               (r_c1),     
    .r_d1               (r_d1),     
    .r_v1               (r_v1)     
);

endmodule