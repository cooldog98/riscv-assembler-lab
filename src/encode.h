#ifndef ENCODE_H
#define ENCODE_H

#include <stdint.h>

/* R-type: funct7 | rs2 | rs1 | funct3 | rd | opcode */
uint32_t encode_r(uint32_t opcode,
                  uint32_t rd,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  uint32_t funct7);

/* I-type: imm[11:0] | rs1 | funct3 | rd | opcode */
uint32_t encode_i(uint32_t opcode,
                  uint32_t rd,
                  uint32_t funct3,
                  uint32_t rs1,
                  int32_t imm);

/* S-type: imm[11:5] | rs2 | rs1 | funct3 | imm[4:0] | opcode */
uint32_t encode_s(uint32_t opcode,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  int32_t imm);

/* B-type: imm[12|10:5] | rs2 | rs1 | funct3 | imm[4:1|11] | opcode */
uint32_t encode_b(uint32_t opcode,
                  uint32_t funct3,
                  uint32_t rs1,
                  uint32_t rs2,
                  int32_t imm);

/* J-type: imm[20|10:1|11|19:12] | rd | opcode */
uint32_t encode_j(uint32_t opcode,
                  uint32_t rd,
                  int32_t imm);

#endif /* ENCODE_H */

