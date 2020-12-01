`ifndef MYCPU_H
    `define MYCPU_H

    `define BR_BUS_WD       33
//    `define FS_TO_DS_BUS_WD 64
    `define FS_TO_DS_BUS_WD 103
        
//    `define DS_TO_ES_BUS_WD 137
//    `define DS_TO_ES_BUS_WD 155
    `define DS_TO_ES_BUS_WD 206
    
//    `define ES_TO_MS_BUS_WD 109
    `define ES_TO_MS_BUS_WD 160
    
//    `define MS_TO_WS_BUS_WD 70
    `define MS_TO_WS_BUS_WD 121
    
    `define WS_TO_RF_BUS_WD 38
    
    `define NO_DEST 0
    `define DO_DEST 1  
    `define NO_SRC 0
    `define DO_SRC 1

// EXCEPTION CODE  
    `define NO_EX   5'hff  
    `define EX_INT  5'h00
    `define EX_ADEL 5'h04
    `define EX_ADES 5'h05
    `define EX_SYS  5'h08
    `define EX_BP   5'h09
    `define EX_RI   5'h0a
    `define EX_OV   5'h0c
    
`endif
