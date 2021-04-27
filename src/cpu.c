#include <stdio.h>
#include <assert.h>

#include <virtrisc/virtrisc.h>
#include <virtrisc/ops.h>

#define OPCODE_SHIFT 0
#define OPCODE_MASK ((Word) 0b1111111 << OPCODE_SHIFT)
#define OPCODE(inst) (((inst) & OPCODE_MASK) >> OPCODE_SHIFT)

#define FUNCT3_SHIFT 12
#define FUNCT3_MASK ((Word) 0b111 << FUNCT3_SHIFT)
#define FUNCT3(inst) (((inst) & FUNCT3_MASK) >> FUNCT3_SHIFT)

#define FUNCT7_SHIFT 25
#define FUNCT7_MASK ((Word) 0b1111111 << FUNCT7_SHIFT)
#define FUNCT7(inst) (((inst) & FUNCT7_MASK) >> FUNCT7_SHIFT)

#define IMM12_SHIFT 20
#define IMM12_MASK ((Word) 0xFFF << IMM12_SHIFT)
#define IMM12_I(inst) (((inst) & IMM12_MASK) >> IMM12_SHIFT)

#define IMM12_S(inst) (((((Word) 0b11111 << 7) & (inst)) >> 7) | ((((Word) 0b1111111 << 25) & (inst)) >> 20))

#define OFFSET_J(inst) (((((Word) 0b1111111111 << 21) & (inst)) >> 20) | ((((Word) 1 << 20) & (inst)) >> 9) \
                      | ((((Word) 0xFF << 12) & (inst))) | ((((Word) 1 << 31) & (inst)) >> 11))
#define OFFSET_B(inst) (((((Word) 1 << 7) & (inst)) << 4) | ((((Word) 0b1111 << 8) & (inst)) >> 7) \
                      | ((((Word) 0b111111 << 25) & (inst)) >> 20) | ((((Word) 1 << 31) & (inst)) >> 19))

#define IMM20_SHIFT 12
#define IMM20_MASK ((Word) 0xFFFFF << IMM20_SHIFT)
#define IMM20(inst) (((inst) & IMM20_MASK) >> IMM20_SHIFT)

#define RD_SHIFT 7
#define RD_MASK ((Word) 0b11111 << RD_SHIFT)
#define RD(inst) (((inst) & RD_MASK) >> RD_SHIFT)

#define RS1_SHIFT 15
#define RS1_MASK ((Word) 0b11111 << RS1_SHIFT)
#define RS1(inst) (((inst) & RS1_MASK) >> RS1_SHIFT)

#define RS2_SHIFT 20
#define RS2_MASK ((Word) 0b11111 << RS2_SHIFT)
#define RS2(inst) (((inst) & RS2_MASK) >> RS2_SHIFT)


void VirtRISC_CPU_Init(VirtRISC_CPU *cpu) {
    assert(cpu != nullptr);

    for (Byte idx = 0; idx < XLEN; idx++) {
        cpu->r[idx] = 0;
    }

    cpu->pc = 0;
}

Word signExtendImm12(Word imm12) {
    if (imm12 & (1 << 11)) { // Test sign bit
        imm12 |= 0xFFFFF000; // Sign extend if required
    }

    return imm12;
}

Word signExtendImm12B(Word imm12) {
    if (imm12 & (1 << 12)) { // Test sign bit
        imm12 |= 0xFFFFE000; // Sign extend if required
    }

    return imm12;
}

Word signExtendOffset20(Word imm20) {
    if (imm20 & (1 << 20)) { // Test sign bit (bit 21)
        imm20 |= 0xFFE00000; // Sign extend if required
    }

    return imm20;
}


Word signExtendByte(Byte b) {
    if (b & (1 << 7)) {
        return (Word) b | 0xFFFFFF00;
    }

    return (Word) b;
}

Word signExtendHWord(HWord h) {
    if (h & (1 << 15)) {
        return (Word) h | 0xFFFF0000;
    }

    return (Word) h;
}

void VirtRISC_CPU_DumpRegisters(const VirtRISC_CPU *cpu) {
    printf("PC:\t\t0x%x (%d)\n", cpu->pc, cpu->pc);
    for (int i = 0; i < XLEN; i++) {
        printf("R%d:\t\t0x%x (%d)\n", i, cpu->r[i], cpu->r[i]);
    }
}


void CPU_ExecuteI_Arithmetic(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12);
void CPU_ExecuteI_Load(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12);
void CPU_ExecuteI_Sys(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12);
void CPU_ExecuteI_JALR(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12);

void CPU_ExecuteJ_JAL(VirtRISC_CPU *cpu, Word opcode, Word rd, Word offset20);

