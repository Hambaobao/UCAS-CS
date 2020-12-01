/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *            Copyright (C) 2019 University of Chinese Academic of Science, UCAS
 *               Author : Zhang Lei (Email : zhanglei171@mails.ucas.ac.cn)
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * * * * *
 *                       File System .C files
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

#include "type.h"
#include "fs.h"

uint32_t root_inode_addr;
uint32_t root_direction_addr;

uint32_t write_dest, write_src, write_size;
uint32_t read_dest, read_src, read_size;

int do_print_info(void)
{
    vt100_move_cursor(1,1);    
    printk("Current inode = %d", Current_Direction.inode_id);
}

void init_superblock(void)
{
    superblock.fs_size          = FS_SIZE;
    superblock.start_addr       = FS_START_ADDR;

    superblock.blockmap_addr    = FS_START_ADDR + BLOCK_SIZE;
    superblock.blockmap_numb    = BLOCKMAP_NUM;

    superblock.inodemap_addr    = FS_START_ADDR + (1 + BLOCKMAP_NUM)* BLOCK_SIZE;
    superblock.inodemap_numb    = INODEMAP_NUM;  

    superblock.inode_addr       = FS_START_ADDR + (1 + BLOCKMAP_NUM + INODEMAP_NUM) * BLOCK_SIZE;
    superblock.inode_numb       = INODE_NUM;

    superblock.datablock_addr   = FS_START_ADDR + (1 + BLOCKMAP_NUM + INODEMAP_NUM) * BLOCK_SIZE + INODE_NUM * INODE_SIZE;    
    superblock.datablock_numb   = DATABLOCK_NUM;

    superblock.magic_number     = FS_MAGIC_NUMBER;

    sdwrite(&superblock, FS_START_ADDR, sizeof(superblock_t));
    sdwrite(&superblock, FS_START_ADDR + 512, sizeof(superblock_t));    // Back SuperBlock

    uint32_t zero_start;
    zero_start = superblock.start_addr + 1024;                          // 2 * 512 for 2 superblocks

    sdwrite(&ZERO_BUFF, zero_start, 4096 - 1024);
}

void init_blockmap(void)
{
    uint32_t i;
    uint32_t zero_start = superblock.blockmap_addr;

    for(i = 0; i < BLOCKMAP_NUM; i++)
        sdwrite(&ZERO_BUFF, zero_start + i * BLOCK_SIZE, 4096);
}

void init_inodemap(void)
{
    uint32_t i;
    uint32_t zero_start = superblock.inodemap_addr;

    for(i = 0; i < INODEMAP_NUM; i++)
        sdwrite(&ZERO_BUFF, zero_start + i * BLOCK_SIZE, 4096);
}

void init_inode(void)
{
//      TODO NOTHING    
//    uint32_t i;
//    uint32_t zero_start = superblock.inode_addr;

//    for(i = 0; i < INODE_NUM; i++)
//        sdwrite(&ZERO_BUFF, zero_start + i * BLOCK_SIZE, 4096);
}

int search_inode_map()
{
    int i,j,k;
    for(i = 0; i < INODEMAP_NUM; i++)
    {
        uint32_t inode_map_addr = superblock.inodemap_addr + i * BLOCK_SIZE;
        sdread(&inode_map_buff, inode_map_addr, 4096);
        for(j = 0; j < 4096; j++)
            if(inode_map_buff[j] != 0xff)
            {
                uint8_t temp = inode_map_buff[j];
                for(k = 0; k < 8 ; k++)
                {
                    if(!((temp >> k) & 0x1))
                    {
                        inode_map_buff[j] |= (1 << k);
                        sdwrite(&inode_map_buff, inode_map_addr, 4096);                        
                        return i * BLOCK_SIZE * 8 + (j * 8) + k;
                    }
                }                                            
            }
    }
    return -1;  
}

int get_root_direction(void)
{
    root_inode_addr = superblock.inode_addr;
    root_direction_addr = superblock.datablock_addr;

    sdread(&Current_Inode,root_inode_addr,sizeof(inode_t));
    sdread(&Current_Direction,root_direction_addr,sizeof(C_dentry_t));
}

