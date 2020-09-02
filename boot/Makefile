# boot loader的 Makefile

BOOT_OBJS := $(OBJDIR)/boot/boot.o $(OBJDIR)/boot/loader.o

$(OBJDIR)/boot/boot.o: boot/boot.S
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/boot/loader.o: boot/loader.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -Os -c $< -o $@

# 生成boot loader的elf可执行文件
$(OBJDIR)/boot/boot.elf: $(BOOT_OBJS)
	@echo + ld $@
	@mkdir -p $(@D)
	@$(LD) $(LDFLAGS) -N -e start -Ttext 0x7C00 -o $@ $^

# 现在我们要将elf转成flat binary格式,并反汇编一份boot loader（调试）
# 并且要再末尾写0xAA55
$(OBJDIR)/boot/boot: $(OBJDIR)/boot/boot.elf
	@echo + elf2bin $@
	@$(OBJDUMP) -S $< > $@.asm
	@$(OBJCOPY) -S -O binary -j .text $< $@
	@perl boot/sign.pl $(OBJDIR)/boot/boot

all: $(OBJDIR)/boot/boot