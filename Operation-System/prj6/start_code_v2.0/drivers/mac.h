#ifndef INCLUDE_MAC_H_
#define INCLUDE_MAC_H_

#include "type.h"
#include "queue.h"
//#define TEST_REGS1
//#define TEST_REGS2
#define TEST_REGS3
#define GMAC_BASE_ADDR (0xbfe10000)
#define DMA_BASE_ADDR (0xbfe11000)
#define PSIZE (256)
#define PNUM (64)

extern queue_t recv_block_queue;
extern uint32_t recv_flag[PNUM];
extern uint32_t ch_flag;

/* exception function pointer*/
int (*mac_irq_handler[4])();

// MAC
#define LS1C_MAC_IRQ    (0x0)

// INT1
#define INT1_SR_ADDR    (0xbfd01058)    // INT1_SR Register Address
#define INT1_EN_ADDR    (0xbfd0105c)    // INT1_EN Register Address
#define INT1_SET_ADDR   (0xbfd01060)    // INT1_SET Register Address
#define INT1_CLR_ADDR   (0xbfd01064)    // INT1_CLR Register Address
#define INT1_POL_ADDR   (0xbfd01068)    // INT1_POL Register Address
#define INT1_EDGE_ADDR  (0xbfd0106c)    // INT1_POL Register Address

// DMA Registers
#define DMA_TX_PDR  (DMA_BASE_ADDR + 4) // Transmit Poll Demand Register
#define DMA_RX_PDR  (DMA_BASE_ADDR + 8) // Receive  Poll Demand Register

// RDES0
#define RDES0_OWN   (1 << 31)   // Own By DMA

// RDES1
#define RDES1_RBS1  (1024)      // Buffer1 Size        
#define RDES1_RCH   (1 << 24)   // Chain or Ring
#define RDES1_RER   (1 << 25)   // Last of Ring  
#define RDES1_FS    (1 << 29)   // First Segment
#define RDES1_LS    (1 << 30)   // Last Segment
#define RDES1_IC    (1 << 31)   // Interruption on Complete 

enum GmacRegisters
{
    GmacAddr0Low = 0x0044,  /* Mac frame filtering controls */
    GmacAddr0High = 0x0040, /* Mac address0 high Register  */
};

enum DmaRegisters
{
    DmaStatus = 0x0014,    /* CSR5 - Dma status Register                        */
    DmaInterrupt = 0x001C, /* CSR7 - Interrupt enable                           */
    DmaControl = 0x0018,   /* CSR6 - Dma Operation Mode Register                */
};

enum DmaControlReg
{

    DmaStoreAndForward = 0x00200000, /* (SF)Store and forward                            21      RW        0       */
    DmaRxThreshCtrl128 = 0x00000018, /* (RTC)Controls thre Threh of MTL tx Fifo 128      4:3   RW                */

    DmaTxStart = 0x00002000, /* (ST)Start/Stop transmission                      13      RW        0       */

    DmaTxSecondFrame = 0x00000004, /* (OSF)Operate on second frame                     4       RW        0       */
};

enum InitialRegisters
{
    DmaIntDisable = 0,
};

typedef struct desc
{
    uint32_t tdes0;
    uint32_t tdes1;
    uint32_t tdes2;
    uint32_t tdes3;
} desc_t;

extern desc_t TX_Desc[PNUM];   // DMA Send Descriptor
extern desc_t RX_Desc[PNUM];   // DMA Recv Descriptor
extern desc_t RV_Desc[PNUM];   // OK Recv Descriptor

extern uint32_t TX_Buffer[PNUM][PSIZE];  // DMA Send Buffer
extern uint32_t RX_Buffer[PNUM][PSIZE];  // DMA Recv Buffer
extern uint32_t RV_Buffer[PNUM][PSIZE];  // OK Recv Buffer

typedef struct mac
{
    uint32_t psize; // backpack size
    uint32_t pnum;
    uint32_t mac_addr; // MAC base address
    uint32_t dma_addr; // DMA base address

    uint32_t saddr; // send address
    uint32_t daddr; // receive address

    uint32_t saddr_phy; // send phy address
    uint32_t daddr_phy; // receive phy address

    uint32_t td; // DMA send desc
    uint32_t rd; // DMA receive desc

    uint32_t td_phy;
    uint32_t rd_phy;

} mac_t;

uint32_t read_register(uint32_t base, uint32_t offset);
void reg_write_32(uint32_t addr, uint32_t data);

void print_rx_dscrb(mac_t *mac);
void print_tx_dscrb(mac_t *mac);
uint32_t do_net_recv(uint32_t rd, uint32_t rd_phy, uint32_t daddr);
void do_net_send(uint32_t td, uint32_t td_phy);
void do_init_mac(void);
void do_wait_recv_package(void);
void mac_irq_handle(void);
void mac_recv_handle(mac_t *test_mac);
#endif