int init_root_direction(void)
{
    // Create Root Dir
    
    uint32_t inode_numb = search_inode_map();
    uint32_t inode_addr = superblock.inode_addr + inode_numb * INODE_SIZE;
    uint32_t data_addr = superblock.datablock_addr + inode_numb * BLOCK_SIZE;
    
    if(inode_numb == -1)
    {
        printk(">[ERROR] No Enough Inode Space       \n");
        screen_reflush();
        return 0;          
    }

    inode_buff.create_time = 0;
    inode_buff.modify_time = 0;
    inode_buff.mode = D_RDWR;
    inode_buff.used_size = 0;
    inode_buff.Indirect = 0;
    inode_buff.Double_Indirect = 0;
    inode_buff.inode_id = inode_numb;

    int i,j;    
    for(i = 0; i < MAX_DIR_BLOCK; i++)
        inode_buff.direct[i] = 0;

    sdwrite(&inode_buff, inode_addr, sizeof(inode_t));

    Direction_buff.inode_id = inode_numb;

    strcpy(Direction_buff.Direction[0].fname, ".");
    Direction_buff.Direction[0].inode_id = Direction_buff.inode_id;

    strcpy(Direction_buff.Direction[1].fname, "..");        
    Direction_buff.Direction[1].inode_id = Direction_buff.inode_id;

    for(i = 2; i < MAX_DIRECTIONS; i++)
        strcpy(Direction_buff.Direction[i].fname, "\0");

    strcpy(Direction_buff.current_path, "/");         

    sdwrite(&Direction_buff, data_addr, sizeof(C_dentry_t));
    sdread(&Current_Direction, data_addr, sizeof(C_dentry_t));
//    memcpy(&Current_Direction, &Direction_buff, sizeof(C_dentry_t));           
}

void print_superblock_info(void)
{
    uint32_t block_map_offset, inode_map_offset, inode_offset, data_offset;
    block_map_offset    = 1;
    inode_map_offset    = block_map_offset + superblock.blockmap_numb;
    inode_offset        = inode_map_offset + superblock.inodemap_numb;
    data_offset         = inode_offset + superblock.inode_numb;

    my_printf("     magic : 0x%x                                 \n", superblock.magic_number);
    my_printf("     num sector : %d, start sector : %d           \n", BLOCK_SIZE/512, superblock.start_addr/512);
    my_printf("     block map offset : %d (%d)                   \n", block_map_offset, superblock.blockmap_numb);
    my_printf("     inode map offset : %d (%d)                   \n", inode_map_offset, superblock.inodemap_numb);
    my_printf("     inode offset : %d (%d)                       \n", inode_offset, superblock.inode_numb);
    my_printf("     data offset : %d (%d)                        \n", data_offset, superblock.datablock_numb);
    my_printf("     inode entry size : %dB, dir entry size : %dB \n",32, 32);
    screen_reflush();
}

// Direction Operations
int mkfs(void)
{
    // Create FS
    my_printf("[FS] Setting File System!                  \n");
    screen_reflush();

    my_printf("[FS] Setting SuperBlock...                 \n");
    screen_reflush();
    init_superblock();          // 1. Init SuperBlock
    print_superblock_info();

    my_printf("[FS] Setting Block_Map...                  \n");
    screen_reflush();
    init_blockmap();            // 2. Init Block Map

    my_printf("[FS] Setting Inode_Map...                  \n");
    screen_reflush();          	 
    init_inodemap();            // 3. Init Inode Map

    my_printf("[FS] Setting Inode...                      \n");
    screen_reflush();      
    init_inode();               // 4. Init Inode

    my_printf("[FS] Setting Root Direction	...           \n");	
    init_root_direction();   // 5. Current Direction

    my_printf("[FS] File System Initialize Succeeded...   \n");
    screen_reflush();    
}


