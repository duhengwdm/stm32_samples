
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding  -mcpu=cortex-m0

LOPS = -Wall -m32 -emit-llvm -target arm-none-eabi -mcpu=cortex-m0 -mthumb
LLCOPS = -march=thumb -mcpu=cortex-m0
#LLCOPS = -mcpu=cortex-m0
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
#OOPS = -std-compile-opts
OOPS = -std-link-opts

gcc : blinker05.gcc.thumb.flash.bin blinker05.gcc.thumb.sram.bin

all : gcc clang

clang : blinker05.clang.thumb.norm.flash.bin blinker05.clang.thumb.opt.flash.bin blinker05.clang.thumb.norm.sram.bin blinker05.clang.thumb.opt.sram.bin 

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s
	rm -f *.hex

#---------------------------------

flash.o : flash.s
	$(ARMGNU)-as $(AOPS) flash.s -o flash.o

sram.o : sram.s
	$(ARMGNU)-as $(AOPS) sram.s -o sram.o

blinker05.gcc.thumb.o : blinker05.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c blinker05.c -o blinker05.gcc.thumb.o

blinker05.gcc.thumb.flash.bin : flash.ld flash.o blinker05.gcc.thumb.o
	$(ARMGNU)-ld -o blinker05.gcc.thumb.flash.elf -T flash.ld flash.o blinker05.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker05.gcc.thumb.flash.elf > blinker05.gcc.thumb.flash.list
	$(ARMGNU)-objcopy blinker05.gcc.thumb.flash.elf blinker05.gcc.thumb.flash.bin -O binary

blinker05.gcc.thumb.sram.bin : sram.ld sram.o blinker05.gcc.thumb.o
	$(ARMGNU)-ld -o blinker05.gcc.thumb.sram.elf -T sram.ld sram.o blinker05.gcc.thumb.o
	$(ARMGNU)-objdump -D blinker05.gcc.thumb.sram.elf > blinker05.gcc.thumb.sram.list
	$(ARMGNU)-objcopy blinker05.gcc.thumb.sram.elf blinker05.gcc.thumb.sram.hex -O ihex
	$(ARMGNU)-objcopy blinker05.gcc.thumb.sram.elf blinker05.gcc.thumb.sram.bin -O binary

#---------------------------------

blinker05.clang.bc : blinker05.c
	clang $(LOPS) -c blinker05.c -o blinker05.clang.bc

blinker05.clang.thumb.norm.flash.bin : flash.ld flash.o blinker05.clang.bc
	#llc $(LLCOPS) blinker05.clang.bc -o blinker05.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) blinker05.clang.thumb.norm.s -o blinker05.clang.thumb.norm.o
	llc $(LLCOPS) blinker05.clang.bc -filetype=obj -o blinker05.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker05.clang.thumb.norm.flash.elf -T flash.ld flash.o blinker05.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker05.clang.thumb.norm.flash.elf > blinker05.clang.thumb.norm.flash.list
	$(ARMGNU)-objcopy blinker05.clang.thumb.norm.flash.elf blinker05.clang.thumb.norm.flash.bin -O binary

blinker05.clang.thumb.opt.flash.bin : flash.ld flash.o blinker05.clang.bc
	opt $(OOPS) blinker05.clang.bc -o blinker05.clang.thumb.opt.bc
	#llc $(LLCOPS) blinker05.clang.thumb.opt.bc -o blinker05.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) blinker05.clang.thumb.opt.s -o blinker05.clang.thumb.opt.o
	llc $(LLCOPS) blinker05.clang.thumb.opt.bc -filetype=obj -o blinker05.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker05.clang.thumb.opt.flash.elf -T flash.ld flash.o blinker05.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker05.clang.thumb.opt.flash.elf > blinker05.clang.thumb.opt.flash.list
	$(ARMGNU)-objcopy blinker05.clang.thumb.opt.flash.elf blinker05.clang.thumb.opt.flash.bin -O binary


blinker05.clang.thumb.norm.sram.bin : sram.ld sram.o blinker05.clang.bc
	#llc $(LLCOPS) blinker05.clang.bc -o blinker05.clang.thumb.norm.s
	#$(ARMGNU)-as $(AOPS) blinker05.clang.thumb.norm.s -o blinker05.clang.thumb.norm.o
	llc $(LLCOPS) blinker05.clang.bc -filetype=obj -o blinker05.clang.thumb.norm.o
	$(ARMGNU)-ld -o blinker05.clang.thumb.norm.sram.elf -T sram.ld sram.o blinker05.clang.thumb.norm.o
	$(ARMGNU)-objdump -D blinker05.clang.thumb.norm.sram.elf > blinker05.clang.thumb.norm.sram.list
	$(ARMGNU)-objcopy blinker05.clang.thumb.norm.sram.elf blinker05.clang.thumb.norm.sram.hex -O ihex
	$(ARMGNU)-objcopy blinker05.clang.thumb.norm.sram.elf blinker05.clang.thumb.norm.sram.bin -O binary

blinker05.clang.thumb.opt.sram.bin : sram.ld sram.o blinker05.clang.bc
	opt $(OOPS) blinker05.clang.bc -o blinker05.clang.thumb.opt.bc
	#llc $(LLCOPS) blinker05.clang.thumb.opt.bc -o blinker05.clang.thumb.opt.s
	#$(ARMGNU)-as $(AOPS) blinker05.clang.thumb.opt.s -o blinker05.clang.thumb.opt.o
	llc $(LLCOPS) blinker05.clang.thumb.opt.bc -filetype=obj -o blinker05.clang.thumb.opt.o
	$(ARMGNU)-ld -o blinker05.clang.thumb.opt.sram.elf -T sram.ld sram.o blinker05.clang.thumb.opt.o
	$(ARMGNU)-objdump -D blinker05.clang.thumb.opt.sram.elf > blinker05.clang.thumb.opt.sram.list
	$(ARMGNU)-objcopy blinker05.clang.thumb.opt.sram.elf blinker05.clang.thumb.opt.sram.hex -O ihex
	$(ARMGNU)-objcopy blinker05.clang.thumb.opt.sram.elf blinker05.clang.thumb.opt.sram.bin -O binary




