#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static char **my_completion(const char *, int, int);
char *my_generator(const char *, int);
char *your_generator(const char *text, int state);
char *dupstr(char *);
void *xmalloc(int);

/* show system */
/* show all status */
/* show all counter */
/* quit */
/* set int A */
/* set int B */
char *cmd[] = { "show", "quit", "set", NULL};
char *cmd2[] = { "system", "all", NULL};
char *cmd3[] = { "status", "counter", NULL};
char *cmd4[] = { "int", NULL};
char *cmd5[] = { "A", "B", NULL};

char **cmd_pp = NULL;
int main()
{
    char *buf = NULL;

    rl_attempted_completion_function = my_completion;
    rl_completion_entry_function = your_generator;
    /*
    printf("%p\n", cmd);
    printf("%p\n", cmd2);
    printf("%p\n", cmd3);
    printf("%p\n", cmd4);
    printf("%p\n", cmd5);
    */
    while ((buf = readline("\ncli>")) != NULL) {
        //while((buf = readline("\n &gt;&gt; "))!=NULL) {
        //enable auto-complete
        //rl_bind_key('\t', rl_complete);

        printf("cmd [%s]\n", buf);
        if (strncmp(buf, "quit", 4) == 0)
            break;
        if (buf[0] != 0)
            add_history(buf);
    }
    free(buf);

    return 0;
}

static char **my_completion(const char *text, int start, int end)
{
    char **matches;

    matches = (char **)NULL;
    //printf("\n cxxu text: %s, start: %d, end: %d.\n", text, start, end);
    //printf("\n rl_buf: %s.\n", rl_line_buffer);

    if (start == 0) {
        matches = rl_completion_matches((char *)text, &my_generator);
    } else {
        //rl_bind_key('\t', rl_abort);
        //rl_bind_key('\t', rl_complete);
#if 1
    if (strncmp(rl_line_buffer, "show system", 11) == 0) {
        cmd_pp = NULL;
    } else if (strncmp(rl_line_buffer, "show all status", 15) == 0) {
        cmd_pp = NULL;
    } else if (strncmp(rl_line_buffer, "show all counter", 16) == 0) {
        cmd_pp = NULL;
    } else if (strncmp(rl_line_buffer, "set int A", 9) == 0) {
        cmd_pp = NULL;
    } else if (strncmp(rl_line_buffer, "set int B", 9) == 0) {
        cmd_pp = NULL;
    } else if (strncmp(rl_line_buffer, "show all ", 9) == 0) {
        cmd_pp = cmd3;
    } else if (strncmp(rl_line_buffer, "set int ", 8) == 0) {
        cmd_pp = cmd5;
    } else if (strncmp(rl_line_buffer, "show ", 5) == 0) {
        cmd_pp = cmd2;
    } else if (strncmp(rl_line_buffer, "set ", 4) == 0) {
        cmd_pp = cmd4;
    } 
#endif

    }
    return (matches);
}

char *my_generator(const char *text, int state)
{
    static int list_index = 0, len;
    char *name = NULL;
    if (!state) {
        list_index = 0;
        len = strlen(text);
    }
    name = cmd[list_index];
    while (name) {
        list_index++;

        if (strncmp(name, text, len) == 0)
            return (dupstr(name));
        name = cmd[list_index];
    }
    /* If no names matched, then return NULL. */
    return ((char *)NULL);

}

char *your_generator(const char *text, int state)
{
    static int list_index = 0, len;
    char *name = NULL;

    if (!state) {
        list_index = 0;
        len = strlen(text);
    }
    //printf("\nbbbbbbbbb, text: %s, state: %d\n", text, state);
    if (cmd_pp == NULL) {
        return NULL;
    }
    name = cmd_pp[list_index];
    while (name) {
        list_index++;

        if (strncmp(name, text, len) == 0) {
            return (dupstr(name));
        }
        name = cmd_pp[list_index];
    }
    /* If no names matched, then return NULL. */
    return ((char *)NULL);

}

char *dupstr(char *s)
{
    char *r;

    r = (char *)xmalloc((strlen(s) + 1));
    strcpy(r, s);
    return (r);
}

void *xmalloc(int size)
{
    void *buf;

    buf = malloc(size);
    if (!buf) {
        fprintf(stderr, "Error: Out of memory. Exiting.'n");
        exit(1);
    }
    memset(buf, 0, size);

    return buf;
}
