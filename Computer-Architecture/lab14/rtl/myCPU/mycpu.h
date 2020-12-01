`ifndef MYCPU_H
    `define MYCPU_H

    `define BR_BUS_WD       35
//    `define FS_TO_DS_BUS_WD 64
    `define FS_TO_DS_BUS_WD 105
        
//    `define DS_TO_ES_BUS_WD 137
//    `define DS_TO_ES_BUS_WD 155
    `define DS_TO_ES_BUS_WD 212
    
//    `define ES_TO_MS_BUS_WD 109
    `define ES_TO_MS_BUS_WD 166
    
//    `define MS_TO_WS_BUS_WD 70
    `define MS_TO_WS_BUS_WD 126
    
    `define WS_TO_RF_BUS_WD 38
    
    `define NO_DEST 0
    `define DO_DEST 1  
    `define NO_SRC 0
    `define DO_SRC 1
	
	`define STATE_ONE 2'b01
	`define STATE_TWO 2'b10

// EXCEPTION CODE  
    `define NO_EX   5'hff  
    `define EX_INT  5'h00
    `define MOD     5'h01
    `define TLBL    5'h02
    `define TLBS    5'h03
    `define EX_ADEL 5'h04
    `define EX_ADES 5'h05
    `define EX_SYS  5'h08
    `define EX_BP   5'h09
    `define EX_RI   5'h0a
    `define EX_OV   5'h0c

    
// lab13    
    `define CP0_ENTRYHI_ADDR    8'b01010_000
    
`endif
