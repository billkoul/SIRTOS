CFLAGS = -O3 -std=c99 -fstrength-reduce -fomit-frame-pointer -fno-stack-protector -fvisibility=hidden -fvisibility-inlines-hidden -nostdinc -fno-builtin -I./include 
CPPFLAGS = -O3 -fstrength-reduce -fomit-frame-pointer -finline-functions -nostdinc -fno-builtin -I./include

all: kernel.bin

strip: kernel.bin
	strip.exe kernel.bin

kernel.bin: core/arch/boot.o main.o screen.o blowfish.o abs.o simple_vga.o floor.o floorf.o isxdigit.o isspace.o isblank.o isalpha.o isalnum.o sqrt.o pow.o user.o shell.o string.o cpuid.o textmode.o common.o cpufunctions.o panic.o core/arch/gdt.o core/arch/interrupt.o descriptor_tables.o pit.o rtc.o paging.o ordered_array.o core/arch/process.o task.o isr.o fs.o initrd.o beep.o syscalls.o pokex.o font.o keyb.o math.o ps2.o pci.o kheap.o menu.o md5.o vbe.o core/arch/bios.o gfx.o elf.o window.o binary.o panel.o file_manager.o hello.o usb.o bmp.o paint.o calculator.o g_shell.o memory_browser.o login.o settings.o

	ld -m elf_i386 -T link.ld -o kernel.bin core/arch/boot.o main.o screen.o blowfish.o abs.o simple_vga.o floor.o floorf.o isxdigit.o isspace.o isblank.o isalpha.o isalnum.o sqrt.o pow.o user.o shell.o string.o cpuid.o textmode.o common.o cpufunctions.o panic.o core/arch/gdt.o core/arch/interrupt.o descriptor_tables.o pit.o rtc.o paging.o ordered_array.o core/arch/process.o task.o isr.o fs.o initrd.o beep.o syscalls.o pokex.o font.o keyb.o math.o ps2.o pci.o kheap.o menu.o md5.o vbe.o core/arch/bios.o gfx.o elf.o window.o binary.o panel.o file_manager.o usb.o bmp.o paint.o calculator.o  g_shell.o memory_browser.o login.o  settings.o


	@echo "************** removing output files ******************"
	cp hello.o initrd/hello.o
	cp hello.bin initrd/hello.bin
	rm *.o
	rm hello.bin
	@echo "************** Creating Initrd image ******************"
	./initrd/make_initrd initrd/driver.bin driver.bin initrd/read.txt read.txt initrd/version.txt version.txt initrd/cursor.bmp cursor.bmp initrd/hello.o hello.o
	@echo "********* Copying files to boot directory *************"
	cp kernel.bin boot/kernel.bin
	cp initrd.img boot/initrd.img
	@echo "******************************************************"
	@echo "************************ Done ************************"
	@echo "****************** Ceravnos is ready *****************"
	@echo "******************************************************"

main.o: core/main.c
	@echo "*************** Compiling scripts ******************"
	@echo "********************** main. c**********************"
	gcc -m32 $(CFLAGS) -c core/main.c

blowfish.o: core/security/blowfish.c
	@echo "*************** Compiling System ******************"
	@echo "****************** blowfish.c ******************"
	gcc -m32 $(CFLAGS) -c core/security/blowfish.c

user.o: core/system/user.c
	@echo "****************** user.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/user.c

font.o: core/gui/font.c
	@echo "****************** font.c **********************"
	gcc -m32 $(CFLAGS) -c core/gui/font.c

window.o: core/gui/window.c
	@echo "****************** window.c **********************"
	gcc -m32 $(CFLAGS) -c core/gui/window.c

elf.o: core/system/elf.c
	@echo "****************** elf.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/elf.c

binary.o: core/system/binary.c
	@echo "****************** binary.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/binary.c

cpuid.o: core/system/cpuid.c
	@echo "****************** cpuid.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/cpuid.c
	
