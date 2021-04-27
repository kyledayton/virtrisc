#ifndef VIRTRISC_CPU_H
#define VIRTRISC_CPU_H

#include <virtrisc/types.h>

#define XLEN 32
typedef struct VirtRISC_CPU {
    struct VirtRISC_Machine *machine;
    Word r[XLEN]; // Registers
    Word pc; // Program Counter
} VirtRISC_CPU;

void VirtRISC_CPU_Init(VirtRISC_CPU *cpu);
void VirtRISC_CPU_Execute(VirtRISC_CPU *cpu, Word instruction);
void VirtRISC_CPU_DumpRegisters(const VirtRISC_CPU *cpu);

#endif //VIRTRISC_CPU_H
