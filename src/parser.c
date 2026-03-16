#include <ctype.h>
#include <string.h>

#include "parser.h"

static const char *skip_spaces(const char *s) {
    while (*s == ' ' || *s == '\t') {
        ++s;
    }
    return s;
}

static void trim_trailing_spaces(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == ' ' || s[len - 1] == '\t')) {
        s[--len] = '\0';
    }
}

int parse_line(const char *line,
               char *label, size_t label_size,
               char *mnemonic, size_t mnemonic_size,
               char *operands, size_t operands_size) {
    char local[256];
    const char *p;
    char *colon;
    char *rest;

    if (label && label_size > 0) {
        label[0] = '\0';
    }
    if (mnemonic && mnemonic_size > 0) {
        mnemonic[0] = '\0';
    }
    if (operands && operands_size > 0) {
        operands[0] = '\0';
    }

    if (line == NULL) {
        return 0;
    }

    /* Copy at most sizeof(local)-1 characters and null-terminate */
    strncpy(local, line, sizeof(local) - 1);
    local[sizeof(local) - 1] = '\0';

    /* Trim leading spaces */
    p = skip_spaces(local);
    if (*p == '\0') {
        return 0; /* empty / whitespace-only line */
    }

    /* Move leading spaces out of the way if needed */
    if (p != local) {
        memmove(local, p, strlen(p) + 1);
    }

    /* Look for label of the form "label:" at the start of the line */
    colon = strchr(local, ':');
    if (colon != NULL) {
        /* Ensure colon comes before any whitespace to be considered a label */
        char *ws = local;
        while (*ws != '\0' && *ws != ' ' && *ws != '\t' && ws != colon) {
            ++ws;
        }
        if (ws == colon) {
            *colon = '\0';
            trim_trailing_spaces(local);
            if (label && label_size > 0) {
                strncpy(label, local, label_size - 1);
                label[label_size - 1] = '\0';
            }
            rest = colon + 1;
        } else {
            rest = local;
        }
    } else {
        rest = local;
    }

    /* Skip spaces before mnemonic */
    rest = (char *)skip_spaces(rest);
    if (*rest == '\0') {
        return 0;
    }

    /* Extract mnemonic: first token until whitespace */
    char *mn_start = rest;
    while (*rest != '\0' && *rest != ' ' && *rest != '\t') {
        ++rest;
    }

    if (*rest != '\0') {
        *rest++ = '\0';
    }

    if (mnemonic && mnemonic_size > 0) {
        strncpy(mnemonic, mn_start, mnemonic_size - 1);
        mnemonic[mnemonic_size - 1] = '\0';
    }

    /* Remaining part (after skipping spaces) is operands */
    rest = (char *)skip_spaces(rest);
    if (operands && operands_size > 0) {
        strncpy(operands, rest, operands_size - 1);
        operands[operands_size - 1] = '\0';
        trim_trailing_spaces(operands);
    }

    return 1;
}