void CPU_ExecuteU(VirtRISC_CPU *cpu, Word opcode, Word rd, Word imm20);
void CPU_ExecuteR(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word rs2, Word funct7);
void CPU_ExecuteB(VirtRISC_CPU *cpu, Word opcode, Word funct3, Word rs1, Word rs2, Word imm12);

void CPU_ExecuteS_Store(VirtRISC_CPU *cpu, Word opcode, Word funct3, Word rs1, Word rs2, Word imm12);

Word CPU_RegGet(VirtRISC_CPU *cpu, Word r);
void CPU_RegSet(VirtRISC_CPU *cpu, Word r, Word val);

void VirtRISC_CPU_Execute(VirtRISC_CPU *cpu, Word instruction) {
    Word opcode = OPCODE(instruction);

    switch (opcode) {
        // I-type instructions
        case 0b0010011: { // ADDI / SLTI / SLTIU / XORI / ORI / ANDI / SLLI / SRLI / SRAI
            CPU_ExecuteI_Arithmetic(cpu, opcode, RD(instruction), FUNCT3(instruction), RS1(instruction), IMM12_I(instruction));
        } break;

        case 0b0000011: { // LB / LH / LW / LBU / LHU
            CPU_ExecuteI_Load(cpu, opcode, RD(instruction), FUNCT3(instruction), RS1(instruction), IMM12_I(instruction));
        } break;

        case 0b1110011: { // ECALL / EBREAK
            CPU_ExecuteI_Sys(cpu, opcode, RD(instruction), FUNCT3(instruction), RS1(instruction), IMM12_I(instruction));
        } break;

        case OPCODE_JALR: {
            CPU_ExecuteI_JALR(cpu, opcode, RD(instruction), FUNCT3(instruction), RS1(instruction), IMM12_I(instruction));
        } break;

        // S-type instructions
        case 0b0100011: { // SB / SH / SW
            CPU_ExecuteS_Store(cpu, opcode, FUNCT3(instruction), RS1(instruction), RS2(instruction), IMM12_S(instruction));
        } break;

        // U-type instructions
        case OPCODE_LUI:
        case OPCODE_AUIPC: {
            CPU_ExecuteU(cpu, opcode, RD(instruction), IMM20(instruction));
        } break;

        // R-type instructions
        case 0b0110011: {
            CPU_ExecuteR(cpu, opcode, RD(instruction), FUNCT3(instruction), RS1(instruction), RS2(instruction),
                         FUNCT7(instruction));
        } break;

        // J-type instructions
        case OPCODE_JAL: {
            CPU_ExecuteJ_JAL(cpu, opcode, RD(instruction), OFFSET_J(instruction));
        } break;

        // B-type instructions
        case 0b1100011: { // BEQ / BNE / BLT / BGE / BLTU / BGEU
            CPU_ExecuteB(cpu, opcode, FUNCT3(instruction), RS1(instruction), RS2(instruction), OFFSET_B(instruction));
        } break;

        default: {
            VirtRISC_CPU_DumpRegisters(cpu);
            printf("Unknown instruction ins=0x%x\n", instruction);
            cpu->machine->running = false;
        }
    }
}

void CPU_ExecuteI_JALR(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12) {
    switch (funct3) {
        case FUNCT3_JALR: {
            CPU_RegSet(cpu, rd, cpu->pc); // Store the address following this jump ins to RD

            Word newAddr = CPU_RegGet(cpu, rs1) + signExtendImm12B(imm12);
            cpu->pc = newAddr & 0xFFFFFFFE; // Set the LSB to 0 as per RISC-V spec
        } break;
        default: {
            printf("Unknown I-type instruction opcode=0x%x rd=0x%x funct3=0x%x rs1=0x%x imm12=0x%x\n", opcode, rd, funct3, rs1, imm12);
        }
    }
}

void CPU_ExecuteI_Arithmetic(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12) {
    switch (funct3) {
        case FUNCT3_ADDI: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) + signExtendImm12(imm12));
        } break;
        case FUNCT3_SLTI: {
            CPU_RegSet(cpu, rd, (Signed) CPU_RegGet(cpu, rs1) < (Signed) signExtendImm12(imm12));
        } break;
        case FUNCT3_SLTIU: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) < signExtendImm12(imm12));
        } break;
        case FUNCT3_XORI: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) ^ signExtendImm12(imm12));
        } break;
        case FUNCT3_ORI: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) | signExtendImm12(imm12));
        } break;
        case FUNCT3_ANDI: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) & signExtendImm12(imm12));
        } break;
        case FUNCT3_SLLI: {
            CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) << (imm12 & 0b11111));
        } break;
        case FUNCT3_SRLI: { // SRLI and SRAI share the same funct3, but are differentiated by bit #30
            Word shiftAmt = imm12 & 0b11111;
            Byte srai = ((imm12 & (1<<5)) >> 5);

            if (srai) {
                Word value = (Word) (((Signed) CPU_RegGet(cpu, rs1))>> shiftAmt);
                CPU_RegSet(cpu, rd, value);
            } else {
                CPU_RegSet(cpu, rd, CPU_RegGet(cpu, rs1) >> shiftAmt);
            }
        } break;
        default: {
            printf("Unknown I-type instruction opcode=0x%x rd=0x%x funct3=0x%x rs1=0x%x imm12=0x%x\n", opcode, rd, funct3, rs1, imm12);
        }
    }
}

