#include "mm.h"
#include "sched.h"

// pcb_t *current_running;
// pcb_t pcb[NUM_MAX_TASK];
//TODO:Finish memory management functions here refer to mm.h and add any functions you need.

uint32_t pmem_distribute_index = 0;
void free_physical_scope(uint32_t start, uint32_t end)
{

}

pageframe_t *palloc()
{

}

void pfree(pageframe_t *item)
{

}

// used in task2 & task3
void init_TLB()
{
 
}

uint32_t TLB_miss_time = 0;
void do_TLB_Refill()
{

}

uint32_t Page_Fault_time = 0;
void do_page_fault(uint32_t badvaddr)
{

}

uint32_t SD_Base = 0x40000000;
uint32_t SD_Page_Index = 0;
uint32_t Page_Swap_time = 0;
void swap_page(uint32_t badvaddr)
{

}

void swap_page_readback(uint32_t badvaddr)
{
  
}

void init_stack_space(uint32_t vaddr, pcb_t *pcb)
{

}
