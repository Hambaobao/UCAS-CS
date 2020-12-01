#include "mac.h"
#include "irq.h"
#include "type.h"
#include "screen.h"
#include "syscall.h"
#include "sched.h"
#include "test5.h"

desc_t *send_desc;
desc_t *receive_desc;
uint32_t cnt = 1; //record the time of iqr_mac
//uint32_t buffer[PSIZE] = {0x00040045, 0x00000100, 0x5d911120, 0x0101a8c0, 0xfb0000e0, 0xe914e914, 0x00000801,0x45000400, 0x00010000, 0x2011915d, 0xc0a80101, 0xe00000fb, 0x14e914e9, 0x01080000};
uint32_t buffer[PSIZE] = {0xffffffff, 0x5500ffff, 0xf77db57d, 0x00450008, 0x0000d400, 0x11ff0040, 0xa8c073d8, 0x00e00101, 0xe914fb00, 0x0004e914, 0x0000, 0x005e0001, 0x2300fb00, 0x84b7f28b, 0x00450008, 0x0000d400, 0x11ff0040, 0xa8c073d8, 0x00e00101, 0xe914fb00, 0x0801e914, 0x0000};

/**
 * Clears all the pending interrupts.
 * If the Dma status register is read then all the interrupts gets cleared
 * @param[in] pointer to synopGMACdevice.
 * \return returns void.
 * you will use it in task3
 */
void clear_interrupt()
{
    uint32_t data;
    data = reg_read_32(0xbfe11000 + DmaStatus);
    reg_write_32(0xbfe11000 + DmaStatus, data);
}

static void mac_send_desc_init(mac_t *mac)
{
    int i,j;
    // initial Send Buffer
    for(i = 0; i < PNUM; i++)
        for(j = 0; j < PSIZE; j++)
            TX_Buffer[i][j] = 0;

    // initial TX_Desc
    for(i = 0; i < PNUM - 1; i++){
        TX_Desc[i].tdes0 = 0;
        TX_Desc[i].tdes1 = RDES1_LS | RDES1_FS | RDES1_RCH | RDES1_RBS1;
        TX_Desc[i].tdes2 = ((int)(&TX_Buffer[i]) & 0x1fffffff);
        TX_Desc[i].tdes3 = ((int)(&TX_Desc[i+1]) & 0x1fffffff);
    }

    TX_Desc[63].tdes0 = 0;
    TX_Desc[63].tdes1 = RDES1_LS | RDES1_FS | RDES1_RER | RDES1_RCH | RDES1_RBS1;
    TX_Desc[63].tdes2 = ((uint32_t)(&TX_Buffer[63]) & 0x1fffffff);
    TX_Desc[63].tdes3 = ((uint32_t)(&TX_Desc[0]) & 0x1fffffff);

    // Fill Send Buffer
    for(i = 0; i < PNUM; i++)
    {
        for(j = 0; j < PSIZE - 1; j++)
            TX_Buffer[i][j] = buffer[j];
        TX_Buffer[i][j] = i;        
    }
        
    // Initial Mac
    mac->saddr      = (uint32_t)(&TX_Buffer[0]);    // Send Vaddr
    mac->saddr_phy  = ((mac->saddr) & 0x1fffffff);  // Send Paddr
    mac->td         = (uint32_t)(&TX_Desc[0]);      // Send(Transmit) Desc Vaddr
    mac->td_phy     = ((mac->td) & 0x1fffffff);     // Send(Transmit) Desc Paddr
}

static void mac_recv_desc_init(mac_t *mac)
{
    int i,j;
    // initial Send Buffer
    for(i = 0; i < PNUM; i++)
        for(j = 0; j < PSIZE; j++)
            RX_Buffer[i][j] = 0;

    // initial TX_Desc
    for(i = 0; i < PNUM - 1; i++){
        RX_Desc[i].tdes0 = 0;
        RX_Desc[i].tdes1 = RDES1_RCH | RDES1_RBS1;
        RX_Desc[i].tdes2 = ((int)(&RX_Buffer[i]) & 0x1fffffff);
        RX_Desc[i].tdes3 = ((int)(&RX_Desc[i+1]) & 0x1fffffff);
    }

    RX_Desc[63].tdes0 = 0;
    RX_Desc[63].tdes1 = RDES1_RER | RDES1_RCH | RDES1_RBS1;
    RX_Desc[63].tdes2 = ((uint32_t)(&RX_Buffer[63]) & 0x1fffffff);
    RX_Desc[63].tdes3 = ((uint32_t)(&RX_Desc[0]) & 0x1fffffff);

    // Initial Mac
    mac->daddr      = (uint32_t)(&RX_Buffer[0]);    // Recv Vaddr
    mac->daddr_phy  = ((mac->daddr) & 0x1fffffff);  // Recv Paddr
    mac->rd         = (uint32_t)(&RX_Desc[0]);      // Recv Desc Vaddr
    mac->rd_phy     = ((mac->rd) & 0x1fffffff);     // Recv Desc Paddr
}


static uint32_t printf_recv_buffer(uint32_t recv_buffer)
{
}

