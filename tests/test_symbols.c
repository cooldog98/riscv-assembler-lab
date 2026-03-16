#include <assert.h>
#include <string.h>

#include "symbols.h"

static void test_add_and_find(void) {
    symbol_t storage[8];
    symbol_table_t tbl;
    symbols_init(&tbl, storage, 8);

    assert(symbols_add(&tbl, "start", 0x00000000));
    assert(symbols_add(&tbl, "loop",  0x00000004));
    assert(symbols_add(&tbl, "end",   0x00000010));

    symbol_t *s;

    s = symbols_find(&tbl, "start");
    assert(s != NULL);
    assert(strcmp(s->name, "start") == 0);
    assert(s->addr == 0x00000000);

    s = symbols_find(&tbl, "loop");
    assert(s != NULL);
    assert(strcmp(s->name, "loop") == 0);
    assert(s->addr == 0x00000004);

    s = symbols_find(&tbl, "end");
    assert(s != NULL);
    assert(strcmp(s->name, "end") == 0);
    assert(s->addr == 0x00000010);
}

static void test_duplicates_and_full(void) {
    symbol_t storage[2];
    symbol_table_t tbl;
    symbols_init(&tbl, storage, 2);

    assert(symbols_add(&tbl, "a", 1));
    assert(symbols_add(&tbl, "b", 2));

    /* Table is full; further adds fail */
    assert(!symbols_add(&tbl, "c", 3));

    /* Duplicate name fails */
    symbol_t storage2[4];
    symbols_init(&tbl, storage2, 4);

    assert(symbols_add(&tbl, "dup", 0x10));
    assert(!symbols_add(&tbl, "dup", 0x20));
}

static void test_not_found(void) {
    symbol_t storage[4];
    symbol_table_t tbl;
    symbols_init(&tbl, storage, 4);

    assert(symbols_add(&tbl, "foo", 0x100));
    assert(symbols_add(&tbl, "bar", 0x200));

    symbol_t *s = symbols_find(&tbl, "baz");
    assert(s == NULL);
}

int main(void) {
    test_add_and_find();
    test_duplicates_and_full();
    test_not_found();
    return 0;
}

