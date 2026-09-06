ENTRY(_start)

SECTIONS
{
    /* 1. Tell the linker our kernel base is physical 0x10000 */
    . = 0x10000;

    /* 2. Executable Code Section (Aligned to 4-byte boundary) */
    .text : ALIGN(4)
    {
        *(.text*)   /* The wildcard * catches sub-sections like .text.startup */
    }

    /* 3. Read-Only Data Section (Strings, constants) */
    .rodata : ALIGN(4)
    {
        *(.rodata*)
    }

    /* 4. Initialized Data Section (Global variables with values) */
    .data : ALIGN(4)
    {
        *(.data*)
    }

    /* 5. Uninitialized Data Section (Clears to zero at startup) */
    .bss : ALIGN(4)
    {
        *(COMMON)
        *(.bss*)
    }
}
