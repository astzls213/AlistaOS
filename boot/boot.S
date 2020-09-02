# 基于 x86 的 Boot loader
# boot.asm 的作用：
# 将CPU从实模式到32位保护模式
# 作者 - 暨南大学某帅b

.globl start
start:
    # Real mode at cs:ip -> 0000:7c00
    .code16
    cli
    cld

    xorw %ax, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %ss
    # 加载
    # 开启A20(通过Keyboard Controller)
    # 详见https://www.win.tue.nl/~aeb/linux/kbd/scancodes-11.html
enable_A20:
    call waitKB
    movb $0xd1, %al
    outb %al, $0x64
    call waitKB
    movb $0xdf, %al
    outb %al, $0x60
    jmp A20_OK
  
# 等待键盘端口可写子程序
waitKB:
    inb $0x64, %al
    testb $0x2, %al
    jnz waitKB
    ret

A20_OK:
    # 设置gdt
    lgdt gdtdesc
    # 假的进入保护模式
    movl %cr0, %eax
    orl $0x1, %eax
    movl %eax, %cr0
    # 真的进入保护模式
    ljmp $0x8, $prot
prot:
    .code32
    # 设置数据段选择子
    movw $0x10, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    # 设置好堆栈指针，进入C code
    movl    $start, %esp
    call loader
    # 异常，进入死循环，使用户手动重启
spin:
    jmp spin

# GDT相关初始化
# GDT entry 每个8字节，详情格式参见OSDev.org
.p2align 2     # 4 Bytes 对齐
gdTable:
    .quad 0x0                 # 空段(用于跳转前)
    .quad 0x00CF9A000000FFFF # 代码段
    .quad 0x00CF92000000FFFF # 数据段

gdtdesc:
    .word 0x17
    .long gdTable
