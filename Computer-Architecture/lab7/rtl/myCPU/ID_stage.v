`include "mycpu.h"

module id_stage(
    input                          clk           ,
    input                          reset         ,
    //allowin
    input                          es_allowin    ,
    output                         ds_allowin    ,
    //from fs
    input                          fs_to_ds_valid,
    input  [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus  ,
    //to es
    output                         ds_to_es_valid,
    output [`DS_TO_ES_BUS_WD -1:0] ds_to_es_bus  ,
    //to fs
    output [`BR_BUS_WD      -1:0] br_bus        ,
    //to rf: for write back
    input  [`WS_TO_RF_BUS_WD -1:0] ws_to_rf_bus,
    
    input  [4:0] reg_dest_es,
    input  [4:0] reg_dest_ms,
    input  [4:0] reg_dest_ws,
    
    input   [31:0] es_value,
    input   [31:0] ms_value,
    input   [31:0] ws_value,
    
    input   es_lw_valid
);


wire        ds_src_type;

reg         ds_valid   ;
wire        ds_ready_go;

wire [31                 :0] fs_pc;
reg  [`FS_TO_DS_BUS_WD -1:0] fs_to_ds_bus_r;
assign fs_pc = fs_to_ds_bus[31:0];

wire [31:0] ds_inst;
wire [31:0] ds_pc  ;
assign {ds_inst,
        ds_pc  } = fs_to_ds_bus_r;

wire        rf_we   ;
wire [ 4:0] rf_waddr;
wire [31:0] rf_wdata;
assign {rf_we   ,  //37:37
        rf_waddr,  //36:32
        rf_wdata   //31:0
       } = ws_to_rf_bus;

wire        br_taken;
wire [31:0] br_target;

wire [11:0] alu_op;
wire        load_op;
wire        src1_is_sa;
wire        src1_is_pc;
wire        src2_is_imm;
wire        src2_is_0imm;
wire        src2_is_8;
wire        res_from_mem;
wire        gr_we;
wire        mem_we;
wire [ 4:0] dest;
wire [15:0] imm;
wire [31:0] rs_value;
wire [31:0] rt_value;

// lab6 append
wire [9:0] mudi;
wire [14:0] mvto;

wire [ 5:0] op;
wire [ 4:0] rs;
wire [ 4:0] rt;
wire [ 4:0] rd;
wire [ 4:0] sa;
wire [ 5:0] func;
wire [25:0] jidx;
wire [63:0] op_d;
wire [31:0] rs_d;
wire [31:0] rt_d;
wire [31:0] rd_d;
wire [31:0] sa_d;
wire [63:0] func_d;

// lab6 append
wire        inst_add;
wire        inst_addi;
wire        inst_sub;
wire        inst_slti;
wire        inst_sltiu;
wire        inst_andi;
wire        inst_ori;
wire        inst_xori;
wire        inst_sllv;
wire        inst_srlv;
wire        inst_srav;
wire        inst_mult;
wire        inst_multu;
wire        inst_div;
wire        inst_divu;
wire        inst_mfhi;
wire        inst_mflo;
wire        inst_mthi;
wire        inst_mtlo;

wire        inst_addu;//2s
wire        inst_subu;//2s
wire        inst_slt;//2s
wire        inst_sltu;//2s
wire        inst_and;//2s
wire        inst_or;//2s
wire        inst_xor;//2s
wire        inst_nor;//2s
wire        inst_sll;//1s,1d,1i
wire        inst_srl;//1s,1d,1i
wire        inst_sra;//1s,1d,1i
wire        inst_addiu;//1s
wire        inst_lui;//ns
wire        inst_lw;
wire        inst_sw;
wire        inst_beq;//2s,nd
wire        inst_bne;//2s,nd
wire        inst_jal;//ns,1d
wire        inst_jr;//1s,nd
//lab7
wire		inst_bgez;
wire        inst_bgtz;
wire        inst_blez;
wire        inst_bltz;
wire        inst_j;
wire        inst_bltzal;
wire   		inst_bgezal;
wire 		inst_jalr;
wire		inst_lb;
wire 		inst_lbu;
wire 		inst_lh;
wire 		inst_lhu;
wire		inst_lwl;
wire		inst_lwr;
wire 		inst_sb;
wire		inst_sh;
wire		inst_swl;
wire 		inst_swr;
wire        dst_is_r31;  
wire        dst_is_rt;   

wire [ 4:0] rf_raddr1;
wire [31:0] rf_rdata1;
wire [ 4:0] rf_raddr2;
wire [31:0] rf_rdata2;

wire        rs_eq_rt;

assign br_bus       = {br_taken,br_target};

assign ds_to_es_bus = {
					   inst_swl,	  //154:154
					   inst_swr,	  //153:153
					   inst_sb,       //152:152
					   inst_sh,       //151:151
					   inst_lwl,      //150:150
					   inst_lwr,	  // 149:149
					   inst_lhu,      //148:148
					   inst_lh,		  //147:147
					   inst_lbu,	  //146:146	
					   inst_lb,       //145:145
                       inst_mult,     //144:144
                       inst_multu,    //143:143
                       inst_div,      //142:142
                       inst_divu,     //141:141
                       inst_mfhi,     //140:140
                       inst_mflo,     //139:139
                       inst_mthi,     //138:138
                       inst_mtlo,     //137:137
                       src2_is_0imm,  //136:136
                       alu_op      ,  //135:124
                       load_op     ,  //123:123
                       src1_is_sa  ,  //122:122
                       src1_is_pc  ,  //121:121
                       src2_is_imm ,  //120:120
                       src2_is_8   ,  //119:119
                       gr_we       ,  //118:118
                       mem_we      ,  //117:117
                       dest        ,  //116:112
                       imm         ,  //111:96
                       rs_value    ,  //95 :64
                       rt_value    ,  //63 :32
                       ds_pc          //31 :0
                      };                      

assign ds_allowin     = !ds_valid || ds_ready_go && es_allowin;
assign ds_to_es_valid = ds_valid && ds_ready_go;
always @(posedge clk) begin
    if (reset) begin
        ds_valid <= 1'b0;
    end
    else if (ds_allowin) begin
        ds_valid <= fs_to_ds_valid;
    end

    if (fs_to_ds_valid && ds_allowin) begin
        fs_to_ds_bus_r <= fs_to_ds_bus;
    end
end

// lab6 append
assign mudi = ds_inst[15:6];
assign mvto = ds_inst[20:6];

assign op   = ds_inst[31:26];
assign rs   = ds_inst[25:21];
assign rt   = ds_inst[20:16];
assign rd   = ds_inst[15:11];
assign sa   = ds_inst[10: 6];
assign func = ds_inst[ 5: 0];
assign imm  = ds_inst[15: 0];
assign jidx = ds_inst[25: 0];

decoder_6_64 u_dec0(.in(op  ), .out(op_d  ));
decoder_6_64 u_dec1(.in(func), .out(func_d));
decoder_5_32 u_dec2(.in(rs  ), .out(rs_d  ));
decoder_5_32 u_dec3(.in(rt  ), .out(rt_d  ));
decoder_5_32 u_dec4(.in(rd  ), .out(rd_d  ));
decoder_5_32 u_dec5(.in(sa  ), .out(sa_d  ));
//lab7
assign inst_bgez = op_d[6'h01] & rt_d[5'h01];
assign inst_bgtz = op_d[6'h07] & rt_d[5'h00];
assign inst_blez = op_d[6'h06] & rt_d[5'h00];
assign inst_bltz = op_d[6'h01] & rt_d[5'h00];
assign inst_j = op_d[6'h02];
assign inst_bltzal = op_d[6'h01] & rt_d[5'h10];
assign inst_bgezal = op_d[6'b01] & rt_d[5'h11];
assign inst_jalr = op_d[6'h00] & rt_d[5'h00] & func_d[6'h09];
assign inst_lb = op_d[6'h20];
assign inst_lbu = op_d[6'h24];
assign inst_lh = op_d[6'h21];
assign inst_lhu = op_d[6'h25];
assign inst_lwl = op_d[6'h22];
assign inst_lwr = op_d[6'h26];
assign inst_sb = op_d[6'h28];
assign inst_sh = op_d[6'h29];
assign inst_swl = op_d[6'h2a];
assign inst_swr = op_d[6'h2e];
// lab6 append
assign inst_add    = op_d[6'h00] & func_d[6'h20] & sa_d[5'h00];
assign inst_addi   = op_d[6'h08];
assign inst_sub    = op_d[6'h00] & func_d[6'h22] & sa_d[5'h00];
assign inst_slti   = op_d[6'h0a];
assign inst_sltiu  = op_d[6'h0b];
assign inst_andi   = op_d[6'h0c];
assign inst_ori    = op_d[6'h0d];
assign inst_xori   = op_d[6'h0e];
assign inst_sllv   = op_d[6'h00] & func_d[6'h04] & sa_d[5'h00];
assign inst_srlv   = op_d[6'h00] & func_d[6'h06] & sa_d[5'h00];
assign inst_srav   = op_d[6'h00] & func_d[6'h07] & sa_d[5'h00];
assign inst_mult   = op_d[6'h00] & func_d[6'h18] & (mudi == 10'b0);
assign inst_multu  = op_d[6'h00] & func_d[6'h19] & (mudi == 10'b0);
assign inst_div    = op_d[6'h00] & func_d[6'h1a] & (mudi == 10'b0);
assign inst_divu   = op_d[6'h00] & func_d[6'h1b] & (mudi == 10'b0);
assign inst_mfhi   = op_d[6'h00] & func_d[6'h10] & sa_d[5'h00];
assign inst_mflo   = op_d[6'h00] & func_d[6'h12] & sa_d[5'h00];
assign inst_mthi   = op_d[6'h00] & func_d[6'h11] & (mvto == 15'b0);
assign inst_mtlo   = op_d[6'h00] & func_d[6'h13] & (mvto == 15'b0);

assign inst_addu   = op_d[6'h00] & func_d[6'h21] & sa_d[5'h00];
assign inst_subu   = op_d[6'h00] & func_d[6'h23] & sa_d[5'h00];
assign inst_slt    = op_d[6'h00] & func_d[6'h2a] & sa_d[5'h00];
assign inst_sltu   = op_d[6'h00] & func_d[6'h2b] & sa_d[5'h00];
assign inst_and    = op_d[6'h00] & func_d[6'h24] & sa_d[5'h00];
assign inst_or     = op_d[6'h00] & func_d[6'h25] & sa_d[5'h00];
assign inst_xor    = op_d[6'h00] & func_d[6'h26] & sa_d[5'h00];
assign inst_nor    = op_d[6'h00] & func_d[6'h27] & sa_d[5'h00];
assign inst_sll    = op_d[6'h00] & func_d[6'h00] & rs_d[5'h00];
assign inst_srl    = op_d[6'h00] & func_d[6'h02] & rs_d[5'h00];
assign inst_sra    = op_d[6'h00] & func_d[6'h03] & rs_d[5'h00];
assign inst_addiu  = op_d[6'h09];
assign inst_lui    = op_d[6'h0f] & rs_d[5'h00];
assign inst_lw     = op_d[6'h23];
assign inst_sw     = op_d[6'h2b];
assign inst_beq    = op_d[6'h04];
assign inst_bne    = op_d[6'h05];
assign inst_jal    = op_d[6'h03];
assign inst_jr     = op_d[6'h00] & func_d[6'h08] & rt_d[5'h00] & rd_d[5'h00] & sa_d[5'h00];


assign alu_op[ 0] = inst_addu | inst_add | inst_addiu | inst_addi | inst_lw |
					inst_sw | inst_jal | inst_bltzal | inst_bgezal | inst_jalr | 
					inst_lb |inst_lbu | inst_lh | inst_lhu | inst_lwl | inst_lwr |
					inst_sb | inst_sh | inst_swl | inst_swr;
assign alu_op[ 1] = inst_subu | inst_sub;
assign alu_op[ 2] = inst_slt | inst_slti;
assign alu_op[ 3] = inst_sltu | inst_sltiu;
assign alu_op[ 4] = inst_and | inst_andi;
assign alu_op[ 5] = inst_nor;
assign alu_op[ 6] = inst_or | inst_ori;
assign alu_op[ 7] = inst_xor | inst_xori;
assign alu_op[ 8] = inst_sll | inst_sllv;
assign alu_op[ 9] = inst_srl | inst_srlv;
assign alu_op[10] = inst_sra | inst_srav;
assign alu_op[11] = inst_lui;

assign load_op = inst_lw | inst_lb | inst_lbu | inst_lh | inst_lhu | inst_lwl | inst_lwr;

assign src1_is_sa   = inst_sll   | inst_srl | inst_sra;
assign src1_is_pc   = inst_jal || inst_bltzal || inst_bgezal || inst_jalr;
assign src2_is_imm  = inst_addiu | inst_addi | inst_slti | inst_sltiu | inst_lui | inst_lw |
					  inst_lb |inst_lbu | inst_lh | inst_lhu| inst_sw | inst_lwl | inst_lwr | inst_sb | inst_sh | inst_swl | inst_swr;
assign src2_is_0imm = inst_andi | inst_ori | inst_xori;
assign src2_is_8    = inst_jal || inst_bltzal || inst_bgezal || inst_jalr;
assign res_from_mem = inst_lw | inst_lb | inst_lh | inst_lhu | inst_lbu |inst_lwl | inst_lwr;
assign dst_is_r31   = inst_jal || inst_bltzal || inst_bgezal;
assign dst_is_rt    = inst_addiu | inst_addi | inst_slti | inst_sltiu | inst_andi | inst_ori | inst_xori |
                      inst_lui | inst_lw | inst_lb | inst_lh | inst_lhu |inst_lbu | inst_lwl | inst_lwr;
assign gr_we        = ~inst_sw & ~inst_beq & ~inst_bne & ~inst_jr & ~inst_mthi & ~inst_mtlo & ~inst_mult & ~inst_multu &
                      ~inst_div & ~inst_divu & ~inst_bgez & ~inst_bgtz & ~inst_blez & ~inst_bltz 
					  & ~inst_j & ~inst_sb & ~inst_sh & ~inst_swl & ~inst_swr;
assign mem_we       = inst_sw | inst_sb | inst_sh | inst_swl | inst_swr;

assign dest         = dst_is_r31 ? 5'd31 :
                      dst_is_rt  ? rt    : 
                                   rd;

assign rf_raddr1 = rs;
assign rf_raddr2 = rt;
regfile u_regfile(
    .clk    (clk      ),
    .raddr1 (rf_raddr1),
    .rdata1 (rf_rdata1),
    .raddr2 (rf_raddr2),
    .rdata2 (rf_rdata2),
    .we     (rf_we    ),
    .waddr  (rf_waddr ),
    .wdata  (rf_wdata )
    );

//assign rs_value = rf_rdata1;
//assign rt_value = rf_rdata2;

assign rs_value = (rs) ? ((rs == reg_dest_es)?(es_value):
                        ((rs == reg_dest_ms)?(ms_value):
                        ((rs == reg_dest_ws)?(ws_value):(rf_rdata1)))):(rf_rdata1);
                        
assign rt_value = (rt) ? ((rt == reg_dest_es)?(es_value):
                        ((rt == reg_dest_ms)?(ms_value):
                        ((rt == reg_dest_ws)?(ws_value):(rf_rdata2)))):(rf_rdata2);
                        
//assign ds_ready_go = !es_lw_valid;
assign ds_ready_go = !(es_lw_valid && ( (rs == reg_dest_es || rs == reg_dest_ms || rs == reg_dest_ws) || 
                      (rt == reg_dest_es || rt == reg_dest_ms ||rt == reg_dest_ws) ));                      
//assign ds_ready_go = 1'b1;
//lab7
wire rs_bgez;
wire rs_bgtz;
//assign rs_bgez = (rs_value >= 1'b0);
assign rs_bgez = ~rs_value[31];
//assign rs_bgtz = (rs_value > 1'b0);
assign rs_bgtz = (~rs_value[31]) & (rs_value != 32'b0);



 /*用大于等于还是符号为可以考虑�?�?*/
//assign rs_eq_rt = (rs_value >= 1'b0);
assign rs_eq_rt = (rs_value == rt_value);


assign br_taken = (   inst_beq  &&  rs_eq_rt
                   || inst_bne  && !rs_eq_rt
                   || inst_jal
				   || inst_j
                   || inst_jr
				   || inst_jalr
				   || (inst_bgez && rs_bgez)
				   || (inst_bgtz && rs_bgtz)
				   || (inst_blez && (~rs_bgtz))
				   || (inst_bltz && (~rs_bgez))
				   || (inst_bltzal && (~rs_bgez))
				   || (inst_bgezal && rs_bgez)
                  ) && ds_valid;
assign br_target = (inst_beq || inst_bne || inst_bgez || inst_bltz || inst_bgtz || inst_blez || inst_bltzal || inst_bgezal) ? (fs_pc + {{14{imm[15]}}, imm[15:0], 2'b0}) :
                   (inst_jr || inst_jalr)              ? rs_value :
                  /*jal, j,bltzal, */              {fs_pc[31:28], jidx[25:0], 2'b0};

endmodule
