/* Linker script para APM32F103CBT6 */
ENTRY(Reset_Handler)

MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 128K
    RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 20K
}

_estack = ORIGIN(RAM) + LENGTH(RAM);

SECTIONS
{
    /* Código en Flash */
    .text :
    {
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    /* Datos Inicializados */
    /* _sidata es la dirección en FLASH donde están los datos para copiar a RAM */
    _sidata = LOADADDR(.data);

    .data :
    {
        . = ALIGN(4);
        _sdata = .;        /* Inicio en RAM */
        *(.data*)
        . = ALIGN(4);
        _edata = .;        /* Fin en RAM */
    } > RAM AT > FLASH

    /* Datos BSS (Ceros) - Corrige __bss_start y __bss_end */
    .bss :
    {
        . = ALIGN(4);
        __bss_start = .;   /* El startup busca este nombre exacto */
        __bss_start__ = .; /* Alias por seguridad */
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        __bss_end = .;     /* El startup busca este nombre exacto */
        __bss_end__ = .;   /* Alias por seguridad */
    } > RAM
}