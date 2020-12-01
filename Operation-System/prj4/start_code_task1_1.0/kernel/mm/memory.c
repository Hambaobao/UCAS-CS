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
    for(; p < end && pmem_distribute_index < NUMB_PAGE_FRAME; p += PAGE_SIZE , pmem_distribute_index++)
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

void fill_static_page_table()
{
    // malloc virtual address from 0xff00 0000 - 0xff10 0000 (sum of 256 * 4KB = 1MB)
    // malloc virtual address from 0x0000 0000 - 0x0010 0000 (sum of 256 * 4KB = 1MB)
    pageframe_t *frame;
    int vaddr;
    int j;
    for(vaddr = 0x00000000, j = 0; j < 256; j++, vaddr += PAGE_SIZE)
    {
        frame = palloc();
        static_page_table[vaddr >> 12].ptentry = ((frame->paddr >> 12) << 12) & 0xfffff000;       
    }
}

// only used in task1
void init_static_TLB()
{
    // Static TLB only save virtual address from 0x0000 0000 - 0x0004 0000 (sum of 64 * 4KB = 0.25MB = 256KB)
    uint8_t ASID = 0;
    uint32_t entryhi, entrylo0, entrylo1; 

    SET_CP0_PAGEMASK(0x0);
    uint32_t vaddr;
    int i;
    for(vaddr = 0x00000000, i = 0; i < 64; i += 2, vaddr += (PAGE_SIZE * 2))
    {
        entryhi = (((vaddr >> 12) >> 1) << 13) | (ASID & 0xff);
        entrylo0 = ((static_page_table[i].ptentry >> 12) << 6) | (PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G;
        entrylo1 = ((static_page_table[i+1].ptentry >> 12) << 6) | (PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G;

        SET_CP0_ENTRYHI(entryhi);
        SET_CP0_ENTRYLO0(entrylo0);
        SET_CP0_ENTRYLO1(entrylo1);
        SET_CP0_INDEX(i/2);

        asm volatile ("tlbwi");
    }
}

// used in task2 & task3
void fill_page_table()
{
    // malloc virtual address from 0x0000 1000 - 0x0100 1000 (sum of 4096 * 4KB = 16MB)
    pageframe_t *frame;
    int vaddr;
    int j;

    int CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G) & 0x3f;
    // Paging 8MB first & On-Demand Paging others
    // 2K * 4K = 8MB
    for(vaddr = 0x00001000, j = 0; j < 2048; j++, vaddr += PAGE_SIZE)
    {
        frame = palloc();
        page_table[vaddr >> 12].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (PTEV << 5);       
    }

    for(; j < NUMB_VPAGE; j++, vaddr += PAGE_SIZE)
    {
        page_table[vaddr >> 12].ptentry = 0x0;       
    }
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
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G) & 0x3f;

    uint32_t entryhi,entrylo0,entrylo1,index,context,badvaddr; 

    entryhi  = GET_CP0_ENTRYHI();
    context  = GET_CP0_CONTEXT();
    badvaddr = GET_CP0_BADVADDR();

    uint32_t PPN0 = page_table[(badvaddr >> 12) & 0xfffffffe].ptentry;
    uint32_t PPN1 = page_table[(badvaddr >> 12) | 0x1].ptentry;  

//    context = context << 9;
    SET_CP0_ENTRYHI(entryhi | (0xff & current_running->pid));
    asm volatile("tlbp");
    index = GET_CP0_INDEX();

    if(index & 0x80000000)
    {
        // TLB refill
        if((PPN0 & 0x10) & (PPN0 & 0xf == current_running->pid))
        {
            entrylo0 = PPN0 >> 6;
            entrylo1 = PPN1 >> 6;
            SET_CP0_ENTRYLO0(entrylo0);
            SET_CP0_ENTRYLO1(entrylo1);
            asm volatile("tlbwr");        
        }
        else
        {
            do_page_fault(badvaddr);
        }
        
    }
    else
    { 
        //TLB invalid
        if((PPN0 & 0x10) & (PPN0 & 0xf == current_running->pid))
        {
            entrylo0 = PPN0 >> 6;
            entrylo1 = PPN1 >> 6;  
            SET_CP0_ENTRYLO0(entrylo0);
            SET_CP0_ENTRYLO1(entrylo1);
            asm volatile("tlbwi");        
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
    printk("-> Page Fault Exception %d      ",Page_Fault_time);
    Page_Fault_time++;

    uint32_t CDVG_Flags;
    CDVG_Flags = ((PTE_C << 3) | (PTE_D << 2) | (PTE_V << 1) | PTE_G) & 0x3f;

    uint32_t entryhi,entrylo0,entrylo1;  

    if(free_mem_list.head != NULL)
    {
        pageframe_t *frame;
        frame = palloc();
        page_table[(badvaddr >> 12) & 0xfffffffe].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (PTEV << 5) | (current_running->pid & 0x1f);
        frame = palloc();
        page_table[(badvaddr >> 12) | 0x1].ptentry = (frame->paddr & 0xfffff000) | (CDVG_Flags << 6) | (PTEV << 5) | (current_running->pid & 0x1f);   

        uint32_t PPN0 = page_table[(badvaddr >> 12) & 0xfffffffe].ptentry;
        uint32_t PPN1 = page_table[(badvaddr >> 12) | 0x1].ptentry; 

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

uint32_t Page_Swap_time = 0;
void swap_page(uint32_t badvaddr)
{
    vt100_move_cursor(0,0);
    printk("-> Exception: Swap Page %d              ",Page_Swap_time);
    Page_Swap_time++;

    // FIFO Swap

    // 1. Get item = Physical Memory Block form busy_mem_list.head
    pageframe_t *item;
    item = (pageframe_t *)pmem_queue_dequeue(&busy_mem_list);

    // 2. Write the context of item into SD card
    // Write_SD_card();
    
    // 3. Paging the vaddr to item block
    // 4. Replace TLB entry
}
