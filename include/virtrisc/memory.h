#ifndef VIRTRISC_MEMORY_H
#define VIRTRISC_MEMORY_H

#include <virtrisc/types.h>

/** MemMap func pointer declarations **/
typedef struct VirtRISC_MemMap VirtRISC_MemMap;
typedef Byte (*MEM_LOAD_BYTE_FUNC)(VirtRISC_MemMap *mem, Word address);
typedef HWord (*MEM_LOAD_HWORD_FUNC)(VirtRISC_MemMap *mem, Word address);
typedef Word (*MEM_LOAD_WORD_FUNC)(VirtRISC_MemMap *mem, Word address);
typedef void (*MEM_STORE_BYTE_FUNC)(VirtRISC_MemMap *mem, Word address, Byte val);
typedef void (*MEM_STORE_HWORD_FUNC)(VirtRISC_MemMap *mem, Word address, HWord val);
typedef void (*MEM_STORE_WORD_FUNC)(VirtRISC_MemMap *mem, Word address, Word val);

struct VirtRISC_MemMap {
    Word low, high; // Begin and end addresses of memory

    MEM_LOAD_BYTE_FUNC   LoadByte;
    MEM_STORE_BYTE_FUNC  StoreByte;

    MEM_LOAD_HWORD_FUNC  LoadHWord;
    MEM_STORE_HWORD_FUNC StoreHWord;

    MEM_LOAD_WORD_FUNC   LoadWord;
    MEM_STORE_WORD_FUNC  StoreWord;
};

Byte VirtRISC_MemMap_LoadByte(VirtRISC_MemMap *mem, Word address);
void VirtRISC_MemMap_StoreByte(VirtRISC_MemMap *mem, Word address, Byte val);

HWord VirtRISC_MemMap_LoadHWord(VirtRISC_MemMap *mem, Word address);
void VirtRISC_MemMap_StoreHWord(VirtRISC_MemMap *mem, Word address, HWord val);

Word VirtRISC_MemMap_LoadWord(VirtRISC_MemMap *mem, Word address);
void VirtRISC_MemMap_StoreWord(VirtRISC_MemMap *mem, Word address, Word val);

typedef struct VirtRISC_BasicMem {
    VirtRISC_MemMap base;
    Byte *data;
    size_t dataSize;
} VirtRISC_BasicMem;

void VirtRISC_BasicMem_Init(VirtRISC_BasicMem *mem, Word baseAddress, Byte *data, size_t dataSize, bool readonly);

Byte VirtRISC_BasicMem_LoadByte(VirtRISC_MemMap *mem, Word address);
void VirtRISC_BasicMem_StoreByte(VirtRISC_MemMap *mem, Word address, Byte val);

HWord VirtRISC_BasicMem_LoadHWord(VirtRISC_MemMap *mem, Word address);
void VirtRISC_BasicMem_StoreHWord(VirtRISC_MemMap *mem, Word address, HWord val);

Word VirtRISC_BasicMem_LoadWord(VirtRISC_MemMap *mem, Word address);
void VirtRISC_BasicMem_StoreWord(VirtRISC_MemMap *mem, Word address, Word val);

#endif //VIRTRISC_MEMORY_H
