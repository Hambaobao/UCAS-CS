module mycpu_top(
    input         clk,
    input         resetn,
    // inst sram interface
    output        inst_sram_en,
    output [ 3:0] inst_sram_wen,
    output [31:0] inst_sram_addr,
    output [31:0] inst_sram_wdata,
    input  [31:0] inst_sram_rdata,
    // data sram interface
    output        data_sram_en,
    output [ 3:0] data_sram_wen,
    output [31:0] data_sram_addr,
    output [31:0] data_sram_wdata,
    input  [31:0] data_sram_rdata,
    // trace debug interface
    output [31:0] debug_wb_pc,
    output [ 3:0] debug_wb_rf_wen,
    output [ 4:0] debug_wb_rf_wnum,
    output [31:0] debug_wb_rf_wdata
);
reg         reset;
always @(posedge clk) reset <= ~resetn;

wire         ds_allowin;
wire         es_allowin;
wire         ms_allowin;
wire         ws_allowin;
wire         fs_to_ds_valid;
wire         ds_to_es_valid;
wire         es_to_ms_valid;
wire         ms_to_ws_valid;
wire [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus;
wire [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus;
wire [`ES_TO_MS_BUS_WD -1:0] es_to_ms_bus;
wire [`MS_TO_WS_BUS_WD -1:0] ms_to_ws_bus;
wire [`WS_TO_RF_BUS_WD -1:0] ws_to_rf_bus;
wire [`BR_BUS_WD       -1:0] br_bus;


wire [4:0]  reg_dest_es;
wire [4:0]  reg_dest_ms;
wire [4:0]  reg_dest_ws;

wire [31:0] es_value;
wire [31:0] ms_value;
wire [31:0] ws_value;

wire es_lw_valid;

// lab8
wire ws_ex;
//wire ms_ex;
//wire es_ex;
//wire ds_ex;
//wire fs_ex;

wire ws_mfc0;
wire ms_mfc0;
wire es_mfc0;

// CPO
wire eret_flush;
wire [31:0] cp0_status;
wire [31:0] cp0_epc;
wire [31:0] cp0_cause;
wire        count_eq_compare;

// IF stage
if_stage if_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ds_allowin     (ds_allowin     ),
    //brbus
    .br_bus         (br_bus         ),
    //outputs
    .fs_to_ds_valid (fs_to_ds_valid ),
    .fs_to_ds_bus   (fs_to_ds_bus   ),
    // inst sram interface
    .inst_sram_en   (inst_sram_en   ),
    .inst_sram_wen  (inst_sram_wen  ),
    .inst_sram_addr (inst_sram_addr ),
    .inst_sram_wdata(inst_sram_wdata),
    .inst_sram_rdata(inst_sram_rdata),
    
    //lab8
    .eret_flush     (eret_flush),
    .cp0_epc        (cp0_epc),
    //.ds_ex_in       (ds_ex),
    //.es_ex_in       (es_ex),
    //.ms_ex_in       (ms_ex),
    .ws_ex_in       (ws_ex)
);
// ID stage
id_stage id_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .es_allowin     (es_allowin     ),
    .ds_allowin     (ds_allowin     ),
    //from fs
    .fs_to_ds_valid (fs_to_ds_valid ),
    .fs_to_ds_bus   (fs_to_ds_bus   ),
    //to es
    .ds_to_es_valid (ds_to_es_valid ),
    .ds_to_es_bus   (ds_to_es_bus   ),
    //to fs
    .br_bus         (br_bus         ),
    //to rf: for write back
    .ws_to_rf_bus   (ws_to_rf_bus   ),
    
    //pipeline block
    .reg_dest_es    (reg_dest_es),
    .reg_dest_ms    (reg_dest_ms),
    .reg_dest_ws    (reg_dest_ws),
    
    .es_value       (es_value),
    .ms_value       (ms_value),
    .ws_value       (ws_value),
    
    .es_mfc0        (es_mfc0),
    .ms_mfc0        (ms_mfc0),
    .ws_mfc0        (ws_mfc0),
    
    .es_lw_valid    (es_lw_valid),
    
     //CP0
    .eret_flush                 (eret_flush),
    .ws_ex_in                   (ws_ex),
    .ws_to_ds_cp0_status        (cp0_status),
    .ws_to_ds_cp0_cause         (cp0_cause),
    .ws_to_ds_count_eq_compare  (count_eq_compare)
);
// EXE stage
exe_stage exe_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ms_allowin     (ms_allowin     ),
    .es_allowin     (es_allowin     ),
    //from ds
    .ds_to_es_valid (ds_to_es_valid ),
    .ds_to_es_bus   (ds_to_es_bus   ),
    //to ms
    .es_to_ms_valid (es_to_ms_valid ),
    .es_to_ms_bus   (es_to_ms_bus   ),
    // data sram interface
    .data_sram_en   (data_sram_en   ),
    .data_sram_wen  (data_sram_wen  ),
    .data_sram_addr (data_sram_addr ),
    .data_sram_wdata(data_sram_wdata),
    
    //pipeline block
    .reg_dest_es    (reg_dest_es),
    .es_value       (es_value),
    .es_lw_valid     (es_lw_valid),

    //lab8 append
    .eret_flush     (eret_flush),
    .ws_ex_in       (ws_ex),
    .es_mfc0        (es_mfc0),       
    .ms_ex_in       (ms_ex)

);
// MEM stage
mem_stage mem_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ws_allowin     (ws_allowin     ),
    .ms_allowin     (ms_allowin     ),
    //from es
    .es_to_ms_valid (es_to_ms_valid ),
    .es_to_ms_bus   (es_to_ms_bus   ),
    //to ws
    .ms_to_ws_valid (ms_to_ws_valid ),
    .ms_to_ws_bus   (ms_to_ws_bus   ),
    //from data-sram
    .data_sram_rdata(data_sram_rdata),

    //pipeline block
    .reg_dest_ms    (reg_dest_ms),
    .ms_value       (ms_value),
    .ms_ex_out      (ms_ex),  
    
    //lab8 append 
    .eret_flush     (eret_flush),
    .ws_ex_in       (ws_ex),
    .ms_mfc0        (ms_mfc0)   
);
// WB stage
wb_stage wb_stage(
    .clk            (clk            ),
    .reset          (reset          ),
    //allowin
    .ws_allowin     (ws_allowin     ),
    //from ms
    .ms_to_ws_valid (ms_to_ws_valid ),
    .ms_to_ws_bus   (ms_to_ws_bus   ),
    //to rf: for write back
    .ws_to_rf_bus   (ws_to_rf_bus   ),
    //trace debug interface
    .debug_wb_pc      (debug_wb_pc      ),
    .debug_wb_rf_wen  (debug_wb_rf_wen  ),
    .debug_wb_rf_wnum (debug_wb_rf_wnum ),
    .debug_wb_rf_wdata(debug_wb_rf_wdata),  
    //pipeline block
    .reg_dest_ws    (reg_dest_ws),
    .ws_value       (ws_value),
    
    //lab8 append
	.ws_mfc0            (ws_mfc0),
    .eret_flush         (eret_flush),
	.cp0_epc            (cp0_epc),
       
    //CP0
    
    .cp0_status         (cp0_status),
    .cp0_cause          (cp0_cause),
    .count_eq_compare   (count_eq_compare),
	.ws_ex_out          (ws_ex)

);

endmodule
