#include <assert.h>
#include <stdint.h>
#include <string.h>

#include "assembler.h"
#include "instr_table.h"
#include "encode.h"

static const instr_desc_t *find_instr_desc(const char *name) {
    for (unsigned int i = 0; i < instr_table_size; ++i) {
        if (instr_table[i].name && strcmp(instr_table[i].name, name) == 0) {
            return &instr_table[i];
        }
    }
    return NULL;
}

static void test_simple_program(void) {
    const char *lines[] = {
        "start: addi x1, x0, 5",
        "loop:  addi x1, x1, -1",
        "       beq  x1, x0, end",
        "       jal  x0, loop",
        "end:   add  x2, x1, x0",
    };

    uint32_t out[8];
    int n = assemble(lines, 5, out, 8);
    assert(n == 5);

    const instr_desc_t *addi = find_instr_desc("addi");
    const instr_desc_t *beq  = find_instr_desc("beq");
    const instr_desc_t *jal  = find_instr_desc("jal");
    const instr_desc_t *add  = find_instr_desc("add");

    assert(addi && beq && jal && add);

    /* Expected encodings using the same helpers */
    uint32_t e0 = encode_i(addi->opcode, 1, addi->funct3, 0, 5);    /* addi x1, x0, 5 */
    uint32_t e1 = encode_i(addi->opcode, 1, addi->funct3, 1, -1);   /* addi x1, x1, -1 */

    /* PC addresses: start=0, loop=4, beq at 8, jal at 12, end=16 */
    int32_t beq_off = 16 - 8;   /* from beq at PC=8 to end at PC=16 */
    uint32_t e2 = encode_b(beq->opcode, beq->funct3, 1, 0, beq_off);

    int32_t jal_off = 4 - 12;   /* from jal at PC=12 back to loop at PC=4 */
    uint32_t e3 = encode_j(jal->opcode, 0, jal_off);

    uint32_t e4 = encode_r(add->opcode, 2, add->funct3, 1, 0, add->funct7); /* add x2, x1, x0 */

    assert(out[0] == e0);
    assert(out[1] == e1);
    assert(out[2] == e2);
    assert(out[3] == e3);
    assert(out[4] == e4);
}

int main(void) {
    test_simple_program();
    return 0;
}

