# architecture: mips, riscv or riscve
ARCH=riscve

# board specific parameters
BAUDRATE=57600
F_CLK=25000000
SERIAL_DEV=/dev/ttyUSB0

# compiler flags
CFLAGS_STRIP = -fdata-sections -ffunction-sections
LDFLAGS_STRIP = --gc-sections
LD_SCRIPT = hf-risc.ld

CFLAGS_NO_HW_MULDIV = -mnohwmult -mnohwdiv -ffixed-lo -ffixed-hi
#GCC_mips = mips-elf-gcc -Wall -Os -c -mips2 -mno-branch-likely -mpatfree -mfix-r4000 -mno-check-zero-division -msoft-float -ffreestanding -nostdlib -fomit-frame-pointer -G 0 -I ./include -DCPU_SPEED=${F_CLK} -DBIG_ENDIAN $(CFLAGS_NO_HW_MULDIV) $(CFLAGS_STRIP) #-DDEBUG_PORT -fno-delayed-branch
#GCC_mips = mips-elf-gcc -Wall -Os -c -mips1 -fno-delayed-branch -ffixed-hi -ffixed-lo -mno-check-zero-division -msoft-float -ffreestanding -nostdlib -fomit-frame-pointer -G 0 -I ./include -DCPU_SPEED=${F_CLK} -DBIG_ENDIAN $(CFLAGS_STRIP) #-DDEBUG_PORT
GCC_mips = mips-elf-gcc -Wall -O2 -c -mips1 -fno-delayed-branch -mno-check-zero-division -msoft-float -ffreestanding -nostdlib -fomit-frame-pointer -G 0 -I ./include -DCPU_SPEED=${F_CLK} -DBIG_ENDIAN $(CFLAGS_STRIP) #-DDEBUG_PORT
AS_mips = mips-elf-as -mips1 -msoft-float
LD_mips = mips-elf-ld -mips1 $(LDFLAGS_STRIP)
DUMP_mips = mips-elf-objdump
READ_mips = mips-elf-readelf
OBJ_mips = mips-elf-objcopy
SIZE_mips = mips-elf-size

GCC_riscv = riscv64-elf-gcc -march=rv32i -mabi=ilp32 -Wall -O2 -c -mstrict-align -ffreestanding -nostdlib -ffixed-s11 -I ./include -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) #-DDEBUG_PORT #-ffixed-s10 -ffixed-s11 -mno-div -mrvc -fPIC #-DDEBUG_PORT
AS_riscv = riscv64-elf-as -march=rv32i -mabi=ilp32 #-fPIC
LD_riscv = riscv64-elf-ld -melf32lriscv $(LDFLAGS_STRIP)
DUMP_riscv = riscv64-elf-objdump -Mno-aliases
READ_riscv = riscv64-elf-readelf
OBJ_riscv = riscv64-elf-objcopy
SIZE_riscv = riscv64-elf-size

GCC_riscve = riscv64-elf-gcc -march=rv32e -mabi=ilp32e -Wall -O2 -c -mstrict-align -ffreestanding -nostdlib -ffixed-a5 -I ./include -DCPU_SPEED=${F_CLK} -DLITTLE_ENDIAN $(CFLAGS_STRIP) #-DDEBUG_PORT 
AS_riscve = riscv64-elf-as -march=rv32e -mabi=ilp32e #-fPIC
LD_riscve = riscv64-elf-ld -melf32lriscv $(LDFLAGS_STRIP)
DUMP_riscve = riscv64-elf-objdump -Mno-aliases
READ_riscve = riscv64-elf-readelf
OBJ_riscve = riscv64-elf-objcopy
SIZE_riscve = riscv64-elf-size

#CFLAGS  = -march=rv32im -mabi=ilp32 -Os -fdata-sections -ffunction-sections -flto -Wall -std=c11 -I$(INCDIR) -I$(INCMEMPHIS) -I$(INCMUTILS) -I$(HALDIR)
#LDFLAGS = --specs=nano.specs -march=rv32im -mabi=ilp32 -nostartfiles -Wl,--section-start=".init"=0,--gc-sections,-flto -L../libmutils -lmutils

all:

