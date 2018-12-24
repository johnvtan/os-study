ENTRY(loader)
SECTIONS
{
         . = 0x00100000; /* Set current address to 0x100000 */
         .text ALIGN (0x1000) : /* align to 4 KB */
         {
            *(.text)
         }

         .rodata ALIGN (0x1000) :
         {
            *(.rodata*) /* all read only data sections from all files */
         }

         .data ALIGN (0x1000) :
         {
            *(.data) /* all data sections from all files */
         }

         .bss ALIGN (0x1000) :
         {
            *(COMMON) /* all COMMON sections from all files */
            *(.bss) /* all bss sections from all files */
         }
}
