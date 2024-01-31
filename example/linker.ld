
ENTRY(Reset_Handler)

_estack = ORIGIN(RAM) + LENGTH(RAM);

/* Memory definition */
MEMORY
{
  RAM (xrw)     : ORIGIN = 0x20000000, LENGTH = 64K
  SRAM2 (xrw)   : ORIGIN = 0x10000000, LENGTH = 16K
  FLASH (xr)    : ORIGIN = 0x08000000, LENGTH = 256K
}


/* Output Sections */
SECTIONS
{
  /* The startup code into "FLASH" Rom type memory */
  .isr_vector :
  {
    . = ALIGN(4);
    *(.isr_vector)
    . = ALIGN(4);
  } > FLASH

  /* The program code and other data into "FLASH" Rom type memory */
  .text :
  {
    . = ALIGN(4);
    *(.text)
    *(.text*)
    . = ALIGN(4);
    _etext = .;
  } > FLASH

  /* Constant data into "FLASH" Rom type memory */
  .rodata :
  {
    . = ALIGN(4);
    *(.rodata)    /* .rodata sections (constants, strings, etc.) */
    *(.rodata*)   /* .rodata* sections (constants, strings, etc.) */
    . = ALIGN(4);
  } > FLASH

  _sidata = LOADADDR(.data);

  .data :
  {
    . = ALIGN(4);
    _sdata = .;
    *(.data)
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } > RAM AT > FLASH

  /* SRAM2 section
  *
  * IMPORTANT NOTE!
  * If initialized variables will be placed in this section,
  * the startup code needs to be modified to copy the init-values.
  */
  .sram2 :
  {
    . = ALIGN(4);
    _ssram2 = .;       /* create a global symbol at sram2 start */
    *(.sram2)
    *(.sram2*)

    . = ALIGN(4);
    _esram2 = .;       /* create a global symbol at sram2 end */
  } >SRAM2 AT> FLASH

  .bss :
  {
    _sbss = .;
    *(.bss)
    *(.bss*)
    . = ALIGN(4);
    _ebss = .;
  } > RAM

}
