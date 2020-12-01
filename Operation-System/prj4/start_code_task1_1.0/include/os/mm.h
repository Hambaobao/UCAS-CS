#ifndef INCLUDE_MM_H_
#define INCLUDE_MM_H_

#include "type.h"
#include "queue.h"

#define TLB_ENTRY_NUMBER 64

/* 16MB size of physical memory */
#define PAGE_SIZE 4096          // 4k
#define NUMB_PAGE_FRAME 4096    // 4k
//#define NUMB_PAGE_FRAME 6400    // 25*256

/* 8MB size of virtual memory */
#define NUMB_VPAGE 4096 // 4k sum 4k * 4KB = 16MB
//#define NUMB_VPAGE (1024 * 1024) // 1M sum 1M * 4KB = 4GB

// for speed TLB
#define PTE_G   1   // global   shift left 0
#define PTE_V   1   // valid    shift left 1
#define PTE_D   1   // dirty    shift left 2
#define PTE_C   2   // consist  shift left 3

// Denote PageTableEntry
#define PTEV    1   // Page Talbe Entry Valid   shift left 5
//PTE Flags // R = Reserved
// 31                              12 11 9  8   7   6    5  4     0
// +--------------20-----------------+----+---+---+---+----+------+
// |       Page Table Index          |  C | D | V | G |PTEV| ASID
// +---------------------------------+----+---+---+---+---+------+

typedef struct pte {
    uint32_t ptentry;
} pte_t;

typedef struct pageframe
{
    uint32_t paddr;         //physical addr
    struct pgframe *prev;
    struct pgframe *next;   
}pageframe_t;

// pte_t page_table[16][NUMB_VPAGE];
pte_t static_page_table[256]; // used in task1
pte_t page_table[NUMB_VPAGE]; // used in task2 & task3
pageframe_t physical_mem[NUMB_PAGE_FRAME];  // my used physical memory

queue_t free_mem_list;      // save free physical memory frames
queue_t busy_mem_list;      // save busy physical memory frames

extern uint32_t pmem_distribute_index;

void do_TLB_Refill();
void do_page_fault(uint32_t badvaddr);

void free_physical_scope(uint32_t start, uint32_t end);

void fill_static_page_table();// only used in task1
void init_static_TLB(); // only used in task1

void init_TLB(); 

void swap_page();

#endif