serial:
	stty ${BAUDRATE} raw cs8 -parenb -crtscts clocal cread ignpar ignbrk -ixon -ixoff -ixany -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke -F ${SERIAL_DEV}

load: serial
	echo "u" > ${SERIAL_DEV}
	sleep 1
	cat code.bin > ${SERIAL_DEV}
	sleep 5
	echo "b" > ${SERIAL_DEV}

debug: serial
	cat ${SERIAL_DEV}

crt:
	$(AS_$(ARCH)) -o crt0.o lib/$(ARCH)/crt0.s
	$(GCC_$(ARCH)) -o libc.o lib/libc.c
	$(GCC_$(ARCH)) -o interrupt.o lib/interrupt.c
	$(GCC_$(ARCH)) -o math.o lib/math.c
	$(GCC_$(ARCH)) -o malloc.o lib/malloc.c

link:
	$(LD_$(ARCH)) -Tlib/$(ARCH)/$(LD_SCRIPT) -Map test.map -N -o test.elf *.o
	$(DUMP_$(ARCH)) --disassemble --reloc test.elf > test.lst
	$(DUMP_$(ARCH)) -h test.elf > test.sec
	$(DUMP_$(ARCH)) -s test.elf > test.cnt
	$(OBJ_$(ARCH)) -O binary test.elf test.bin
	$(SIZE_$(ARCH)) test.elf
	mv test.elf code.elf
	mv test.bin code.bin
	mv test.lst code.lst
	mv test.sec code.sec
	mv test.cnt code.cnt
	mv test.map code.map
	hexdump -v -e '4/1 "%02x" "\n"' code.bin > code.txt

monitor:
	$(AS_$(ARCH)) -o boot_rom.o lib/$(ARCH)/boot_rom.s
	$(GCC_$(ARCH)) -o libc.o lib/libc.c
	$(GCC_$(ARCH)) -o math.o lib/math.c
		$(GCC_$(ARCH)) -o monitor.o boot/monitor.c
	$(LD_$(ARCH)) -Tlib/$(ARCH)/hf-risc_bootloader.ld -Map test.map -N -o test.elf \
		boot_rom.o libc.o monitor.o
	$(DUMP_$(ARCH)) --disassemble --reloc test.elf > test.lst
	$(DUMP_$(ARCH)) -h test.elf > test.sec
	$(DUMP_$(ARCH)) -s test.elf > test.cnt
	$(OBJ_$(ARCH)) -O binary test.elf test.bin --pad-to 4096
	$(SIZE_$(ARCH)) test.elf
	mv test.elf code.elf
	mv test.bin code.bin
	mv test.lst code.lst
	mv test.sec code.sec
	mv test.cnt code.cnt
	mv test.map code.map
	hexdump -v -e '4/1 "%02x" "\n"' code.bin > boot.txt
	xxd -b -c 4 code.bin | awk '{print $$2 $$3 $$4 $$5}' > boot_bin.txt
	./../tools/xilinx/ram_image ./../tools/xilinx/ram_xilinx.vhd boot.txt boot_ram.vhd
	
monitor_uart_axi:
	$(AS_$(ARCH)) -o boot_rom.o lib/$(ARCH)/boot_rom.s
	$(GCC_$(ARCH)) -o libc.o lib/libc.c
	$(GCC_$(ARCH)) -o math.o lib/math.c
		$(GCC_$(ARCH)) -o monitor_uart_axi.o boot/monitor_uart_axi.c
	$(LD_$(ARCH)) -Tlib/$(ARCH)/hf-risc_bootloader.ld -Map test.map -N -o test.elf \
		boot_rom.o libc.o monitor_uart_axi.o
	$(DUMP_$(ARCH)) --disassemble --reloc test.elf > test.lst
	$(DUMP_$(ARCH)) -h test.elf > test.sec
	$(DUMP_$(ARCH)) -s test.elf > test.cnt
	$(OBJ_$(ARCH)) -O binary test.elf test.bin --pad-to 4096
	$(SIZE_$(ARCH)) test.elf
	mv test.elf code.elf
	mv test.bin code.bin
	mv test.lst code.lst
	mv test.sec code.sec
	mv test.cnt code.cnt
	mv test.map code.map
	hexdump -v -e '4/1 "%02x" "\n"' code.bin > boot.txt
	xxd -b -c 4 code.bin | awk '{print $$2 $$3 $$4 $$5}' > boot_bin.txt
	./../tools/xilinx/ram_image ./../tools/xilinx/ram_xilinx.vhd boot.txt boot_ram.vhd

