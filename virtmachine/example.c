
#include <virtrisc/virtrisc.h>
#include <virtrisc/ops.h>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BYTES(n) (n)
#define KILOBYTES(n) (BYTES(1024) * (n))

#define RAM_SIZE KILOBYTES(16)

Byte *LoadBinaryFile(const char filename[], size_t *outFileSize);
void EBreak(VirtRISC_Machine *machine) {
    VirtRISC_CPU_DumpRegisters(&machine->cpu);
}

void ECall(VirtRISC_Machine *machine) {
    machine->running = false;
}

int main () {
    // Initialize machine
    VirtRISC_Machine machine;
    VirtRISC_Machine_Init(&machine);
    machine.ecallFunc = ECall;
    machine.ebreakFunc = EBreak;

    // Load ROM file from disk
    size_t romFileSize = 0;
    Byte *romFile = LoadBinaryFile("rom.bin", &romFileSize);
    assert(romFile != nullptr);
    assert(romFileSize > 0);

    // Construct ROM map
    VirtRISC_BasicMem rom;
    VirtRISC_BasicMem_Init(&rom, 0x0, romFile, romFileSize, true);

    // Allocate buffer for RAM
    Byte *ramBuf = malloc(RAM_SIZE);
    memset(ramBuf, 0, RAM_SIZE);

    // Create RAM map
    VirtRISC_BasicMem ram;
    VirtRISC_BasicMem_Init(&ram, 0x10000000, ramBuf, RAM_SIZE, false);

    // Attach mem to machine
    VirtRISC_Machine_MapMem(&machine, (VirtRISC_MemMap *) &rom);
    VirtRISC_Machine_MapMem(&machine, (VirtRISC_MemMap *) &ram);

    // Start execution at PC=0x0
    VirtRISC_Machine_Start(&machine, 0x0);

    free(romFile);
    free(ramBuf);
}

Byte *LoadBinaryFile(const char filename[], size_t *outFileSize) {
    FILE *f = fopen(filename, "rb");

    if (!f) {
        return nullptr;
    }

    size_t filesize = 0;
    fseek(f, 0, SEEK_END);
    filesize = ftell(f);
    rewind(f);

    Byte *buf = malloc(filesize * sizeof(Byte));
    fread(buf, sizeof(Byte), filesize, f);
    fclose(f);

    if (outFileSize) {
        *outFileSize = filesize;
    }

    return buf;
}