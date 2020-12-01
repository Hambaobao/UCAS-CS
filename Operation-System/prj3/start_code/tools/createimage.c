#include <assert.h>
#include <elf.h>
#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int boot_pad;
int kernel_pad;

int kernel_filesz;

void write_bootblock(FILE *image, FILE *bbfile, Elf32_Phdr *Phdr);
Elf32_Phdr *read_exec_file(FILE *opfile);
uint8_t count_kernel_sectors(Elf32_Phdr *Phdr);
void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz);

Elf32_Phdr *read_exec_file(FILE *opfile)
{	
	Elf32_Phdr *ph;
	Elf32_Ehdr *eh;

	ph = (Elf32_Phdr *)malloc(sizeof(Elf32_Phdr));
	eh = (Elf32_Ehdr *)malloc(sizeof(Elf32_Ehdr));

	fread(eh,sizeof(Elf32_Ehdr),1,opfile);

	fseek(opfile,(long)(eh->e_phoff),SEEK_SET); 

	fread(ph,sizeof(Elf32_Phdr),1,opfile);

	printf("e_phnum:%d\n",eh->e_phnum);

	return ph;	
}

uint8_t count_kernel_sectors(Elf32_Phdr *Phdr)
{
	int n = Phdr->p_memsz;
	return (n + 512 - 1) / 512;
}

void write_bootblock(FILE *image, FILE *file, Elf32_Phdr *phdr)
{
	char buf[520];
	Elf32_Off p_offset;
	Elf32_Word p_memsz;

	p_offset = phdr->p_offset;
	p_memsz = phdr->p_memsz;

	boot_pad = ((p_memsz + 512 - 1) / 512) * 512;
	
	fseek(file,(long)(p_offset),SEEK_SET);
	fread(buf,(int)(p_memsz),1,file);
	fwrite(buf,(int)(p_memsz),1,image);

	return;
//	fseek(image,512L,SEEK_SET);
}


void write_kernel(FILE *image, FILE *knfile, Elf32_Phdr *Phdr, int kernelsz)
{
	char buf;
	int a = 0;

	Elf32_Off p_offset;
	Elf32_Word p_memsz;
	Elf32_Word p_filesz;

	p_offset = Phdr->p_offset;
	p_memsz = Phdr->p_memsz;
	p_filesz = Phdr->p_filesz;

	int sz = p_filesz;

	kernel_pad = ((p_memsz + 512 - 1) / 512) * 512;

	fseek(knfile,(long)(p_offset),SEEK_SET);	

	int i,j;
	j = kernel_pad - p_filesz;

	for(i = 0;i < sz;i++)
	{
		fread(&buf,1,1,knfile);
		fwrite(&buf,1,1,image);
	}
	for(i = 0;i < j;i++)
	{
		fwrite(&a,1,1,image);
	}

	return;
//	fseek(image,1024L,SEEK_SET);
}

void record_kernel_sectors(FILE *image, uint8_t kernelsz)
{	
	uint8_t *p;
	p = &kernelsz;

	fseek(image,504,SEEK_SET);
	fwrite(p,sizeof(uint8_t),1,image);

	int *end;
	int e = 0xaa550000;
	end = &e; 

	fseek(image,508,SEEK_SET);
	fwrite(end,4,1,image);	
}

void extent_opt(Elf32_Phdr *Phdr_bb, Elf32_Phdr *Phdr_k, int kernelsz)
{
//	unsigned int b_addr,k_addr;
	
//	b_addr = Phdr_bb->p_vaddr;
//	k_addr = Phdr_k->p_vaddr;

	printf("0x%x:bootblock\n",(*Phdr_bb).p_vaddr);
	printf("\tsegment %d\n",0);
	printf("\t\toffset 0x%x\tvaddr 0x%x\n",(*Phdr_bb).p_offset,(*Phdr_bb).p_vaddr);
	printf("\t\tfilesize 0x%x\tmemsz 0x%x\n",(*Phdr_bb).p_filesz,(*Phdr_bb).p_memsz);
	printf("\t\twriting 0x%x bytes\n",(*Phdr_bb).p_filesz);
	printf("\t\tpadding up to 0x%x\n",boot_pad);
	
	printf("0x%x:kernel\n",(*Phdr_k).p_vaddr);
	printf("\tsegment %d\n",0);
	printf("\t\toffset 0x%x\tvaddr 0x%x\n",(*Phdr_k).p_offset,(*Phdr_k).p_vaddr);
	printf("\t\tfilesize 0x%x\tmemsz 0x%x\n",(*Phdr_k).p_filesz,(*Phdr_k).p_memsz);
	printf("\t\twriting 0x%x bytes\n",(*Phdr_k).p_filesz);
	printf("\t\tpadding up to 0x%x\n",kernel_pad);

	printf("-----------------my createimage---------------\n");
}


int main()
{	FILE *b_file;
	FILE *k_file;
	FILE *m_file;
	
	Elf32_Phdr *b_ph;
	Elf32_Phdr *k_ph;

	uint8_t k_sector_cnt;	

	b_file = fopen("bootblock","rb");
	k_file = fopen("main","rb");
	m_file = fopen("image","wb");	

	b_ph = read_exec_file(b_file);
	k_ph = read_exec_file(k_file);


	fseek(m_file,0x0L,SEEK_SET);

	k_sector_cnt = count_kernel_sectors(k_ph);

	write_bootblock(m_file,b_file,b_ph);

	fseek(m_file,0x200L,SEEK_SET);		

	write_kernel(m_file,k_file,k_ph,k_sector_cnt);		

	record_kernel_sectors(m_file,k_sector_cnt);

	extent_opt(b_ph,k_ph,k_sector_cnt);

	fclose(b_file);
	fclose(k_file);
	fclose(m_file);

	printf("os_size:%d\n",k_sector_cnt);

	return 0;
}
