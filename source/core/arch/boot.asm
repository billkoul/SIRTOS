;--------------------------------------
;		Bootstrap
;--------------------------------------

;These are the multiboot macroes, they are used to configure how GRUB loads the kernel
MBOOT_PAGE_ALIGN    equ 1<<0 ;Loads everything on page boundaries
MBOOT_MEM_INFO      equ 1<<1 ;Load memory map
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Required (MAGIC)
MBOOT_VBEINFO	    equ 1<<2 ; video info
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO | MBOOT_VBEINFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)

[BITS 32]

[GLOBAL mboot]
[EXTERN code]
[EXTERN bss]  
[EXTERN end]  

mboot:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM
  dd  MBOOT_VBEINFO
  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start ;initial EIP

[GLOBAL start] ;this is the Kernel's entry point
[EXTERN init_system] ;this is the starting function in the kernel

start:
  push    esp ;Stack location
  push    ebx ;Multiboot header location

  cli	     ;Stop interrupts
  call init_system ;call the starting function
  jmp $      

