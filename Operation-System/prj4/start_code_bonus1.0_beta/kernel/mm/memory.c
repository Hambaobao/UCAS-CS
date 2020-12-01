#include "mm.h"
#include "sched.h"

pcb_t *current_running;
pcb_t pcb[NUM_MAX_TASK];
//TODO:Finish memory management functions here refer to mm.h and add any functions you need.

uint32_t pmem_distribute_index = 0;
void free_physical_scope(uint32_t start, uint32_t end)
{
    int p;
    p = start >> 12;
    p = p << 12;
    // 4k align
    for(; (p < end) & (pmem_distribute_index < NUMB_PAGE_FRAME); p += PAGE_SIZE , pmem_distribute_index++)
    {   
        physical_mem[pmem_distribute_index].paddr = p;
        physical_mem[pmem_distribute_index].prev = NULL;
        physical_mem[pmem_distribute_index].next = NULL;
        pmem_queue_push(&free_mem_list, (void *)(&physical_mem[pmem_distribute_index]));
    }
}

pageframe_t *palloc()
{
    pageframe_t *item;
    item = (pageframe_t *)pmem_queue_dequeue(&free_mem_list);
    pmem_queue_push(&busy_mem_list, (void *)(item));
    return item;
}

void pfree(pageframe_t *item)
{
    pmem_queue_remove(&busy_mem_list,item);
    pmem_queue_push(&free_mem_list,item);
}

// used in task2 & task3
void init_TLB()
{
    SET_CP0_PAGEMASK(0x0);
    SET_CP0_ENTRYHI(0x0);
    SET_CP0_ENTRYLO0(0x0);
    SET_CP0_ENTRYLO1(0x0);

    int i;
    for(i = 0; i < 32; i++)
    {
        SET_CP0_INDEX(i);
        asm volatile ("tlbwi");
    }    
}

uint32_t TLB_miss_time = 0;
void do_TLB_Refill()
{
    vt100_move_cursor(0,0);
    printk("-> TLB Exception %d                 ",TLB_miss_time);
    TLB_miss_time++;
//    current_running->user_context.cp0_epc = current_running->user_context.cp0_epc + 4;

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1)) & 0x3f;

    uint32_t entryhi,entrylo0,entrylo1,index,context,badvaddr; 

    entryhi  = GET_CP0_ENTRYHI();
    context  = GET_CP0_CONTEXT();
    badvaddr = GET_CP0_BADVADDR();

    uint32_t PPN0 = current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry;
    uint32_t PPN1 = current_running->page_table[(badvaddr >> 12) | 0x1].ptentry;  

//    context = context << 9;
    SET_CP0_ENTRYHI((entryhi & 0xfffff000) | (0xff & current_running->pid));
    asm volatile("tlbp");
    index = GET_CP0_INDEX();

    if(index & 0x80000000)
    {
        if(PPN0 & 0x1) // PTE Valid 
        {
            if(PPN0 & 0x2) // Page In Memory
            {
                entrylo0 = PPN0 >> 6;
                entrylo1 = PPN1 >> 6;  
                SET_CP0_ENTRYLO0(entrylo0);
                SET_CP0_ENTRYLO1(entrylo1);
                asm volatile("tlbwr");        
            }
            else        // Page In SD_card
            {
                swap_page_readback(badvaddr); // read back to memory
            }            
        }
        else
        {
            do_page_fault(badvaddr);
        }   
    }
    else
    { 
        //TLB invalid
        if(PPN0 & 0x1) // PTE Valid & 
        {
            if(PPN0 & 0x2) // Page In Memory
            {
                entrylo0 = PPN0 >> 6;
                entrylo1 = PPN1 >> 6;  
                SET_CP0_ENTRYLO0(entrylo0);
                SET_CP0_ENTRYLO1(entrylo1);
                asm volatile("tlbwr");        
            }
            else        // Page In SD_card
            {
                swap_page_readback(badvaddr); // read back to memory
            }            
        }
        else
        {
            do_page_fault(badvaddr);
        }    
    }
}

