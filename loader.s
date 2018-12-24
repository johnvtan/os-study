  global loader                 ; entry symbol for elf

  MAGIC_NUMBER equ 0x1BADB002   ; magic number constant
  FLAGS equ 0x0                 ; multiboot flags
  CHECKSUM equ -MAGIC_NUMBER    ; calculate the checksum - magic number + checksum = 0

  section .text
  align 4                       ; code must be 4 byte aligned
  dd MAGIC_NUMBER               ; dd = define double word. This just places the magic number, flags, and checksum at the
  dd FLAGS                      ; start of the text section
  dd CHECKSUM

loader:
  mov eax, 0xCAFEBABE           ; dump 0xCAFEBABE into eax
.loop:
  jmp .loop
