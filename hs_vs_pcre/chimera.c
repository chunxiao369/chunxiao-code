#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <hs/hs.h>
#include <hs/ch.h>
#include <hs/hs_common.h>
#include <hs/ch_common.h>

#define MAX_HIT_NUM 32
#define MAX_GROUPS_NUM 32

struct groups_match {
    uint32_t from[MAX_GROUPS_NUM];
    uint32_t to[MAX_GROUPS_NUM];
    uint32_t count;
};

struct scan_ctx {
    uint32_t  max_num; //最大命中个数，需要预设一个大于0的值
    uint32_t  count;   //命中个数
    uint32_t id[MAX_HIT_NUM] ;     //命中规则id
    uint32_t from[MAX_HIT_NUM];    //命令起始位置
    uint32_t to[MAX_HIT_NUM];      //命中结束位置
    struct groups_match gp_match[MAX_HIT_NUM]; //返回group的各个匹配位置
};

/**
 * This is the function that will be called for each match that occurs. @a ctx
 * is to allow you to have some application-specific state that you will get
 * access to for each match. In our simple example we're just going to use it
 * to pass in the pattern that was being searched for so we can print it out.
 */
static int eventHandler(unsigned int id, unsigned long long from,
                        unsigned long long to, unsigned int flags, void *ctx) {
    printf("Match at id:%u offset from:%llu to:%llu\n",id, from, to);
    return 0;
}

static int errorHandler(ch_error_event_t error_type,
                        unsigned int id, void *info, void *ctx) {
    printf("error id:%u msg:%s\n",id, info);
    return 0;
}

static int __scan_on_event(unsigned int id, unsigned long long from, unsigned long long to, 
        unsigned int flags, unsigned int size, const ch_capture_t *captured, void *ctx)
{
    struct scan_ctx *c = (struct scan_ctx *)ctx;

    if (c->count < MAX_HIT_NUM) {
        if (c->id)
            c->id[c->count] = id;
        if (c->from)
            c->from[c->count] = from;
        if (c->to)
            c->to[c->count] = to;
    }

#if 1
    int i = 0;
    if (size > 0) {
        c->gp_match[c->count].count = size;
        //printf("id:%d\n", id);
        for (i = 0; i < size; i ++) {
            c->gp_match[c->count].from[i] = captured->from;
            c->gp_match[c->count].to[i] = captured->to;
            //printf("sub expression from:%d to:%d\n", captured->from, captured->to);
            captured++;
        }
    }
#endif
    c->count++;
    //printf("Match for pattern from %llu to offset %llu\n", from, to);
    //printf("match rule id %d\n", id);
    return 0;
}


int main(int argc, char *argv[]) {
    char *patterns[] = {"he(\\w+)o","b(\\w+)du"};
    const unsigned int ids[] = {1, 4};
    int pattern_num = 2;
    const unsigned int flags[] = {CH_FLAG_DOTALL, CH_FLAG_DOTALL};
    char buffer[64] = "hello,world,baidu.com";

    /* First, we attempt to compile the pattern provided on the pattern
     * We assume 'DOTALL' semantics, meaning that the '.' meta-character will
     * match newline characters. The compiler will analyse the given pattern and
     * either return a compiled Hyperscan database, or an error message
     * explaining why the pattern didn't compile.
     */
    ch_database_t *database;
    ch_compile_error_t *compile_err;
    if (ch_compile_multi((const char* const *)patterns, flags, ids, pattern_num, CH_MODE_GROUPS, NULL, &database,
                   &compile_err) != CH_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to compile pattern\n");
        ch_free_compile_error(compile_err);
        return -1;
    }

    /**
     * scratch space needs to be allocated with the ch_alloc_scratch
     * function. In typical usage, you would reuse this scratch space for many
     * calls to ch_scan, but as we're only doing one, we'll be allocating it
     * and deallocating it as soon as our matching is done.
    */
    ch_scratch_t *scratch = NULL;
    if (ch_alloc_scratch(database, &scratch) != CH_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to allocate scratch space. Exiting.\n");
        ch_free_database(database);
        return -1;
    }

    struct scan_ctx ctx;
    if (ch_scan(database, buffer, strlen(buffer), 0, scratch, __scan_on_event, errorHandler, 
                (void *)&ctx) != CH_SUCCESS) {
        fprintf(stderr, "ERROR: Unable to scan input buffer. Exiting.\n");
        ch_free_scratch(scratch);
        ch_free_database(database);
        return -1;
    }
    printf("Scan is complete. \n");

    printf("The result is as below. \n");
    //result
    int i;
    int j;
	int len;
	char buf[64] = {0x0};
    printf("count num:%d\n", ctx.count);
    for (i = 0; i < ctx.count; i++) {
        printf("id: %d match\n", ctx.id[i]);
        for (j = 0; j < ctx.gp_match[i].count; j++) {
            printf("sub match from:%d to:%d\n",ctx.gp_match[i].from[j], ctx.gp_match[i].to[j]);
        }
    }

    /* Scanning is complete, any matches have been handled, so now we just
     * clean up and exit.
     */
    ch_free_scratch(scratch);
    ch_free_database(database);
    
    return 0;

}