#include <string.h>

#include "symbols.h"

void symbols_init(symbol_table_t *tbl, symbol_t *storage, size_t capacity) {
    if (!tbl) return;
    tbl->data = storage;
    tbl->capacity = capacity;
    tbl->count = 0;
}

int symbols_add(symbol_table_t *tbl, const char *name, uint32_t addr) {
    size_t i;
    if (!tbl || !name) {
        return 0;
    }

    /* Reject duplicates */
    for (i = 0; i < tbl->count; ++i) {
        if (strcmp(tbl->data[i].name, name) == 0) {
            return 0;
        }
    }

    if (tbl->count >= tbl->capacity) {
        return 0;
    }

    symbol_t *sym = &tbl->data[tbl->count++];
    strncpy(sym->name, name, sizeof(sym->name) - 1);
    sym->name[sizeof(sym->name) - 1] = '\0';
    sym->addr = addr;
    return 1;
}

symbol_t *symbols_find(symbol_table_t *tbl, const char *name) {
    size_t i;
    if (!tbl || !name) {
        return NULL;
    }

    for (i = 0; i < tbl->count; ++i) {
        if (strcmp(tbl->data[i].name, name) == 0) {
            return &tbl->data[i];
        }
    }
    return NULL;
}

