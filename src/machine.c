#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <virtrisc/virtrisc.h>

VirtRISC_MemMap *VirtRISC_Machine_GetMemMap(VirtRISC_Machine *machine, Word address) {
    for (size_t i = 0; i < machine->numMemMaps; i++) {
        VirtRISC_MemMap *map = machine->mem[i];
        if (address >= map->low && address <= map->high) {
            return map;
        }
    }

    return nullptr;
}

void VirtRISC_Machine_Init(VirtRISC_Machine *machine) {
    for (size_t i = 0; i < MAX_MEM_MAPS; i++) {
        machine->mem[i] = nullptr;
    }
    machine->numMemMaps = 0;
    machine->running = false;

    VirtRISC_CPU_Init(&machine->cpu);
    machine->cpu.machine = machine;
}

void VirtRISC_Machine_MapMem(VirtRISC_Machine *machine, VirtRISC_MemMap *mem) {
    if (machine->numMemMaps + 1 < MAX_MEM_MAPS) {
        machine->mem[machine->numMemMaps++] = mem;
    }
}

void VirtRISC_Machine_Start(VirtRISC_Machine *machine, Word initProgramCounter) {
    machine->cpu.pc = initProgramCounter;
    machine->running = true;

    VirtRISC_MemMap *rom = VirtRISC_Machine_GetMemMap(machine, initProgramCounter);
    assert(rom && "No ROM found");

    while (machine->running) {
        Word inst = rom->LoadWord(rom, machine->cpu.pc);
        machine->cpu.pc += sizeof(Word);
        VirtRISC_CPU_Execute(&machine->cpu, inst);
    }
}

void VirtRISC_Machine_HaltException(VirtRISC_Machine *machine, const char *message) {
    printf("Machine halted due to exception: %s\n", message);
    VirtRISC_CPU_DumpRegisters(&machine->cpu);
    machine->running = false;
}