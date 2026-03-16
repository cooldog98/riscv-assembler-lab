#ifndef PARSER_H
#define PARSER_H

#include <stddef.h>

/*
 * parse_line
 *  - Extracts optional label, mnemonic, and operands from one assembly line.
 *  - line: input string (not modified)
 *  - label / mnemonic / operands: output buffers (may be NULL if not needed)
 *  - *_size: sizes of the corresponding output buffers
 *
 *  Returns 1 on success, 0 if the line does not contain a mnemonic
 *  (e.g. empty line).
 */
int parse_line(const char *line,
               char *label, size_t label_size,
               char *mnemonic, size_t mnemonic_size,
               char *operands, size_t operands_size);

#endif /* PARSER_H */

