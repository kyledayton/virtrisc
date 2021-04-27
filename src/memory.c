#include <virtrisc/memory.h>

Byte VirtRISC_MemMap_LoadByte(VirtRISC_MemMap *mem, Word address) {
    if (mem->LoadByte) {
        return mem->LoadByte(mem, address);
    }

    return 0;
}

void VirtRISC_MemMap_StoreByte(VirtRISC_MemMap *mem, Word address, Byte val) {
    if (mem->StoreByte) {
        mem->StoreByte(mem, address, val);
    }
}

HWord VirtRISC_MemMap_LoadHWord(VirtRISC_MemMap *mem, Word address) {
    if (mem->LoadHWord) {
        return mem->LoadHWord(mem, address);
    }

    return 0;
}

void VirtRISC_MemMap_StoreHWord(VirtRISC_MemMap *mem, Word address, HWord val) {
    if (mem->StoreHWord) {
        mem->StoreHWord(mem, address, val);
    }
}

Word VirtRISC_MemMap_LoadWord(VirtRISC_MemMap *mem, Word address) {
    if (mem->LoadWord) {
        return mem->LoadWord(mem, address);
    }

    return 0;
}

void VirtRISC_MemMap_StoreWord(VirtRISC_MemMap *mem, Word address, Word val) {
    if (mem->StoreWord) {
        mem->StoreWord(mem, address, val);
    }
}

void
VirtRISC_BasicMem_Init(VirtRISC_BasicMem *mem,
                       Word baseAddress,
                       Byte *data, size_t dataSize,
                       bool readonly) {

    mem->base.low = baseAddress;
    mem->base.high = baseAddress + dataSize;
    mem->data = data;
    mem->dataSize = dataSize;

    mem->base.LoadByte = VirtRISC_BasicMem_LoadByte;
    mem->base.LoadHWord = VirtRISC_BasicMem_LoadHWord;
    mem->base.LoadWord = VirtRISC_BasicMem_LoadWord;

    if (!readonly) {
        mem->base.StoreByte = VirtRISC_BasicMem_StoreByte;
        mem->base.StoreHWord = VirtRISC_BasicMem_StoreHWord;
        mem->base.StoreWord = VirtRISC_BasicMem_StoreWord;
    }
}

Byte *VirtRISC_BasicMem_MapAddress(VirtRISC_MemMap *mem, Word address) {
    VirtRISC_BasicMem *bm = (VirtRISC_BasicMem*) mem;
    return &bm->data[address - bm->base.low];
}

Byte VirtRISC_BasicMem_LoadByte(VirtRISC_MemMap *mem, Word address) {
    return *VirtRISC_BasicMem_MapAddress(mem, address);
}

void VirtRISC_BasicMem_StoreByte(VirtRISC_MemMap *mem, Word address, Byte val) {
    *VirtRISC_BasicMem_MapAddress(mem, address) = val;
}

HWord VirtRISC_BasicMem_LoadHWord(VirtRISC_MemMap *mem, Word address) {
    Byte *data = VirtRISC_BasicMem_MapAddress(mem, address);
    return (HWord) *data | (HWord) *(data + 1) << 8;
}

void VirtRISC_BasicMem_StoreHWord(VirtRISC_MemMap *mem, Word address, HWord val) {
    Byte *data = VirtRISC_BasicMem_MapAddress(mem, address);
    *data = val & 0xFF;
    *(data + 1) = (val & 0xFF00) >> 8;
}

Word VirtRISC_BasicMem_LoadWord(VirtRISC_MemMap *mem, Word address) {
    Byte *data = VirtRISC_BasicMem_MapAddress(mem, address);
    return (Word) *data | (Word) *(data + 1) << 8 | (Word) *(data + 2) << 16 | (Word) *(data + 3) << 24;
}

void VirtRISC_BasicMem_StoreWord(VirtRISC_MemMap *mem, Word address, Word val) {
    Byte *data = VirtRISC_BasicMem_MapAddress(mem, address);
    *data = val & 0xFF;
    *(data + 1) = (val & 0xFF00) >> 8;
    *(data + 2) = (val & 0xFF0000) >> 16;
    *(data + 3) = (val & 0xFF000000) >> 24;
}