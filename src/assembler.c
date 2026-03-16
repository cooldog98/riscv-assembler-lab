#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "assembler.h"
#include "parser.h"
#include "registers.h"
#include "symbols.h"
#include "instr_table.h"
#include "encode.h"

/* Helper: find instruction descriptor by mnemonic */
static const instr_desc_t *find_instr(const char *mnemonic) {
    for (unsigned int i = 0; i < instr_table_size; ++i) {
        if (strcmp(instr_table[i].name, mnemonic) == 0) {
            return &instr_table[i];
        }
    }
    return NULL;
}

int assemble(const char **lines, size_t line_count,
             uint32_t *out, size_t out_capacity) {
    char label[64];
    char mnemonic[32];
    char operands[128];
    symbol_t sym_storage[256];
    symbol_table_t symtab;
    size_t pc = 0; /* in bytes */

    if (!lines || !out) {
        return -1;
    }

    symbols_init(&symtab, sym_storage, 256);

    /* PASS 1: record labels with their byte addresses */
    for (size_t i = 0; i < line_count; ++i) {
        const char *line = lines[i];
        if (!line) {
            continue;
        }

        if (!parse_line(line, label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands)) {
            continue; /* skip empty / whitespace-only lines */
        }

        if (label[0] != '\0') {
            /* Ignore duplicate label failures for simplicity */
            symbols_add(&symtab, label, (uint32_t)pc);
        }

        /* Each instruction is 4 bytes */
        pc += 4;
    }

    /* PASS 2: encode instructions */
    pc = 0;
    size_t out_idx = 0;

    for (size_t i = 0; i < line_count; ++i) {
        const char *line = lines[i];
        if (!line) {
            continue;
        }

        if (!parse_line(line, label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands)) {
            continue;
        }

        if (out_idx >= out_capacity) {
            return -1;
        }

        const instr_desc_t *desc = find_instr(mnemonic);
        if (!desc) {
            return -1;
        }

        uint32_t word = 0;

        /* Operand patterns supported:
         *  - R-type:            "rd, rs1, rs2"
         *  - I-type arithmetic: "rd, rs1, imm"
         *  - I-type load:       "rd, imm(rs1)"
         *  - S-type store:      "rs2, imm(rs1)"
         *  - B-type branch:     "rs1, rs2, label"
         *  - J-type jump:       "rd, label"
         *  - U-type:            "rd, imm20"
         */
        if (desc->fmt == FMT_R) {
            int rd, rs1, rs2;
            char r1[16], r2[16], r3[16];
            if (sscanf(operands, " %15[^,] , %15[^,] , %15s", r1, r2, r3) != 3) {
                return -1;
            }
            rd  = parse_register(r1);
            rs1 = parse_register(r2);
            rs2 = parse_register(r3);
            if (rd < 0 || rs1 < 0 || rs2 < 0) {
                return -1;
            }
            word = encode_r(desc->opcode, (uint32_t)rd,
                            desc->funct3, (uint32_t)rs1,
                            (uint32_t)rs2, desc->funct7);
        } else if (desc->fmt == FMT_I) {
            int rd, rs1;
            char r1[16], r2[16];
            int imm = 0;

            if (strcmp(desc->name, "lw") == 0 ||
                strcmp(desc->name, "lb") == 0 ||
                strcmp(desc->name, "lh") == 0 ||
                strcmp(desc->name, "lbu") == 0 ||
                strcmp(desc->name, "lhu") == 0) {
                /* load: rd, imm(rs1) */
                if (sscanf(operands, " %15[^,] , %d ( %15[^) ] )",
                           r1, &imm, r2) != 3) {
                    return -1;
                }
                rd  = parse_register(r1);
                rs1 = parse_register(r2);
            } else if (strcmp(desc->name, "jalr") == 0) {
                /* jalr: rd, rs1, imm */
                if (sscanf(operands, " %15[^,] , %15[^,] , %d",
                           r1, r2, &imm) != 3) {
                    return -1;
                }
                rd  = parse_register(r1);
                rs1 = parse_register(r2);
            } else {
                /* arithmetic immediate: rd, rs1, imm */
                if (sscanf(operands, " %15[^,] , %15[^,] , %d",
                           r1, r2, &imm) != 3) {
                    return -1;
                }
                rd  = parse_register(r1);
                rs1 = parse_register(r2);
            }

            if (rd < 0 || rs1 < 0) {
                return -1;
            }

            word = encode_i(desc->opcode, (uint32_t)rd,
                            desc->funct3, (uint32_t)rs1,
                            imm);
        } else if (desc->fmt == FMT_S) {
            /* store: rs2, imm(rs1) */
            int rs1, rs2;
            char rbase[16], rsrc[16];
            int imm = 0;

            if (sscanf(operands, " %15[^,] , %d ( %15[^) ] )",
                       rsrc, &imm, rbase) != 3) {
                return -1;
            }

            rs2 = parse_register(rsrc);
            rs1 = parse_register(rbase);
            if (rs1 < 0 || rs2 < 0) {
                return -1;
            }

            word = encode_s(desc->opcode, desc->funct3,
                            (uint32_t)rs1, (uint32_t)rs2,
                            imm);
        } else if (desc->fmt == FMT_B) {
            /* branch: rs1, rs2, label */
            char r1[16], r2[16], lbl[64];
            if (sscanf(operands, " %15[^,] , %15[^,] , %63s",
                       r1, r2, lbl) != 3) {
                return -1;
            }
            int rs1 = parse_register(r1);
            int rs2 = parse_register(r2);
            if (rs1 < 0 || rs2 < 0) {
                return -1;
            }

            symbol_t *target = symbols_find(&symtab, lbl);
            if (!target) {
                return -1;
            }

            int32_t offset = (int32_t)target->addr - (int32_t)pc;
            word = encode_b(desc->opcode, desc->funct3,
                            (uint32_t)rs1, (uint32_t)rs2,
                            offset);
        } else if (desc->fmt == FMT_J) {
            /* jal: rd, label */
            char r1[16], lbl[64];
            if (sscanf(operands, " %15[^,] , %63s", r1, lbl) != 2) {
                return -1;
            }
            int rd = parse_register(r1);
            if (rd < 0) {
                return -1;
            }
            symbol_t *target = symbols_find(&symtab, lbl);
            if (!target) {
                return -1;
            }
            int32_t offset = (int32_t)target->addr - (int32_t)pc;
            word = encode_j(desc->opcode, (uint32_t)rd, offset);
        } else if (desc->fmt == FMT_U) {
            /* U-type (lui, auipc): rd, imm20 */
            int rd;
            char r1[16];
            int imm = 0;
            if (sscanf(operands, " %15[^,] , %d", r1, &imm) != 2) {
                return -1;
            }
            rd = parse_register(r1);
            if (rd < 0) {
                return -1;
            }
            uint32_t uimm = (uint32_t)imm;
            uint32_t inst = 0;
            inst |= (uimm & 0xFFFFF000u);      /* imm[31:12] */
            inst |= ((uint32_t)rd & 0x1Fu) << 7;
            inst |= (desc->opcode & 0x7Fu);
            word = inst;
        } else {
            return -1;
        }

        out[out_idx++] = word;
        pc += 4;
    }

    return (int)out_idx;
}