void CPU_ExecuteI_Load(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12) {
    Word address = CPU_RegGet(cpu, rs1) + signExtendImm12(imm12);
    VirtRISC_MemMap *map = VirtRISC_Machine_GetMemMap(cpu->machine, address);

    if (!map) {
        char msg[512];
        sprintf(msg, "Attempt to access unmapped memory address 0x%x", address);
        VirtRISC_Machine_HaltException(cpu->machine, msg);
        return;
    }

    switch (funct3) {
        case FUNCT3_LB: {
            Byte b = VirtRISC_MemMap_LoadByte(map, address);
            CPU_RegSet(cpu, rd, signExtendByte(b));
        } break;
        case FUNCT3_LH: {
            HWord hw = VirtRISC_MemMap_LoadHWord(map, address);
            CPU_RegSet(cpu, rd, signExtendHWord(hw));
        } break;
        case FUNCT3_LW: {
            Word w = VirtRISC_MemMap_LoadWord(map, address);
            CPU_RegSet(cpu, rd, w);
        } break;
        case FUNCT3_LBU: {
            Byte b = VirtRISC_MemMap_LoadByte(map, address);
            CPU_RegSet(cpu, rd, b);
        } break;
        case FUNCT3_LHU: {
            HWord hw = VirtRISC_MemMap_LoadHWord(map, address);
            CPU_RegSet(cpu, rd, hw);
        } break;
        default: {
            printf("Unknown I-type instruction opcode=0x%x rd=0x%x funct3=0x%x rs1=0x%x imm12=0x%x\n", opcode, rd, funct3, rs1, imm12);
        }
    }
}

void CPU_ExecuteI_Sys(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word imm12) {
    // ECALL and EBREAK are differentiated by the value of imm12
    if (imm12) { // EBREAK
        if (cpu->machine->ebreakFunc) {
            cpu->machine->ebreakFunc(cpu->machine);
        }
    } else {
        if (cpu->machine->ecallFunc) {
            cpu->machine->ecallFunc(cpu->machine);
        }
    }
}

void CPU_ExecuteU(VirtRISC_CPU *cpu, Word opcode, Word rd, Word imm20) {
    switch (opcode) {
        case OPCODE_LUI: {
            cpu->r[rd] = (imm20 << IMM20_SHIFT);
        } break;
        case OPCODE_AUIPC: {
            cpu->r[rd] = (imm20 << IMM20_SHIFT) + (cpu->pc - 4);
        } break;
        default: {
            printf("Unknown U-type instruction opcode=0x%x rd=0x%x imm20=0x%x\n", opcode, rd, imm20);
        }
    }
}

void CPU_ExecuteJ_JAL(VirtRISC_CPU *cpu, Word opcode, Word rd, Word offset20) {
    CPU_RegSet(cpu, rd, cpu->pc); // Store the instruction address following this jump ins to rd
    cpu->pc = cpu->pc - 4 + signExtendOffset20(offset20);
}

