#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include "registers.h"

int parse_register(const char *name) {
    if (name == NULL) {
        return -1;
    }

    /* Expect format "x<number>" with no leading/trailing spaces */
    if (name[0] != 'x' && name[0] != 'X') {
        return -1;
    }

    /* Ensure there is at least one digit after 'x' */
    if (!isdigit((unsigned char)name[1])) {
        return -1;
    }

    char *endptr = NULL;
    long reg = strtol(name + 1, &endptr, 10);

    /* strtol must consume all remaining characters */
    if (endptr == NULL || *endptr != '\0') {
        return -1;
    }

    if (reg < 0 || reg > 31) {
        return -1;
    }

    return (int)reg;
}