int mkdir(char *dir_name)
{
    // Create Dir
    int i;
    for(i = 0; i < 64; i++)
        if(!strcmp(dir_name,Current_Direction.Direction[i].fname))
        {
            my_printf(">[ERROR] Direction has existed!       \n");
            screen_reflush();
            return 0;  
        }

    int j;
    for(j = 2; j < MAX_DIRECTIONS; j++)
        if(Current_Direction.Direction[j].fname[0] == '\0')
            break;
    if(j == MAX_DIRECTIONS)
    {
        my_printf(">[ERROR] No Enough Direction Space       \n");
        screen_reflush();        
        return 0;
    }

    uint32_t inode_numb = search_inode_map();
    uint32_t inode_addr = superblock.inode_addr + inode_numb * INODE_SIZE;
    uint32_t data_addr = superblock.datablock_addr + inode_numb * BLOCK_SIZE;
    my_printf("inode_numb = %d\n",inode_numb);
    screen_reflush();

    if(inode_numb == -1)
    {
        my_printf(">[ERROR] No Enough Inode Space       \n");
        screen_reflush();
        return 0;          
    }

    inode_buff.create_time = 0;
    inode_buff.modify_time = 0;
    inode_buff.mode = D_RDWR;
    inode_buff.used_size = 0;
    inode_buff.Indirect = 0;
    inode_buff.Double_Indirect = 0;
    inode_buff.inode_id = inode_numb;
    for(i = 0; i < MAX_DIR_BLOCK; i++)
        inode_buff.direct[i] = 0;

    sdwrite(&inode_buff, inode_addr, sizeof(inode_t));

    Direction_buff.inode_id = inode_numb;

    strcpy(Direction_buff.Direction[0].fname, ".");
    Direction_buff.Direction[0].inode_id = Direction_buff.inode_id;

    strcpy(Direction_buff.Direction[1].fname, "..");        
    Direction_buff.Direction[1].inode_id = Current_Direction.inode_id;

    my_printf("D[0].fname = %s, D[1].fname = %s\n",Direction_buff.Direction[0].fname,Direction_buff.Direction[1].fname);
    screen_reflush();

    for(i = 2; i < MAX_DIRECTIONS; i++)
        strcpy(Direction_buff.Direction[i].fname, "\0");

    if(!strcmp(Current_Direction.current_path,"/"))
    {   
        strcpy(Direction_buff.current_path, Current_Direction.current_path);         
        strcpy(Direction_buff.current_path + strlen(Direction_buff.current_path), dir_name);
    }
    else
    {
        strcpy(Direction_buff.current_path, Current_Direction.current_path);
        strcpy(Direction_buff.current_path + strlen(Direction_buff.current_path), "/");                 
        strcpy(Direction_buff.current_path + strlen(Direction_buff.current_path), dir_name);
    }
       
    sdwrite(&Direction_buff, data_addr, sizeof(C_dentry_t));

    my_printf("DB[0].id = %d, DB[0].f = %s     \n",Direction_buff.Direction[0].inode_id, Direction_buff.Direction[0].fname);
    my_printf("DB[1].id = %d, DB[1].f = %s     \n",Direction_buff.Direction[1].inode_id, Direction_buff.Direction[1].fname);
    screen_reflush();

    strcpy(Current_Direction.Direction[j].fname, dir_name);
    Current_Direction.Direction[j].inode_id = Direction_buff.inode_id;

    my_printf("j = %d , C[j].fname = %s\n", j, Current_Direction.Direction[j].fname);
    screen_reflush();

//    uint32_t Current_inode_addr = superblock.inode_addr + Current_Direction.inode_id * INODE_SIZE;
    uint32_t Current_data_addr = superblock.datablock_addr + Current_Direction.inode_id * BLOCK_SIZE;

//    sdwrite(&Current_Inode, inode_addr, sizeof(inode_t));
    sdwrite(&Current_Direction, Current_data_addr, sizeof(C_dentry_t));               
}

int rmdir(char *dir_name)
{
    // Remove file
    vt100_move_cursor(1, 1);
    printk("rmdir, dir_name = %s                 ",dir_name);
}

