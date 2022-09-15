/*
 pcre example
 */

#include <stdio.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pcre.h>

#define REGEX_SEARCH_OVECCOUNT 64
int main(int argc, char *argv[])
{
    pcre *p_pcre = NULL;
    int offset = -1;
    int rc = 0;
    int i = 0;
    char *pattern;
    char *p;
    const char *p_errmsg = NULL;
    int ovector[REGEX_SEARCH_OVECCOUNT] = {0};

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pattern> <data> \n", argv[0]);
        return -1;
    }
    pattern = argv[1];
    if (NULL == (p_pcre = pcre_compile(pattern, PCRE_DOTALL | PCRE_MULTILINE, &p_errmsg, &offset, NULL))) {
        printf("ErrMsg=%s, Offset=%d\n", p_errmsg, offset);
        return -1;
    }
    p = argv[2];
    rc = pcre_exec(p_pcre, NULL, p, strlen(p), 0, 0, ovector, REGEX_SEARCH_OVECCOUNT);
    if (rc <= 0) {
        printf("%s doesn't match %s\n", p, pattern);
    } else {
        printf("%s matches %s\n", p, pattern);
        for (i = 0; i < rc; i++) {
            printf("rc i: %d, start:%2d, end: %2d\n", i, ovector[2*i], ovector[2*i + 1]);
        }
    }
    pcre_free(p_pcre);
     
    return 0;
}

/*
./xuexe "a(bbb)(ea)" "eabbbeabbbbf"
./xuexe "a(bbb)(ea)" "dabbea"
./xuexe "name=(.{1,8}).*xxx" "name=embsxxx"
./xuexe "name=(.{1,8}?).*xxx" "name=embsxxx"
*/