boot_sim:
	$(AS_$(ARCH)) -o boot_rom.o lib/$(ARCH)/boot_rom.s
	$(GCC_$(ARCH)) -o libc.o lib/libc.c
	$(GCC_$(ARCH)) -o math.o lib/math.c
	$(GCC_$(ARCH)) -o bootloader_sim.o boot/bootloader_sim.c
	$(LD_$(ARCH)) -Tlib/$(ARCH)/hf-risc_bootloader.ld -Map test.map -N -o test.elf \
		boot_rom.o libc.o bootloader_sim.o
	$(DUMP_$(ARCH)) --disassemble --reloc test.elf > test.lst
	$(DUMP_$(ARCH)) -h test.elf > test.sec
	$(DUMP_$(ARCH)) -s test.elf > test.cnt
	$(OBJ_$(ARCH)) -O binary test.elf test.bin
	$(SIZE_$(ARCH)) test.elf
	mv test.elf code.elf
	mv test.bin code.bin
	mv test.lst code.lst
	mv test.sec code.sec
	mv test.cnt code.cnt
	mv test.map code.map
	hexdump -v -e '4/1 "%02x" "\n"' code.bin > boot.txt

eeprom_sim:
	echo "b16b00b5" > eeprom.txt
	printf "%08x\n" `stat -L -c %s code.txt` >> eeprom.txt
	cat code.txt >> eeprom.txt

coremark: crt
	$(GCC_$(ARCH)) -o core_list_join.o app/coremark/core_list_join.c
	$(GCC_$(ARCH)) -o core_main.o app/coremark/core_main.c -DFLAGS_STR=\"'$(GCC_$(ARCH))'\" -Dee_printf=printf
	$(GCC_$(ARCH)) -o core_matrix.o app/coremark/core_matrix.c
	$(GCC_$(ARCH)) -o core_state.o app/coremark/core_state.c
	$(GCC_$(ARCH)) -o core_util.o app/coremark/core_util.c
	$(GCC_$(ARCH)) -o core_portme.o app/coremark/core_portme.c -Dee_printf=printf -DPERFORMANCE_RUN=1 -DITERATIONS=1000
	$(GCC_$(ARCH)) -o coremark.o app/coremark/coremark.c
	@$(MAKE) --no-print-directory link

vga_test: crt
	$(GCC_$(ARCH)) -o vga_test.o app/vga/vga_test.c
	@$(MAKE) --no-print-directory link

vga_demo: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o vga_demo.o app/vga/vga_demo.c
	@$(MAKE) --no-print-directory link

vga_cube: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o vga_cube.o app/vga/vga_cube.c
	@$(MAKE) --no-print-directory link
	
vga_ball: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o vga_ball.o app/vga/vga_ball.c
	@$(MAKE) --no-print-directory link
	
vga_sandpile: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o vga_sandpile.o app/vga/vga_sandpile.c
	@$(MAKE) --no-print-directory link

vga_sprite: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o vga_sprite.o app/vga/vga_sprite.c
	@$(MAKE) --no-print-directory link

space_invaders: crt
	$(GCC_$(ARCH)) -o vga_drv.o app/vga/vga_drv.c
	$(GCC_$(ARCH)) -o space_invaders.o app/vga/space_invaders.c
	@$(MAKE) --no-print-directory link

kbd_axis: 	crt
	$(GCC_$(ARCH)) -o kbd_axis.o app/kbd_axis.c
	@$(MAKE) --no-print-directory link


include app/malardalen/malardalen.mak
include app/powerstone/powerstone.mak

clean:
	-rm -rf *~ *.o *.elf *.map *.lst *.sec *.cnt *.txt *.bin *.vhd
	-rm -rf app/*~
	-rm -rf boot/*~
	-rm -rf include/*~
	-rm -rf lib/*~
