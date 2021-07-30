/*
 * Usage:
 *     ./simplegrep <pattern> <input file>
 *
 * Example:
 *     ./simplegrep int simplegrep.c
 */

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#include <hs/hs.h>

#define LEN_MAX_NUM     (1024)
#define LEN_PER_LINE    (128)
#define LEN_MAX_SUPPORT ((LEN_MAX_NUM) * (LEN_PER_LINE))

static int g_match = 0;
/**
 * This is the function that will be called for each match that occurs. @a ctx
 * is to allow you to have some application-specific state that you will get
 * access to for each match. In our simple example we're just going to use it
 * to pass in the pattern that was being searched for so we can print it out.
 */
static int eventHandler(unsigned int id, unsigned long long from, unsigned long long to, unsigned int flags, void *ctx)
{
    //printf("Match for pattern \"%s\" at offset %llu\n", (char *)ctx, to);
    g_match++;
    return 0;
}

/**
 * Fill a data buffer from the given filename, returning it and filling @a
 * length with its length. Returns NULL on failure.
 */
static char *readInputData(const char *inputFN, unsigned int *length, unsigned int *line_num)
{
    FILE *f = fopen(inputFN, "rb");
    if (!f) {
        fprintf(stderr, "ERROR: unable to open file \"%s\": %s\n", inputFN, strerror(errno));
        return NULL;
    }

    /* We use fseek/ftell to get our data length, in order to keep this example
     * code as portable as possible. */
    if (fseek(f, 0, SEEK_END) != 0) {
        fprintf(stderr, "ERROR: unable to seek file \"%s\": %s\n", inputFN, strerror(errno));
        fclose(f);
        return NULL;
    }
    long dataLen = ftell(f);
    if (dataLen < 0) {
        fprintf(stderr, "ERROR: ftell() failed: %s\n", strerror(errno));
        fclose(f);
        return NULL;
    }
    if (fseek(f, 0, SEEK_SET) != 0) {
        fprintf(stderr, "ERROR: unable to seek file \"%s\": %s\n", inputFN, strerror(errno));
        fclose(f);
        return NULL;
    }

    char *inputData = malloc(LEN_MAX_SUPPORT);
    if (!inputData) {
        fprintf(stderr, "ERROR: unable to malloc %ld bytes\n", dataLen);
        fclose(f);
        return NULL;
    }
    memset(inputData, 0, LEN_MAX_SUPPORT);

    /* Hyperscan's hs_scan function accepts length as an unsigned int, so we
     * limit the size of our buffer appropriately. */
    if ((unsigned long)dataLen > LEN_MAX_SUPPORT) {
        dataLen = LEN_MAX_SUPPORT;
        printf("WARNING: clipping data to %ld bytes\n", dataLen);
    } else if (dataLen == 0) {
        fprintf(stderr, "ERROR: input file \"%s\" is empty\n", inputFN);
        fclose(f);
        return NULL;
    }

    char *p = inputData;
    char buf[LEN_PER_LINE] = { 0 };
    size_t line_n = 0;
    while (fgets(buf, LEN_PER_LINE, f)) {
        if (line_n > LEN_MAX_NUM) {
            break;
        }
        memcpy(p + (line_n * LEN_PER_LINE), buf, LEN_PER_LINE);
        memset(buf, 0, LEN_PER_LINE);
        line_n++;
    }

    fclose(f);

    *length = (unsigned int)dataLen;
    *line_num = line_n;
    return inputData;
}

