#ifndef VIRTRISC_MACHINE_H
#define VIRTRISC_MACHINE_H

#include <virtrisc/types.h>
#include <virtrisc/memory.h>
#include <virtrisc/cpu.h>

typedef struct VirtRISC_Machine VirtRISC_Machine;
typedef void (*SYS_CALL_FUNC)(VirtRISC_Machine *machine);
typedef void (*SYS_BREAK_FUNC)(VirtRISC_Machine *machine);

#define MAX_MEM_MAPS 32
struct VirtRISC_Machine {
    VirtRISC_CPU cpu;
    VirtRISC_MemMap *mem[MAX_MEM_MAPS];
    SYS_CALL_FUNC ecallFunc;
    SYS_BREAK_FUNC ebreakFunc;
    size_t numMemMaps;
    bool running;
};

void VirtRISC_Machine_Init(VirtRISC_Machine *machine);
void VirtRISC_Machine_MapMem(VirtRISC_Machine *machine, VirtRISC_MemMap *mem);
VirtRISC_MemMap *VirtRISC_Machine_GetMemMap(VirtRISC_Machine *machine, Word address);
void VirtRISC_Machine_Start(VirtRISC_Machine *machine, Word initProgramCounter);
void VirtRISC_Machine_HaltException(VirtRISC_Machine *machine, const char *message);

#endif //VIRTRISC_MACHINE_H
