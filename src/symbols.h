#ifndef SYMBOLS_H
#define SYMBOLS_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    char     name[64];
    uint32_t addr;
} symbol_t;

/* Simple fixed-size table of symbols. */
typedef struct {
    symbol_t *data;
    size_t    capacity;
    size_t    count;
} symbol_table_t;

/* Initialize a symbol table with caller-provided storage. */
void symbols_init(symbol_table_t *tbl, symbol_t *storage, size_t capacity);

/* Add new symbol; returns 1 on success, 0 on failure (duplicate or full). */
int symbols_add(symbol_table_t *tbl, const char *name, uint32_t addr);

/* Find symbol by name; returns pointer to symbol or NULL if not found. */
symbol_t *symbols_find(symbol_table_t *tbl, const char *name);

#endif /* SYMBOLS_H */