textmode.o: core/system/textmode.c
	@echo "****************** textmode.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/textmode.c

ordered_array.o: core/arch/ordered_array.c
	@echo "****************** ordered_array.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/ordered_array.c
	
fs.o: core/fs/fs.c
	@echo "****************** fs.c **********************"
	gcc -m32 $(CFLAGS) -c core/fs/fs.c
	
initrd.o: core/initrd/initrd.c
	@echo "****************** initrd.c **********************"
	gcc -m32 $(CFLAGS) -c core/initrd/initrd.c
	
beep.o: core/system/beep.c
	@echo "****************** initrd.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/beep.c
	
syscalls.o: core/system/syscalls.c
	@echo "****************** syscalls.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/syscalls.c

menu.o: core/system/menu.c
	@echo "****************** menu.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/menu.c

md5.o: core/security/md5.c
	@echo "****************** md5.c **********************"
	gcc -m32 $(CFLAGS) -c core/security/md5.c

abs.o: core/lib/abs.c
	@echo "*************** Compiling Libs ******************"
	@echo "****************** abs.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/abs.c
	
math.o: core/lib/math.c
	@echo "****************** math.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/math.c

floor.o: core/lib/floor.c
	@echo "****************** floor.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/floor.c

floorf.o: core/lib/floorf.c
	@echo "****************** floorf.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/floorf.c
	
pow.o: core/lib/pow.c
	@echo "****************** pow.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/pow.c
	
isxdigit.o: core/lib/isxdigit.c
	@echo "****************** isxdigit.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/isxdigit.c
	
isblank.o: core/lib/isblank.c
	@echo "****************** isblank.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/isblank.c

isalpha.o: core/lib/isalpha.c
	@echo "****************** isalpha.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/isalpha.c
	
isalnum.o: core/lib/isalnum.c
	@echo "****************** isalnum.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/isalnum.c

sqrt.o: core/lib/sqrt.c
	@echo "****************** sqrt.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/sqrt.c
	
isspace.o: core/lib/isspace.c
	@echo "****************** isspace.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/isspace.c

string.o: core/lib/string.c
	@echo "****************** string.c **********************"
	gcc -m32 $(CFLAGS) -c core/lib/string.c
	
common.o: core/arch/common.c
	@echo "*************** Compiling Core ******************"
	@echo "****************** common.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/common.c

cpufunctions.o: core/arch/cpufunctions.c
	@echo "****************** cpufunctions.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/cpufunctions.c
	
panic.o: core/arch/panic.c
	@echo "****************** panic.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/panic.c
	
descriptor_tables.o: core/arch/descriptor_tables.c
	@echo "****************** descriptor_tables.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/descriptor_tables.c
	
kheap.o: core/arch/kheap.c
	@echo "****************** kheap.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/kheap.c
	
paging.o: core/arch/paging.c
	@echo "****************** paging.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/paging.c
	
pit.o: core/arch/pit.c
	@echo "****************** pit.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/pit.c

rtc.o: core/arch/rtc.c
	@echo "****************** rtc.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/rtc.c

isr.o: core/arch/isr.c
	@echo "****************** isr.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/isr.c
	
task.o: core/arch/task.c
	@echo "****************** task.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/task.c
	
core/arch/process.o: core/arch/process.asm
	@echo "****************** process.asm **********************"
	nasm -f elf32 core/arch/process.asm 

core/arch/bios.o: core/arch/bios.asm
	@echo "****************** bios.asm **********************"
	nasm -f elf32 core/arch/bios.asm
	
core/arch/interrupt.o: core/arch/interrupt.asm
	@echo "****************** interrupt.asm **********************"
	nasm -f elf32 core/arch/interrupt.asm 
	
core/arch/gdt.o: core/arch/gdt.asm
	@echo "****************** gdt.asm **********************"
	nasm -f elf32 core/arch/gdt.asm 

