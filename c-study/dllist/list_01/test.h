#include "list.h"

typedef int (*process) (char *p);

typedef struct {
	char *cmd_string;
	process cmd_function;
} string_function;