int read_dir(void)
{
    // ls
    int i;
    for(i = 0; i < MAX_DIRECTIONS; i++)
    {
        if(strcmp(Current_Direction.Direction[i].fname,"\0"))
            my_printf("%s   ",Current_Direction.Direction[i].fname);
    }
    my_printf("\n");
}

int fs_info(void)
{
    // statfs
    print_superblock_info();
}

int enter_fs(char *path)
{
    // cd
    uint32_t inode_numb = Current_Direction.inode_id;
    uint32_t inode_addr = superblock.inode_addr + inode_numb * INODE_SIZE;
    uint32_t data_addr  = superblock.datablock_addr + inode_numb * BLOCK_SIZE;

    sdwrite(&Current_Inode, inode_addr, sizeof(inode_t));

    write_dest = data_addr;
    write_src = &Current_Direction;
    write_size = sizeof(C_dentry_t);
//    my_printf("w_src = 0x%x, w_dest = 0x%x, w_size = 0x%x\n",write_src,write_dest,write_size);
//    screen_reflush();      
//    sdwrite(write_src, write_dest, write_size);
    sdwrite(&Current_Direction, data_addr, sizeof(C_dentry_t));

    int i;
    for(i = 0; i < MAX_DIRECTIONS; i++)
    {
        if(!strcmp(path,Current_Direction.Direction[i].fname))
        {
            inode_numb = Current_Direction.Direction[i].inode_id;
            inode_addr = superblock.inode_addr + inode_numb * INODE_SIZE;
            data_addr  = superblock.datablock_addr + inode_numb * BLOCK_SIZE;

            my_printf("inode_numb = %d             \n",inode_numb);
            screen_reflush();   

            sdread(&Current_Inode, inode_addr, sizeof(inode_t));

            read_dest = &Current_Direction;
            read_src = data_addr;
            read_size = sizeof(C_dentry_t);
//            my_printf("r_src = 0x%x, r_dest = 0x%x, r_size = 0x%x\n",read_dest,read_src,read_size);
//            screen_reflush();    
            sdread(&Current_Direction, data_addr, sizeof(C_dentry_t));

            my_printf("CD_D[0].id = %d, CD_D[0].f = %s     \n",Current_Direction.Direction[0].inode_id, Current_Direction.Direction[0].fname);
            my_printf("CD_D[1].id = %d, CD_D[1].f = %s     \n",Current_Direction.Direction[1].inode_id, Current_Direction.Direction[1].fname);

            my_printf("Current_inode_numb = %d             \n",Current_Direction.inode_id);
            screen_reflush();

            break;  
        }
    }
}



// File System Operations
int mknod(char *fname)
{
    // touch
    vt100_move_cursor(1, 1);
    printk("touch, fname = %s                    ",fname);
}

int fcat(char *fname)
{
    // cat
    vt100_move_cursor(1, 1);
    printk("cat, fname = %s                      ",fname);
}

int fopen(char *fname, int access)
{
    // open file
    vt100_move_cursor(1, 1);
    printk("fopen, fname = %s, access = %d       ",fname,access);
}

int fread(int fd, char *buff, int size)
{
    // read file
    vt100_move_cursor(1, 1);
    printk("fread                                ");
}

int fwrite(int fd, char *buff, int size)
{
    // write file
    vt100_move_cursor(1, 1);
    printk("fwrite                                ");
}

int fclose(int fd)
{
    // close file
        vt100_move_cursor(1, 1);
    printk("fclose                                ");
}

int frename(char *old_fname, char *new_fname)
{
    // rename file
    vt100_move_cursor(1, 1);
    printk("frename                                ");
}

int ffind(char *path, char *fname)
{
    // find file
    vt100_move_cursor(1, 1);
    printk("ffind                                ");
}

int flink(char *target, char *linkname)
{
    // link file
    vt100_move_cursor(1, 1);
    printk("flink                                ");
}

int fseek(int fd, uint32_t offset)
{
    // seek file
    vt100_move_cursor(1, 1);
    printk("fseek                                ");
}