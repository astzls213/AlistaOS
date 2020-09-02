# 定义一些路径变量
OBJDIR := obj
INC_PATH = ./include


# x86的qemu
QEMU := qemu-system-i386

# 交叉编译工具链
CC := x86_64-elf-gcc -pipe
AS := x86_64-elf-as
AR := x86_64-elf-ar
LD := x86_64-elf-ld
OBJCOPY := x86_64-elf-objcopy
OBJDUMP := x86_64-elf-objdump
NM := x86_64-elf-nm


# gcc选项：
CFLAGS := -O1 				# 优化力度
CFLAGS += -fno-builtin 		# 不使用C语言内建函数(防止函数定义重名)
CFLAGS += -I $(INC_PATH)	# 头文件搜索路径
CFLAGS += -std=gnu99		# C语言规范版本
CFLAGS += -nostdinc			# 不在默认路径搜素头文件
CFLAGS += -static			# 静态链接
CFLAGS += -m32				# 32位机器码
CFLAGS += -Wall -Wno-format -Wno-unused -Werror	# 不忽视警告
# -Wno-address-of-packed-member
#CFLAGS += -fno-tree-ch

# ld选项
LDFLAGS := -m elf_i386		# 可执行文件格式为 x86 elf


all:

.SUFFIXES:
.DELETE_ON_ERROR:


# 粘贴所有子目录的Makefile到这里
include boot/Makefile

# 用于qemu的cpu核心数
CPUS ?= 1

qemu:
	@$(QEMU) -hda obj/boot/boot

qemu-nox:
	@$(QEMU) -nographic -hda obj/boot/boot

qemu-gdb:
	@$(QEMU) -s -S -hda obj/boot/boot

# 清除目标文件
clean:
	@echo - rm obj
	@rm -rf obj