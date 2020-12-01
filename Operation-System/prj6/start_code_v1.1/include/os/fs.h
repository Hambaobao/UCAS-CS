/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2019 University of Chinese Academic of Science, UCAS
 *               Author : Zhang Lei (Email : zhanglei171@mails.ucas.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       File System head files
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE. 
 * 
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * */

#ifndef INCLUDE_FS_H
#define INCLUDE_FS_H

#include "type.h"
    
#define O_RDWR  0x1     // Read & Write Model
#define O_RD    0x2     // Read Only Mode
#define O_WR    0x3     // Write Mode
#define D_RDWR  0x4     // Read & Write Model
#define D_RD    0x5     // Read Only Mode
#define D_WR    0x6     // Write Mode

#define FS_SIZE         (1 << 30)           // File System Size = 1GB
#define FS_START_ADDR   (1 << 20)           // Start At 512MB in SD Card    
#define BLOCK_SIZE      (0x1000)            // Block Size 4K Bytes
#define INODE_SIZE      (0x80)              // Inode Size 128 Bytes

#define BLOCKMAP_NUM    (8)                 // Block Map Number
#define INODEMAP_NUM    (8)                 // Inode MAP Number
#define INODE_NUM       (0x40000)           // Inode Number
#define DATABLOCK_NUM   (0x40000)           // Data Block Number, Sum 1GB

#define FS_MAGIC_NUMBER 0x88486666  // File System Magic Number
#define MAX_DIR_BLOCK   (26)        // MAX Number of Direct Blocks
#define MAX_DIRECTIONS  (64)        // MAX ENTRYS IN A DIRECTION

typedef struct superblock
{
    uint32_t fs_size;               // File System Size
    uint32_t start_addr;            // File System Start Address

    uint32_t blockmap_addr;         // BlockMap Start Address
    uint32_t blockmap_numb;         // BlockMap Number

    uint32_t inodemap_addr;         // InodeMap Start Address
    uint32_t inodemap_numb;         // InodeMap Number

    uint32_t inode_addr;            // Inode Start Address
    uint32_t inode_numb;            // Inode Number

    uint32_t datablock_addr;        // DataBlock Start Address
    uint32_t datablock_numb;        // DataBlock Number

    uint32_t magic_number;          // File System Magic Number
}superblock_t;

// sum 128 Bytes
typedef struct inode{
    uint32_t inode_id;                  // inode ID
    uint16_t mode;                      // File Mode
    uint16_t used_size;                 // Used Size
    uint32_t create_time;               // file create time
    uint32_t modify_time;               // last modify time
    uint32_t direct[MAX_DIR_BLOCK];     // Direct Blocks 
    uint32_t Indirect;                  // Indirect Blocks 
    uint32_t Double_Indirect;           // Double Indirect
} inode_t;

typedef struct dentry
{
    uint32_t inode_id;
    char fname[60];
}dentry_t;                              // denrty size = 128 Bytes

typedef struct C_dentry
{
    dentry_t Direction[64];           // size = 4K Bytes, up to 64 files
    char current_path[100];
    uint32_t inode_id;    
}C_dentry_t;

inode_t Current_Inode;                  
C_dentry_t Current_Direction;         

superblock_t superblock;            // SuperBlock
inode_t inode;                      // inode

extern uint32_t global_inode_id;

uint8_t ZERO_BUFF[4096];           // 4K Bytes 0
uint8_t inode_map_buff[4096];
inode_t inode_buff;
C_dentry_t Direction_buff;

// Direction Operations
int mkfs(void);
int mkdir(char *dir_name);
int rmdir(char *dir_name);
int read_dir(void);
int fs_info(void);
int enter_fs(char *path);

// File System Operations
int mknod(char *name);
int fcat(char *name);
int fopen(char *name, int access);
int fread(int fd, char *buff, int size);
int fwrite(int fd, char *buff, int size);
int fclose(int fd);
// extends
int frename(char *old_name, char *new_name);
int ffind(char *path, char *name);
int flink(char *target, char *linkname);
int fseek(int fd, uint32_t offset);

int do_print_info(void);

#endif