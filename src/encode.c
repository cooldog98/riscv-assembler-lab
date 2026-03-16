#include "encode.h"

/* Helper: mask a value to n bits */
static inline uint32_t mask_bits(int n, uint32_t value) {
    return value & ((1u << n) - 1u);
}

uint32_t encode_r(uint32_t opcode,
                  uint32_t rd,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  uint32_t funct7) {
    uint32_t inst = 0;
    inst |= mask_bits(7, opcode);
    inst |= mask_bits(5, rd)     << 7;
    inst |= mask_bits(3, funct3) << 12;
    inst |= mask_bits(5, rs1)    << 15;
    inst |= mask_bits(5, rs2)    << 20;
    inst |= mask_bits(7, funct7) << 25;
    return inst;
}

uint32_t encode_i(uint32_t opcode,
                  uint32_t rd,
                  uint32_t funct3,
                  uint32_t rs1,
                  int32_t imm) {
    uint32_t inst = 0;
    uint32_t uimm = (uint32_t)imm;
    inst |= mask_bits(7, opcode);
    inst |= mask_bits(5, rd)     << 7;
    inst |= mask_bits(3, funct3) << 12;
    inst |= mask_bits(5, rs1)    << 15;
    inst |= mask_bits(12, uimm)  << 20; /* imm[11:0] */
    return inst;
}

uint32_t encode_s(uint32_t opcode,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  int32_t imm) {
    uint32_t inst = 0;
    uint32_t uimm = (uint32_t)imm;
    uint32_t imm_lo = mask_bits(5, uimm);           /* imm[4:0] */
    uint32_t imm_hi = mask_bits(7, uimm >> 5);      /* imm[11:5] */

    inst |= mask_bits(7, opcode);
    inst |= imm_lo               << 7;   /* imm[4:0] -> bits [11:7] */
    inst |= mask_bits(3, funct3) << 12;
    inst |= mask_bits(5, rs1)    << 15;
    inst |= mask_bits(5, rs2)    << 20;
    inst |= imm_hi               << 25;  /* imm[11:5] -> bits [31:25] */
    return inst;
}

uint32_t encode_b(uint32_t opcode,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  int32_t imm) {
    uint32_t inst = 0;
    uint32_t uimm = (uint32_t)imm;

    /* B-type immediate layout (12-bit signed, multiples of 2):
     * imm[12]   -> bit 31
     * imm[10:5] -> bits 30:25
     * imm[4:1]  -> bits 11:8
     * imm[11]   -> bit 7
     *
     * Note: least significant bit is always zero (word-aligned branches).
     */
    uint32_t bit12   = (uimm >> 12) & 0x1;
    uint32_t bits10_5= (uimm >> 5)  & 0x3F;
    uint32_t bits4_1 = (uimm >> 1)  & 0xF;
    uint32_t bit11   = (uimm >> 11) & 0x1;

    inst |= mask_bits(7, opcode);
    inst |= bits4_1               << 8;   /* imm[4:1]  -> bits [11:8]  */
    inst |= bit11                 << 7;   /* imm[11]   -> bit 7        */
    inst |= mask_bits(3, funct3)  << 12;
    inst |= mask_bits(5, rs1)     << 15;
    inst |= mask_bits(5, rs2)     << 20;
    inst |= bits10_5              << 25;  /* imm[10:5] -> bits [30:25] */
    inst |= bit12                 << 31;  /* imm[12]   -> bit 31       */

    return inst;
}

uint32_t encode_j(uint32_t opcode,
                  uint32_t rd,
                  int32_t imm) {
    uint32_t inst = 0;
    uint32_t uimm = (uint32_t)imm;

    /* J-type immediate layout (20-bit signed, multiples of 2):
     * imm[20]    -> bit 31
     * imm[10:1]  -> bits 30:21
     * imm[11]    -> bit 20
     * imm[19:12] -> bits 19:12
     */
    uint32_t bit20    = (uimm >> 20) & 0x1;
    uint32_t bits10_1 = (uimm >> 1)  & 0x3FF;
    uint32_t bit11    = (uimm >> 11) & 0x1;
    uint32_t bits19_12= (uimm >> 12) & 0xFF;

    inst |= mask_bits(7, opcode);
    inst |= mask_bits(5, rd)      << 7;
    inst |= bits19_12             << 12;  /* imm[19:12] -> bits [19:12] */
    inst |= bit11                 << 20;  /* imm[11]    -> bit 20       */
    inst |= bits10_1              << 21;  /* imm[10:1]  -> bits [30:21] */
    inst |= bit20                 << 31;  /* imm[20]    -> bit 31       */

    return inst;
}

