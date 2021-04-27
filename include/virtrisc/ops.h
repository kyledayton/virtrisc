#ifndef VIRTRISC_OPS_H
#define VIRTRISC_OPS_H

/** I-type Ops **/
#define OPCODE_ADDI     0b0010011
#define OPCODE_SLTI     0b0010011
#define OPCODE_SLTIU    0b0010011
#define OPCODE_XORI     0b0010011
#define OPCODE_ORI      0b0010011
#define OPCODE_ANDI     0b0010011
#define OPCODE_SLLI     0b0010011
#define OPCODE_SRLI     0b0010011
#define OPCODE_SRAI     0b0010011
#define OPCODE_LB       0b0000011
#define OPCODE_LH       0b0000011
#define OPCODE_LW       0b0000011
#define OPCODE_LBU      0b0000011
#define OPCODE_LHU      0b0000011
#define OPCODE_JALR     0b1100111
#define OPCODE_EBREAK   0b1110011
#define OPCODE_ECALL    0b1110011

#define FUNCT3_ADDI     0b000
#define FUNCT3_SLTI     0b010
#define FUNCT3_SLTIU    0b011
#define FUNCT3_XORI     0b100
#define FUNCT3_XORI     0b100
#define FUNCT3_ORI      0b110
#define FUNCT3_ANDI     0b111
#define FUNCT3_SLLI     0b001
#define FUNCT3_SRLI     0b101
#define FUNCT3_SRAI     0b101
#define FUNCT3_LB       0b000
#define FUNCT3_LH       0b001
#define FUNCT3_LW       0b010
#define FUNCT3_LBU      0b100
#define FUNCT3_LHU      0b101
#define FUNCT3_JALR     0b000
#define FUNCT3_EBREAK   0b000
#define FUNCT3_ECALL    0b000

/** U-type Ops **/
#define OPCODE_LUI      0b0110111
#define OPCODE_AUIPC    0b0010111

/** R-type Ops **/
#define OPCODE_ADD      0b0110011
#define OPCODE_SUB      0b0110011
#define OPCODE_SLL      0b0110011
#define OPCODE_SLT      0b0110011
#define OPCODE_SLTU     0b0110011
#define OPCODE_XOR      0b0110011
#define OPCODE_SRL      0b0110011
#define OPCODE_SRA      0b0110011
#define OPCODE_OR       0b0110011
#define OPCODE_AND      0b0110011

#define FUNCT3_ADD      0b000
#define FUNCT3_SUB      0b000
#define FUNCT3_SLL      0b001
#define FUNCT3_SLT      0b010
#define FUNCT3_SLTU     0b011
#define FUNCT3_XOR      0b100
#define FUNCT3_SRL      0b101
#define FUNCT3_SRA      0b101
#define FUNCT3_OR       0b110
#define FUNCT3_AND      0b111

#define FUNCT7_ADD      0b0000000
#define FUNCT7_SUB      0b0100000
#define FUNCT7_SLL      0b0000000
#define FUNCT7_SLT      0b0000000
#define FUNCT7_SLTU     0b0000000
#define FUNCT7_XOR      0b0000000
#define FUNCT7_SRL      0b0000000
#define FUNCT7_SRA      0b0100000
#define FUNCT7_OR       0b0000000
#define FUNCT7_AND      0b0000000

/** J-type Ops **/
#define OPCODE_JAL      0b1101111

/** S-type Ops **/
#define OPCODE_SB       0b0100011
#define OPCODE_SH       0b0100011
#define OPCODE_SW       0b0100011

#define FUNCT3_SB       0b000
#define FUNCT3_SH       0b001
#define FUNCT3_SW       0b010

/** B-type Ops **/
#define OPCODE_BEQ      0b1100011
#define OPCODE_BNE      0b1100011
#define OPCODE_BLT      0b1100011
#define OPCODE_BGE      0b1100011
#define OPCODE_BLTU     0b1100011
#define OPCODE_BGEU     0b1100011

#define FUNCT3_BEQ      0b000
#define FUNCT3_BNE      0b001
#define FUNCT3_BLT      0b100
#define FUNCT3_BGE      0b101
#define FUNCT3_BLTU     0b110
#define FUNCT3_BGEU     0b111

/** Registers **/
#define R0 0
#define R1 1
#define R2 2
#define R3 3
#define R4 4
#define R5 5
#define R6 6
#define R7 7
#define R8 8
#define R9 9
#define R10 10
#define R11 11
#define R12 12
#define R13 13
#define R14 14
#define R15 15
#define R16 16
#define R17 17
#define R18 18
#define R19 19
#define R20 20
#define R21 21
#define R22 22
#define R23 23
#define R24 24
#define R25 25
#define R26 26
#define R27 27
#define R28 28
#define R29 29
#define R30 30
#define R31 31

#endif // VIRTRISC_OPS_H
