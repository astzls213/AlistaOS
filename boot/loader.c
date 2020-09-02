/*
 * 这是用于将内核从磁盘加载到内存，然后将控制权交给内核的模块
 * kernel是elf x86_64格式，磁盘默认是IDE类型
 * 否则还要判断磁盘是什么类型，还要额外写驱动程序
 * 这样Boot loader就会超出512Bytes
 * 就要实现 2 stage的boot loader，有点麻烦
 * 所以，有时间以后再扩展这里
 * 作者：暨南大学某帅b
*/
#include <alista/elf.h>

// 扇区大小
#define SECTSIZE 512

// IDE/ATA 驱动程序
static inline uint8_t inb(int);
static inline void insl(int, void*,int);
static inline void outb(int, uint8_t);
void waitdisk(void);
void read_sector(void*, uint32_t);

// 加载段函数
void load_seg(uint32_t, uint32_t, uint32_t);

// loader 负责加载内核
void
loader(void)
{
	// 两个指向程序头的指针，一个指向首个proghdr，一个指向末尾的末尾
	Proghdr *ph, *eph;
	Elf *elf_header = (Elf *) 0x10000;
	// read 1st page off disk
	load_seg((uint32_t) elf_header, 4096, 0);

	// 判断是否为ELF类型
	if (elf_header->e_magic != ELF_MAGIC)
		return;

	// 获取ph和eph的地址
	ph = (Proghdr *) ((uint8_t *) elf_header + elf_header->e_phoff);
	eph = ph + elf_header->e_phnum;

	// 加载所有段至合适内存地址
	for (; ph < eph; ph++)
		load_seg(ph->p_paddr, ph->p_memsz, ph->p_offset);

	// 跳转至kernel入口地址
	((void (*)(void)) (elf_header->e_entry))();
}

/* 读偏离sector1 offset个字节开始处，count个字节
 * 到物理地址pa处。由于我们对磁盘读的单位是扇区，所以
 * 读出来的数据要放在与扇区大小对齐的pa地址上，而pa不一定
 * 是对齐的，但不影响最终效果，因为
 * pa - 对齐了的pa == offset % SECTSIZE
*/ 
void
load_seg(uint32_t pa, uint32_t count, uint32_t offset)
{
	// 声明结束的物理地址
	uint32_t end_pa = pa + count;

	pa &= ~(SECTSIZE - 1);

	// 获取offset起始处所在扇区号
	offset = (offset / SECTSIZE) + 1;

	// 将第offset个扇区写到对齐了的pa
	while (pa < end_pa) {
		read_sector((uint8_t*) pa, offset);
		pa += SECTSIZE;
		offset++;
	}
}

/* 等待disk ready
 * Addr 0x1F7 is Status register.
 *  7	  6 	5	 4	  3	  2  	1 	  0
 * Busy Ready Fault Seek DRQ CORR IDDEX Error
*/
void
waitdisk(void)
{
	while ((inb(0x1F7) & 0xC0) != 0x40)
	    ;
}

// 读第offset个扇区至物理地址dst处
void
read_sector(void *dst, uint32_t offset)
{
	// 等待IDE磁盘不busy
	waitdisk();

	outb(0x1F2, 1);				// count = 1,代表只读一个扇区
	outb(0x1F3, offset);		// 0x1F2~0x1F6 是LBA寻址模式
	outb(0x1F4, offset >> 8);	// 更多细节可以看我的github仓库，有ATA's
	outb(0x1F5, offset >> 16);	// API介绍,at astzls213/MIT6.828-2017lab
	outb(0x1F6, (offset >> 24) | 0xE0);
	outb(0x1F7, 0x20);	// cmd 0x20 - read sectors

	// wait for disk to be ready
	waitdisk();

	// read a sector
	insl(0x1F0, dst, SECTSIZE/4);
}



/*
 *		
 *			内联汇编区域，此处定义的函数只在loader模块使用
 *			所以定义成 static。inline是优化调用速度。
 * 
 */

static inline uint8_t
inb(int port)
{
	uint8_t data;
	__asm__ volatile("inb %w1,%0" : "=a" (data) : "d" (port));
	return data;
}

static inline void
insl(int port, void *addr, int cnt)
{
	__asm__ volatile("cld\n\trepne\n\tinsl"
		     : "=D" (addr), "=c" (cnt)
		     : "d" (port), "0" (addr), "1" (cnt)
		     : "memory", "cc");
}

static inline void
outb(int port, uint8_t data)
{
	__asm__ volatile("outb %0,%w1" : : "a" (data), "d" (port));
}