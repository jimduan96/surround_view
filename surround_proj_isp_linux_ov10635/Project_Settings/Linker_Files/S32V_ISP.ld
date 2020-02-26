SECTIONS
{
  . = 0x10000;
  .text : { *(.none) }
  . = 0x0000000;
  .data : { *(.data) *(.text) }
  .bss : { *(.bss) }	
}

