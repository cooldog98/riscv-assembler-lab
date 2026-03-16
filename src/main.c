#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "assembler.h"

static void usage(const char *prog) {
    fprintf(stderr, "Usage: %s <input.s>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 2;
    }

    const char *path = argv[1];
    FILE *fp = fopen(path, "r");
    if (!fp) {
        perror("fopen");
        return 1;
    }

    size_t cap = 64;
    size_t nlines = 0;
    char **lines = (char **)calloc(cap, sizeof(*lines));
    if (!lines) {
        fclose(fp);
        perror("calloc");
        return 1;
    }

    char buf[512];
    while (fgets(buf, sizeof buf, fp)) {
        /* Strip trailing newline(s) */
        size_t len = strlen(buf);
        while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == '\r')) {
            buf[--len] = '\0';
        }

        if (nlines == cap) {
            cap *= 2;
            char **tmp = (char **)realloc(lines, cap * sizeof(*lines));
            if (!tmp) {
                perror("realloc");
                fclose(fp);
                for (size_t i = 0; i < nlines; ++i) free(lines[i]);
                free(lines);
                return 1;
            }
            lines = tmp;
        }

        lines[nlines] = strdup(buf);
        if (!lines[nlines]) {
            perror("strdup");
            fclose(fp);
            for (size_t i = 0; i < nlines; ++i) free(lines[i]);
            free(lines);
            return 1;
        }
        ++nlines;
    }

    fclose(fp);

    /* Convert to const char* for assemble() */
    const char **src = (const char **)calloc(nlines, sizeof(*src));
    if (!src) {
        perror("calloc");
        for (size_t i = 0; i < nlines; ++i) free(lines[i]);
        free(lines);
        return 1;
    }
    for (size_t i = 0; i < nlines; ++i) {
        src[i] = lines[i];
    }

    uint32_t *out = (uint32_t *)calloc(nlines, sizeof(*out));
    if (!out) {
        perror("calloc");
        free(src);
        for (size_t i = 0; i < nlines; ++i) free(lines[i]);
        free(lines);
        return 1;
    }

    int ninstr = assemble(src, nlines, out, nlines);
    if (ninstr < 0) {
        fprintf(stderr, "assemble: error assembling '%s'\n", path);
        free(out);
        free(src);
        for (size_t i = 0; i < nlines; ++i) free(lines[i]);
        free(lines);
        return 1;
    }

    for (int i = 0; i < ninstr; ++i) {
        printf("0x%08x\n", out[i]);
    }

    free(out);
    free(src);
    for (size_t i = 0; i < nlines; ++i) free(lines[i]);
    free(lines);
    return 0;
}