int main(int argc, char *argv[])
{
    const char *p_errmsg = NULL;
    pcre *p_pcre = NULL;
    int offset = -1;

    if (argc < 4) {
        fprintf(stderr, "Usage: %s <pattern> <input file> <h|p> [times]\n", argv[0]);
        return -1;
    }

    char *pattern = argv[1];
    char *inputFN = argv[2];
    char *type = argv[3];
    int grep_c = LEN_MAX_NUM;
    if (NULL != argv[4]) {
        grep_c = atoi(argv[4]);
    }
    if ('h' != *type && 'p' != *type) {
        fprintf(stderr, "Usage: %s <pattern> <input file> <h|p> [times]\n", argv[0]);
        return -1;
    }

    /* First, we attempt to compile the pattern provided on the command line.
     * We assume 'DOTALL' semantics, meaning that the '.' meta-character will
     * match newline characters. The compiler will analyse the given pattern and
     * either return a compiled Hyperscan database, or an error message
     * explaining why the pattern didn't compile.
     * explainaaaa why the pattern didn't compile.
     */
    hs_database_t *database;
    hs_compile_error_t *compile_err;
    if (hs_compile(pattern, HS_FLAG_SINGLEMATCH | HS_FLAG_DOTALL | HS_FLAG_MULTILINE, \
        HS_MODE_BLOCK, NULL, &database, &compile_err) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern \"%s\": %s\n", pattern, compile_err->message);
        hs_free_compile_error(compile_err);
        return -1;
    }

    /* Finally, we issue a call to hs_scan, which will search the input buffer
     * for the pattern represented in the bytecode. Note that in order to do
     * this, scratch space needs to be allocated with the hs_alloc_scratch
     * function. In typical usage, you would reuse this scratch space for many
     * calls to hs_scan, but as we're only doing one, we'll be allocating it
     * and deallocating it as soon as our matching is done.
     *
     * When matches occur, the specified callback function (eventHandler in
     * this file) will be called. Note that although it is reminiscent of
     * asynchronous APIs, Hyperscan operates synchronously: all matches will be
     * found, and all callbacks issued, *before* hs_scan returns.
     *
     * In this example, we provide the input pattern as the context pointer so
     * that the callback is able to print out the pattern that matched on each
     * match event.
     */
    hs_scratch_t *scratch = NULL;
    if (hs_alloc_scratch(database, &scratch) != HS_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        hs_free_database(database);
        return -1;
    }
    if (NULL == (p_pcre = pcre_compile(pattern, PCRE_DOTALL | PCRE_MULTILINE, &p_errmsg, &offset, NULL))) {
        printf("ErrMsg=%s, Offset=%d\n", p_errmsg, offset);
        hs_free_database(database);
        return -1;
    }

    /* Next, we read the input data file into a buffer. */
    unsigned int length;
    unsigned int line_num;
    unsigned int i;
    unsigned int j;
    char *inputData = readInputData(inputFN, &length, &line_num);
    char *p = NULL;
    if (!inputData) {
        hs_free_database(database);
        return -1;
    }

    printf("Get %u lines from file %s...\n", line_num, inputFN);
    if ('p' == *type) {
        printf("Scanning %u times with libpcre\n", grep_c);
    } else {
        printf("Scanning %u times with hyperscan\n", grep_c);
    }

    for (i = 0; i <= grep_c; i++) {
        if (i >= line_num) {
            j = (i % line_num);
            p = inputData + j * LEN_PER_LINE;
        } else {
            p = inputData + i * LEN_PER_LINE;
        }
        //printf("p: %s", p);
        if ('p' == *type) {
            if (pcre_exec(p_pcre, NULL, p, strlen(p), 0, 0, NULL, 0) < 0) {
                //printf("%s doesn't match %s\n", pText, pattern);
            } else {
                g_match++;
                //printf("%s matches %s\n", pText, pattern);
            }
        } else {
            if (hs_scan(database, p, strlen(p), 0, scratch, eventHandler, pattern) != HS_SUCCESS) {
                fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
                hs_free_scratch(scratch);
                free(inputData);
                hs_free_database(database);
                return -1;
            }
        }
    }
    printf("match counter: %d.\n", g_match);

    /* Scanning is complete, any matches have been handled, so now we just
     * clean up and exit.
     */
    hs_free_scratch(scratch);
    free(inputData);
    hs_free_database(database);
    return 0;
}
