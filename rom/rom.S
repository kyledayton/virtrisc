
.section .text
.global __BEGIN_TEXT, __END_TEXT
.global __BEGIN_BSS, __END_BSS
.global __BEGIN_DATA, __END_DATA
.global __BEGIN_VRAM, __END_VRAM
.global __BEGIN_HID, __END_HID
.global __LOAD_DATA_ADDR, __LOAD_DATA_SIZE

.global _start

_start:
    li x3, 0
    li x4, 10
1:
    addi x3, x3, 1
    blt x3, x4, 1b
    ebreak
    ecall