void CPU_ExecuteR(VirtRISC_CPU *cpu, Word opcode, Word rd, Word funct3, Word rs1, Word rs2, Word funct7) {
    if (rd == 0) {
        return;
    }

    switch(funct3) {
        case FUNCT3_ADD: { // FUNCT3_SUB shares the same opcode
            switch (funct7) {
                case FUNCT7_ADD: {
                    cpu->r[rd] = cpu->r[rs1] + cpu->r[rs2];
                } break;
                case FUNCT7_SUB: {
                    cpu->r[rd] = cpu->r[rs1] - cpu->r[rs2];
                } break;
                default: {
                    printf("Unknown funct7 operand of FUNCT3_ADD/FUNCT3_SUB: funct7=0x%x\n", funct7);
                }
            }
        } break;
        case FUNCT3_SLL: {
            cpu->r[rd] = cpu->r[rs1] << (cpu->r[rs2] & 0b11111);
        } break;
        case FUNCT3_SLT: {
            cpu->r[rd] = (Signed) cpu->r[rs1] < (Signed) cpu->r[rs2];
        } break;
        case FUNCT3_SLTU: {
            cpu->r[rd] = cpu->r[rs1] < cpu->r[rs2];
        } break;
        case FUNCT3_XOR: {
            cpu->r[rd] = cpu->r[rs1] ^ cpu->r[rs2];
        } break;
        case FUNCT3_AND: {
            cpu->r[rd] = cpu->r[rs1] & cpu->r[rs2];
        } break;
        case FUNCT3_OR: {
            cpu->r[rd] = cpu->r[rs1] | cpu->r[rs2];
        } break;
        case FUNCT3_SRL: { // FUNCT3_SRA shares the same opcode
            Word shiftAmt = cpu->r[rs2] & 0b11111;
            switch (funct7) {
                case FUNCT7_SRL: {
                    cpu->r[rd] = cpu->r[rs1] >> shiftAmt;
                } break;
                case FUNCT7_SRA: {
                    cpu->r[rd] = (Word) ((Signed) cpu->r[rs1] >> shiftAmt);
                } break;
                default: {
                    printf("Unknown funct7 operand of FUNCT3_SRL/FUNCT3_SRA: funct7=0x%x\n", funct7);
                }
            }
        } break;
        default: {
            printf("Unknown R-type instruction opcode=0x%x rd=0x%x funct3=0x%x rs1=0x%x rs2=0x%x funct7=0x%x\n", opcode, rd, funct3, rs1, rs2, funct7);
        }
    }
}

void CPU_ExecuteS_Store(VirtRISC_CPU *cpu, Word opcode, Word funct3, Word rs1, Word rs2, Word imm12) {
    Word address = CPU_RegGet(cpu, rs1) + signExtendImm12(imm12);
    VirtRISC_MemMap *map = VirtRISC_Machine_GetMemMap(cpu->machine, address);

    if (!map) {
        char msg[512];
        sprintf(msg, "Attempt to access unmapped memory address 0x%x", address);
        VirtRISC_Machine_HaltException(cpu->machine, msg);
        return;
    }

    switch (funct3) {
        case FUNCT3_SB: {
            Byte b = CPU_RegGet(cpu, rs2) & 0xFF;
            VirtRISC_MemMap_StoreByte(map, address, b);
        } break;
        case FUNCT3_SH: {
            HWord h = CPU_RegGet(cpu, rs2) & 0xFFFF;
            VirtRISC_MemMap_StoreHWord(map, address, h);
        } break;
        case FUNCT3_SW: {
            Word w = CPU_RegGet(cpu, rs2);
            VirtRISC_MemMap_StoreWord(map, address, w);
        } break;
        default: {
            printf("Unknown S-type instruction opcode=0x%x funct3=0x%x rs1=0x%x rs2=0x%x imm12=0x%x\n", opcode, funct3, rs1, rs2, imm12);
        }
    }
}

void CPU_ExecuteB(VirtRISC_CPU *cpu, Word opcode, Word funct3, Word rs1, Word rs2, Word imm12) {
    Word offset = signExtendImm12B(imm12);
    Word jmpAddress = (cpu->pc - 4) + offset;
    switch (funct3) {
        case FUNCT3_BEQ: {
            if (CPU_RegGet(cpu, rs1) == CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        case FUNCT3_BNE: {
            if (CPU_RegGet(cpu, rs1) != CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        case FUNCT3_BLT: {
            if ((Signed) CPU_RegGet(cpu, rs1) < (Signed) CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        case FUNCT3_BGE: {
            if ((Signed) CPU_RegGet(cpu, rs1) >= (Signed) CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        case FUNCT3_BLTU: {
            if (CPU_RegGet(cpu, rs1) < CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        case FUNCT3_BGEU: {
            if (CPU_RegGet(cpu, rs1) >= CPU_RegGet(cpu, rs2)) {
                cpu->pc = jmpAddress;
            }
        } break;
        default: {
            printf("Unknown B-type instruction opcode=0x%x funct3=0x%x rs1=0x%x rs2=0x%x imm12=0x%x\n", opcode, funct3, rs1, rs2, imm12);
        }
    }
}

Word CPU_RegGet(VirtRISC_CPU *cpu, Word r) {
    assert(r >= 0 && r < XLEN);
    return cpu->r[r];
}

void CPU_RegSet(VirtRISC_CPU *cpu, Word r, Word val) {
    assert(r >= 0 && r < XLEN);
    if (r > 0) { // Cannot write to R0
        cpu->r[r] = val;
    }
}