static void mii_dul_force(mac_t *mac)
{
    reg_write_32(mac->dma_addr, 0x80); //?s
                                       //   reg_write_32(mac->dma_addr, 0x400);
    uint32_t conf = 0xc800;            //0x0080cc00;

    // loopback, 100M
    reg_write_32(mac->mac_addr, reg_read_32(mac->mac_addr) | (conf) | (1 << 8));
    //enable recieve all
    reg_write_32(mac->mac_addr + 0x4, reg_read_32(mac->mac_addr + 0x4) | 0x80000001);
}

void dma_control_init(mac_t *mac, uint32_t init_value)
{
    reg_write_32(mac->dma_addr + DmaControl, init_value);
    return;
}

void mac_send_task()
{

    mac_t test_mac;
    uint32_t i;
    uint32_t print_location = 2;

    test_mac.mac_addr = 0xbfe10000;
    test_mac.dma_addr = 0xbfe11000;

    test_mac.psize = PSIZE * 4; // 64bytes
    test_mac.pnum = PNUM;       // pnum

    mac_send_desc_init(&test_mac);

    dma_control_init(&test_mac, DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128);
    clear_interrupt(&test_mac);

    mii_dul_force(&test_mac);

    // register_irq_handler(LS1C_MAC_IRQ, mac_irq_handle);

    // irq_enable(LS1C_MAC_IRQ);
    sys_move_cursor(1, print_location);
    printf("> [MAC SEND TASK] start send package.               \n");

    uint32_t cnt = 0;
    i = 4;
    while (i > 0)
    {
        sys_net_send(test_mac.td, test_mac.td_phy);
        cnt += PNUM;
        sys_move_cursor(1, print_location);
        printf("> [MAC SEND TASK] totally send package %d !        \n", cnt);
        i--;
    }
    sys_exit();
}

void mac_recv_task()
{

    mac_t test_mac;
    uint32_t i;
    uint32_t ret;
    uint32_t print_location = 1;

    test_mac.mac_addr = 0xbfe10000;
    test_mac.dma_addr = 0xbfe11000;

    test_mac.psize = PSIZE * 4; // 64bytes
    test_mac.pnum = PNUM;       // pnum
    mac_recv_desc_init(&test_mac);

    dma_control_init(&test_mac, DmaStoreAndForward | DmaTxSecondFrame | DmaRxThreshCtrl128);
    clear_interrupt(&test_mac);

    mii_dul_force(&test_mac);

    queue_init(&recv_block_queue);
    sys_move_cursor(1, print_location);
    printf("[MAC RECV TASK] start recv:                    ");
    ret = sys_net_recv(test_mac.rd, test_mac.rd_phy, test_mac.daddr);
    if (ret == 0)
    {
        sys_move_cursor(1, print_location);
        printf("[MAC RECV TASK]     net recv is ok!                          ");
    }
    else
    {
        sys_move_cursor(1, print_location);
        printf("[MAC RECV TASK]     net recv is fault!                       ");
    }

    ch_flag = 0;
    for (i = 0; i < PNUM; i++)
    {
        recv_flag[i] = 0;
    }

    uint32_t cnt = 0;
    uint32_t *Recv_desc;
    Recv_desc = (uint32_t *)(test_mac.rd + (PNUM - 1) * 16);
    //printf("(test_mac.rd 0x%x ,Recv_desc=0x%x,REDS0 0X%x\n", test_mac.rd, Recv_desc, *(Recv_desc));
    if (((*Recv_desc) & 0x80000000) == 0x80000000)
    {
        sys_move_cursor(1, print_location);
        printf("> [MAC RECV TASK] waiting receive package.\n");
        sys_wait_recv_package();
    }

    //init_screen();
    cnt += PNUM;
    sys_move_cursor(1, print_location);
    printf("> [MAC RECV TASK]print recv  buffer        \n");
    uint32_t recv_buffer, snd_buffer;
    desc_t *recv = NULL;
    recv_buffer = test_mac.daddr;
    snd_buffer = test_mac.saddr;
    print_location = 3;

    uint32_t valid_num = 0;
    for (i = 0; i < PNUM; i++)
    {
        if (recv_flag[i] == 0)
        {
            sys_move_cursor(1, print_location);
            printf("> [MAC RECV TASK]still waiting receive %dth package.             \n", i);
            sys_wait_recv_package();
        }

        Recv_desc = (uint32_t *)(test_mac.rd + i * 16);
        recv = (desc_t *)Recv_desc;

        sys_move_cursor(1, print_location);
        printf("\n%d recv buffer,r_desc( 0x%x) =0x%x:          \n", i, Recv_desc, *(Recv_desc));

        recv_buffer = recv->tdes2;
        valid_num += printf_recv_buffer((recv->tdes2 | 0xa0000000));
        sys_sleep(1);
        printf("\n");
    }

    print_location = 3;
    sys_move_cursor(1, print_location);
    printf("\nrecv valid %d packages!:\n                ", valid_num);

    sys_exit();
}

void mac_init_task()
{
    uint32_t print_location = 1;
    sys_move_cursor(1, print_location);
    printf("> [MAC INIT TASK] Waiting for MAC initialization .\n");
    sys_init_mac();
    sys_move_cursor(1, print_location);
    printf("> [MAC INIT TASK] MAC initialization succeeded.           \n");
    sys_exit();
}
