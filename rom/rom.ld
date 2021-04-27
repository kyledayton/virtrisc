MEMORY
{
    ROM     (rx)  : ORIGIN = 0x00000000, LENGTH = 32K
    RAM     (rwx) : ORIGIN = 0x10000000, LENGTH = 32K
    VRAM    (rw)  : ORIGIN = 0x20000000, LENGTH = 16K
    HID     (rw)  : ORIGIN = 0x30000000, LENGTH = 1K
}

SECTIONS
{
    .text :
    {
        . = ALIGN(4);
        *(.text*)
        *(.rodata*)
    } > ROM

    . = ALIGN(4);
    __END_TEXT = .;

    .bss (NOLOAD):
    {
        . = ALIGN(4);
        __BEGIN_BSS = .;

        *(.bss*)

        . = ALIGN(4);
        __END_BSS = .;
    } > RAM

    .data : AT(__END_TEXT)
    {
        . = ALIGN(4);
        __BEGIN_DATA = .;

        *(.data*);

        . = ALIGN(4);
        __END_DATA = .;
    } > RAM

    .stack (NOLOAD):
    {
        . = ALIGN(4);
        . = . + 0x800;
        . = ALIGN(4);
    } > RAM
}

__BEGIN_VRAM = ORIGIN(VRAM);
__END_VRAM = ORIGIN(VRAM) + LENGTH(VRAM);

__BEGIN_HID = ORIGIN(HID);
__END_HID = ORIGIN(HID) + LENGTH(HID);

__LOAD_DATA_ADDR = LOADADDR(.data);
__LOAD_DATA_SIZE = __END_DATA - __BEGIN_DATA;