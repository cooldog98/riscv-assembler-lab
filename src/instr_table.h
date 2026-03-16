#ifndef INSTR_TABLE_H
#define INSTR_TABLE_H

/* Instruction format codes (from CS61C reference card) */
typedef enum {
    FMT_R,
    FMT_I,
    FMT_S,
    FMT_B,
    FMT_U,
    FMT_J
} instr_format_t;

typedef struct {
    const char     *name;    /* mnemonic, e.g., "add" */
    instr_format_t  fmt;     /* FMT_R, FMT_I, ...     */
    unsigned int    opcode;  /* 7-bit opcode          */
    unsigned int    funct3;  /* 3-bit funct3 (if any) */
    unsigned int    funct7;  /* 7-bit funct7 (if any) */
} instr_desc_t;

extern const instr_desc_t instr_table[];
extern const unsigned int instr_table_size;

#endif /* INSTR_TABLE_H */