uint32_t Page_Fault_time = 0;
void do_page_fault(uint32_t badvaddr)
{
    vt100_move_cursor(0,0);
    printk("-> Page Fault Exception %d                  ",Page_Fault_time);
    Page_Fault_time++;

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1)) & 0x3f;

    uint32_t entryhi,entrylo0,entrylo1;  

    if(free_mem_list.head != NULL)
    {
        pageframe_t *frame;
        frame = palloc();
        current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;
        frame->vaddr = badvaddr & 0xffffe000;
        frame = palloc();
        current_running->page_table[(badvaddr >> 12) | 0x1].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;   
        frame->vaddr = (badvaddr & 0xfffff000) | 0x1000;

        uint32_t PPN0 = current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry;
        uint32_t PPN1 = current_running->page_table[(badvaddr >> 12) | 0x1].ptentry; 

        entryhi = (((badvaddr >> 12) >> 1) << 13) | (current_running->pid & 0xff);
        entrylo0 = PPN0 >> 6;
        entrylo1 = PPN1 >> 6;

        SET_CP0_ENTRYHI(entryhi);
        SET_CP0_ENTRYLO0(entrylo0);
        SET_CP0_ENTRYLO1(entrylo1);
        asm volatile("tlbwi");  
    }
    else
    {
        swap_page(badvaddr);
    }
}

uint32_t SD_Base = 0x40000000;
uint32_t SD_Page_Index = 0;
uint32_t Page_Swap_time = 0;
void swap_page(uint32_t badvaddr)
{
    vt100_move_cursor(0,0);
    printk("-> Exception: Swap Page %d                      ",Page_Swap_time);
    Page_Swap_time++;

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1)) & 0x3f;

//    uint32_t vaddr0,vaddr1;
    uint32_t SD_addr0,SD_addr1;
    uint32_t entryhi,entrylo0,entrylo1; 
    // FIFO Swap

    // 1. Get item = Physical Memory Block from busy_mem_list.head
    pageframe_t *item0, *item1;
    item0 = (pageframe_t *)pmem_queue_dequeue(&busy_mem_list);
    item1 = (pageframe_t *)pmem_queue_dequeue(&busy_mem_list);

    current_running->page_table[((item0->vaddr)>>12)].ptentry = 0;
    current_running->page_table[((item1->vaddr)>>12)].ptentry = 0;

    SD_addr0 = SD_Base + SD_Page_Index * PAGE_SIZE;
    SD_Page_Index++;
    SD_addr1 = SD_Base + SD_Page_Index * PAGE_SIZE;
    SD_Page_Index++;

    current_running->page_table[((item0->vaddr)>>12)].ptentry = 0x1 | SD_addr0;
    current_running->page_table[((item1->vaddr)>>12)].ptentry = 0x1 | SD_addr1;
    
    // 2. Write the context of item into SD card
    // Write_SD_card();
    sdwrite((uint8_t *)(item0->vaddr & 0xfffff000),SD_addr0,8);
    sdwrite((uint8_t *)(item1->vaddr & 0xfffff000),SD_addr1,8);

    // 3. Paging the vaddr to item block
    uint32_t PPN0,PPN1;

    PPN0 = current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry = (item0->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;
    PPN1 = current_running->page_table[(badvaddr >> 12) | 0x1].ptentry = (item1->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;  

    pmem_queue_push(&busy_mem_list,item0);
    pmem_queue_push(&busy_mem_list,item1);

    // 4. Replace TLB entry
    entryhi = (((badvaddr >> 12) >> 1) << 13) | (current_running->pid & 0xff);
    entrylo0 = PPN0 >> 6;
    entrylo1 = PPN1 >> 6;

    SET_CP0_ENTRYHI(entryhi);
    SET_CP0_ENTRYLO0(entrylo0);
    SET_CP0_ENTRYLO1(entrylo1);
    asm volatile("tlbwi");  
}

uint32_t Page_Swap_Readback_time = 0;
void swap_page_readback(uint32_t badvaddr)
{
    vt100_move_cursor(0,0);
    printk("-> Exception: Swap Page %d                      ",Page_Swap_Readback_time);
    Page_Swap_Readback_time++;

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1)) & 0x3f;

