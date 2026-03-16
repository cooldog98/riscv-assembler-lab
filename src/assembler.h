#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stddef.h>
#include <stdint.h>

/*
 * assemble
 *  - lines: array of pointers to NUL-terminated assembly source lines
 *  - line_count: number of entries in lines
 *  - out: buffer for encoded 32-bit instructions
 *  - out_capacity: number of 32-bit words available in out
 *
 * Returns:
 *  - >= 0: number of encoded instructions written to out
 *  - -1 on error (e.g., unknown mnemonic, symbol not found, buffer too small)
 */
int assemble(const char **lines, size_t line_count,
             uint32_t *out, size_t out_capacity);

#endif /* ASSEMBLER_H */