core/arch/boot.o: core/arch/boot.asm
	@echo "****************** boot.asm **********************"
	nasm -f elf32 core/arch/boot.asm 
	
pokex.o: core/arch/pokex.c
	@echo "****************** pokex.c **********************"
	gcc -m32 -m32 $(CFLAGS) -c core/arch/pokex.c 

pokex.o: core/arch/pokex.c
	@echo "****************** pokex.c **********************"
	gcc -m32 -m32 $(CFLAGS) -c core/arch/pokex.c

screen.o: core/gui/screen.c
	@echo "****************** screen.c **********************"
	gcc -m32 $(CFLAGS) -c core/gui/screen.c

bmp.o: core/gui/bmp.c
	@echo "****************** bmp.c **********************"
	gcc -m32 $(CFLAGS) -c core/gui/bmp.c

cpu.o: core/arch/cpu.c
	@echo "****************** cpu.c **********************"
	gcc -m32 $(CFLAGS) -c core/arch/cpu.c

pci.o: core/system/pci.c
	@echo "************* Compiling Drivers *****************"
	@echo "****************** pci.c **********************"
	gcc -m32 $(CFLAGS) -c core/system/pci.c

keyb.o: core/drivers/keyboard/keyb.c
	@echo "****************** keyb.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/keyboard/keyb.c

keyb.o: core/drivers/keyboard/keyb.c
	@echo "****************** keyb.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/keyboard/keyb.c
	
ps2.o: core/drivers/mouse/ps2.c
	@echo "****************** ps2.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/mouse/ps2.c

simple_vga.o: core/drivers/video/simple_vga.c
	@echo "****************** simple_vga.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/video/simple_vga.c

vbe.o: core/drivers/video/vbe.c
	@echo "****************** vbe.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/video/vbe.c

gfx.o: core/gui/gfx.c
	@echo "****************** gfx.c **********************"
	gcc -m32 $(CFLAGS) -c core/gui/gfx.c

shell.o: services/shell.c
	@echo "************* Compiling Programs *****************"
	@echo "****************** shell.c **********************"
	gcc -m32 $(CFLAGS) -c services/shell.c

panel.o: services/panel.c
	@echo "****************** panel.c **********************"
	gcc -m32 $(CFLAGS) -c services/panel.c

file_manager.o: services/file_manager.c
	@echo "****************** file_manager.c **********************"
	gcc -m32 $(CFLAGS) -c services/file_manager.c

paint.o: services/paint.c
	@echo "****************** paint.c **********************"
	gcc -m32 $(CFLAGS) -c services/paint.c

calculator.o: services/calculator.c
	@echo "****************** calculator.c **********************"
	gcc -m32 $(CFLAGS) -c services/calculator.c

memory_browser.o: services/memory_browser.c
	@echo "****************** browser.c **********************"
	gcc -m32 $(CFLAGS) -c services/memory_browser.c

login.o: services/login.c
	@echo "****************** login.c **********************"
	gcc -m32 $(CFLAGS) -c services/login.c

file_manager.o: services/file_manager.c
	@echo "****************** file_manager.c **********************"
	gcc -m32 $(CFLAGS) -c services/file_manager.c

settings.o: services/settings.c
	@echo "****************** settings.c **********************"
	gcc -m32 $(CFLAGS) -c services/settings.c

g_shell.o: services/g_shell.c
	@echo "****************** g_shell.c **********************"
	gcc -m32 $(CFLAGS) -c services/g_shell.c

hello.o: bin/hello.c
	@echo "****************** hello.c **********************"
	gcc -m32 $(CFLAGS) -c bin/hello.c
	objcopy -O binary hello.o hello.bin

usb.o: core/drivers/usb/usb.c
	@echo "****************** usb.c **********************"
	gcc -m32 $(CFLAGS) -c core/drivers/usb/usb.c	


