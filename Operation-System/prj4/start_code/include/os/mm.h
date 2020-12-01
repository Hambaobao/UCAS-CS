#ifndef INCLUDE_MM_H_
#define INCLUDE_MM_H_

#include "type.h"
#include "queue.h"

#define TLB_ENTRY_NUMBER 64

/* 16MB size of physical memory */
#define PAGE_SIZE 4096          // 4k
//#define NUMB_PAGE_FRAME 4096    // 4k
#define NUMB_PAGE_FRAME 6400    // 25*256

/* 8MB size of virtual memory */
//#define NUMB_VPAGE 4096 // 4k sum 4k * 4KB = 16MB
//#define NUMB_VPAGE 1048576 // from 0x0 - 0xffff ffff  sum 4GB
#define NUMB_VPAGE 524288 // from 0x0 - 0x7fff ffff  sum 2GB
//#define NUMB_VPAGE 262144 // from 0x0 - 0x3fff ffff  sum 1GB
//#define NUMB_VPAGE 131072 // from 0x0 - 0x1fff ffff  sum 512MB
//#define NUMB_VPAGE (1024 * 1024) // 1M sum 1M * 4KB = 4GB

// for speed TLB
#define PTE_G   1   // global   shift left 0 + 6
#define PTE_V   1   // valid    shift left 1 + 6
#define PTE_D   1   // dirty    shift left 2 + 6
#define PTE_C   2   // consist  shift left 3 + 6

// Denote PageTableEntry
#define PTEV    1   // Page Talbe Entry Valid   shift left 0
#define IMM     1   // Page in memory shift left 1

//PTE Flags // R = Reserved
// 31                              12 11 9  8   7   6  5             1    0
// +--------------20-----------------+----+---+---+---+----+-----+---+---+
// |       Page Table Index          |  C | D | V | G |    |     |IMM|PTEV
// +---------------------------------+----+---+---+---+---+--------------+

typedef struct pte {
    uint32_t ptentry;
} pte_t;

typedef struct pageframe
{
    uint32_t paddr;         // physical address
    uint32_t vaddr;         // virtual address
    struct pgframe *prev;
    struct pgframe *next;   
}pageframe_t;

// pte_t page_table[16][NUMB_VPAGE];
// pte_t static_page_table[256]; // used in task1
// pte_t page_table[NUMB_VPAGE]; // used in task2 & task3
pageframe_t physical_mem[NUMB_PAGE_FRAME];  // my used physical memory

queue_t free_mem_list;      // save free physical memory frames
queue_t busy_mem_list;      // save busy physical memory frames

extern unsigned long address[6];

extern uint32_t pmem_distribute_index;

extern uint32_t SD_Base;
extern uint32_t SD_Page_Index;

void do_TLB_Refill();
void do_page_fault(uint32_t badvaddr);

void free_physical_scope(uint32_t start, uint32_t end);

void fill_static_page_table();// only used in task1
void init_static_TLB(); // only used in task1

void init_TLB(); 

void swap_page();

void swap_page_readback(uint32_t PPN0);
void swap_page_writeinto(uint32_t PPN0);


#endif
