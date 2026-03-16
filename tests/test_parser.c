#include <assert.h>
#include <string.h>

#include "parser.h"

static void test_no_label_with_operands(void) {
    char label[32], mnemonic[32], operands[64];
    int ok = parse_line("add x1, x2, x3", label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands);
    assert(ok == 1);
    assert(strcmp(label, "") == 0);
    assert(strcmp(mnemonic, "add") == 0);
    assert(strcmp(operands, "x1, x2, x3") == 0);
}

static void test_label_with_operands(void) {
    char label[32], mnemonic[32], operands[64];
    int ok = parse_line("loop: add x1, x2, x3", label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands);
    assert(ok == 1);
    assert(strcmp(label, "loop") == 0);
    assert(strcmp(mnemonic, "add") == 0);
    assert(strcmp(operands, "x1, x2, x3") == 0);
}

static void test_extra_spaces(void) {
    char label[32], mnemonic[32], operands[64];
    int ok = parse_line("   start:   nop   ", label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands);
    assert(ok == 1);
    assert(strcmp(label, "start") == 0);
    assert(strcmp(mnemonic, "nop") == 0);
    assert(strcmp(operands, "") == 0);
}

static void test_no_operands(void) {
    char label[32], mnemonic[32], operands[64];
    int ok = parse_line("nop", label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands);
    assert(ok == 1);
    assert(strcmp(label, "") == 0);
    assert(strcmp(mnemonic, "nop") == 0);
    assert(strcmp(operands, "") == 0);
}

static void test_empty_and_spaces_only(void) {
    char label[32], mnemonic[32], operands[64];

    int ok = parse_line("", label, sizeof label,
                        mnemonic, sizeof mnemonic,
                        operands, sizeof operands);
    assert(ok == 0);

    ok = parse_line("   \t  ", label, sizeof label,
                    mnemonic, sizeof mnemonic,
                    operands, sizeof operands);
    assert(ok == 0);
}

int main(void) {
    test_no_label_with_operands();
    test_label_with_operands();
    test_extra_spaces();
    test_no_operands();
    test_empty_and_spaces_only();
    return 0;
}

