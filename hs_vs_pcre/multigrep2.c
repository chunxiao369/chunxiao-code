#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hs.h"

#define MAX_DATA_NUM 2

const char *g_expr[] = {
    "abc",
    "def",
    "foobar.*gh",
    "teakettle{4,10}",
    "ijkl[mMn]",
};

static int on_match(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx)
{
    printf("      id[%3d] %20s Match at offset from %3llu to %3llu\n", id, g_expr[id], from, to);
    return 0;
}

int main(int argc, char *argv[])
{
    int i;
    int mode;
    hs_stream_t *p_stream = NULL;
    hs_database_t *db = NULL;
    hs_scratch_t *scratch = NULL;
    hs_compile_error_t *compile_err = NULL;
    unsigned flags[] = {
        HS_FLAG_SINGLEMATCH,
        HS_FLAG_SINGLEMATCH,
        HS_FLAG_SINGLEMATCH,
        HS_FLAG_SINGLEMATCH,
        HS_FLAG_SINGLEMATCH,
    };
    unsigned ids[] = { 0, 1, 2, 3, 4};

    if (argc < 2) {
        mode = HS_MODE_NOSTREAM;
    } else {
        mode = HS_MODE_STREAM;
    }

    char *data[MAX_DATA_NUM] = {"abcdef teakettleeeeeeee i", "jklM"};
    fprintf(stdout, "    hyperscan version: %s, mode: %s\n", hs_version(),\
                    (HS_MODE_STREAM == mode) ? "stream": "block");

    hs_error_t err = hs_compile_multi(g_expr, flags, ids,
                                      sizeof(ids) / sizeof(unsigned),
                                      mode, NULL,
                                      &db, &compile_err);
    if (HS_SUCCESS != err) {
        printf("compile error!!!\n");
        return -1;
    }

    if (hs_alloc_scratch(db, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(db);
        return -1;
    }
    if (HS_MODE_STREAM == mode) {
        err = hs_open_stream(db, 0, &p_stream);
        if (HS_SUCCESS != err) {
            printf("open stream error!!!\n");
            return -1;
        }
    }

    for (i=0; i<MAX_DATA_NUM; i++) {
        fprintf(stdout, "    data: %s\n", data[i]);
        if (HS_MODE_STREAM == mode) {
            if (hs_scan_stream(p_stream, data[i], strlen(data[i]), 0, scratch, on_match, g_expr) != HS_SUCCESS) {
                fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
                hs_free_scratch(scratch);
                hs_free_database(db);
                return -1;
            }
        } else {
            if (hs_scan(db, data[i], strlen(data[i]), 0, scratch, on_match, g_expr) != HS_SUCCESS) {
                fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
                hs_free_scratch(scratch);
                hs_free_database(db);
                return -1;
            }
        }
    }

    if (HS_MODE_STREAM == mode) {
        hs_close_stream(p_stream, scratch, on_match, g_expr);
    }
    hs_free_scratch(scratch);
    hs_free_database(db);

    return 0;
}
