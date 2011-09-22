#include <stdio.h>

#define func(...) myfunc((struct mystru){__VA_ARGS__})

struct mystru
{
    const char *name;
    int number;
};

void myfunc(struct mystru ms )
{
      printf("%s: %d\n", ms.name ?: "untitled", ms.number);
}

int main(int argc, char **argv)
{
    struct mystru a = {
    .name = "abcdef",
    .number = 1,
    };
    printf("%s,%d.\n" ,a.name, a.number);
    func("three", 3);
    func("hello");
    func(.name = "zero");
    func(.number = argc, .name = "argc",);
    func(.number = 42);
    return 0;
}

