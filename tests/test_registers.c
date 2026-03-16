#include <assert.h>
#include <stdio.h>   /* for snprintf */
#include <stddef.h>  /* for NULL */
#include "registers.h"

static void test_valid_registers(void) {
    for (int i = 0; i <= 31; ++i) {
        char buf[8];
        int n = snprintf(buf, sizeof(buf), "x%d", i);
        (void)n; /* silence unused warning if any */
        int parsed = parse_register(buf);
        assert(parsed == i);
    }
}

static void test_invalid_registers(void) {
    /* Out of range */
    assert(parse_register("x32") == -1);
    assert(parse_register("x100") == -1);
    assert(parse_register("x-1") == -1);

    /* Wrong prefix or format */
    assert(parse_register("y0") == -1);
    assert(parse_register("x") == -1);
    assert(parse_register("x3a") == -1);
    assert(parse_register("ax3") == -1);
    assert(parse_register("") == -1);

    /* Case sensitivity / allowed forms */
    assert(parse_register("X0") == 0);

    /* Null pointer */
    assert(parse_register(NULL) == -1);
}

int main(void) {
    test_valid_registers();
    test_invalid_registers();
    return 0;
}