//    uint32_t vaddr0,vaddr1;
    uint32_t SD_addr0,SD_addr1;
    uint32_t entryhi,entrylo0,entrylo1; 
    // FIFO Swap

    // 1. Get item = Physical Memory Block form busy_mem_list.head
    pageframe_t *item0, *item1;
    item0 = (pageframe_t *)pmem_queue_dequeue(&busy_mem_list);
    item1 = (pageframe_t *)pmem_queue_dequeue(&busy_mem_list);

    current_running->page_table[((item0->vaddr)>>12)].ptentry = 0;
    current_running->page_table[((item1->vaddr)>>12)].ptentry = 0;

    SD_addr0 = SD_Base + SD_Page_Index * PAGE_SIZE;
    SD_Page_Index++;
    SD_addr1 = SD_Base + SD_Page_Index * PAGE_SIZE;
    SD_Page_Index++;

    current_running->page_table[((item0->vaddr)>>12)].ptentry = 0x1 | SD_addr0;
    current_running->page_table[((item1->vaddr)>>12)].ptentry = 0x1 | SD_addr1;
    
    // 2. Write the context of item into SD card
    // Write_SD_card();
    sdwrite((uint8_t *)(item0->vaddr & 0xfffff000),SD_addr0,8);
    sdwrite((uint8_t *)(item1->vaddr & 0xfffff000),SD_addr1,8);

    // 3. Paging the vaddr to item block
    uint32_t PPN0,PPN1;

    SD_addr0 = current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry & 0xffff000;
    SD_addr1 = current_running->page_table[(badvaddr >> 12) | 0x1].ptentry & 0xffff000;

    PPN0 = current_running->page_table[(badvaddr >> 12) & 0xfffffffe].ptentry = (item0->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;
    PPN1 = current_running->page_table[(badvaddr >> 12) | 0x1].ptentry = (item1->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;  

    pmem_queue_push(&busy_mem_list,item0);
    pmem_queue_push(&busy_mem_list,item1);

    // 4. Replace TLB entry
    entryhi = (((badvaddr >> 12) >> 1) << 13) | (current_running->pid & 0xff);
    entrylo0 = PPN0 >> 6;
    entrylo1 = PPN1 >> 6;

    SET_CP0_ENTRYHI(entryhi);
    SET_CP0_ENTRYLO0(entrylo0);
    SET_CP0_ENTRYLO1(entrylo1);
    asm volatile("tlbwi");

    // 5. Read Data Back To Memory from SD Card
    sdread((uint8_t *)(badvaddr & 0xffffe000),SD_addr0,8);
    sdread((uint8_t *)((badvaddr & 0xfffff000) | 0x1000),SD_addr1,8);   
}

void init_stack_space(uint32_t vaddr, pcb_t *pcb)
{
    uint32_t entryhi = (vaddr & 0xfffff000);
    SET_CP0_ENTRYHI(entryhi);
    asm volatile("tlbp");

    uint32_t index, entrylo0, entrylo1; 
    index = GET_CP0_INDEX();

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G) & 0x3f;

    if(free_mem_list.head != NULL)
    {
        pageframe_t *frame;
        frame = palloc();
        pcb->page_table[(vaddr >> 12) & 0xfffffffe].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;
        frame = palloc();
        pcb->page_table[(vaddr >> 12) | 0x1].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (IMM << 1) | PTEV;   

        uint32_t PPN0 = pcb->page_table[(vaddr >> 12) & 0xfffffffe].ptentry;
        uint32_t PPN1 = pcb->page_table[(vaddr >> 12) | 0x1].ptentry; 

        entryhi = (((vaddr >> 12) >> 1) << 13);
        entrylo0 = PPN0 >> 6;
        entrylo1 = PPN1 >> 6;
    }
    else
    {
        swap_page(vaddr);
    }

    if(index & 0x80000000)
    {
        //TLB refill
        SET_CP0_ENTRYHI(entryhi);
        SET_CP0_ENTRYLO0(entrylo0);
        SET_CP0_ENTRYLO1(entrylo1);
        asm volatile("tlbwr");  
    }
    else
    {
        //TLB invalid
        SET_CP0_ENTRYLO0(entrylo0);
        SET_CP0_ENTRYLO1(entrylo1);
        asm volatile("tlbwi"); 
    }    
}
