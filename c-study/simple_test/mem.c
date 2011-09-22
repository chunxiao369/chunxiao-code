#include <stdio.h>

void test_mem(int num)
{
    int i = 1;
    int j = 2;
    /*
    printf("mem........%p......%x\n",&j + 1,*(&j + 1));
    printf("mem........%p......%x\n",&j + 2,*(&j + 2));
    printf("mem........%p......%x\n",&j + 3,*(&j + 3));
    */
    switch(num)
    {
        case 4:
        {   
            int k4 = 0xfff4;
            printf("k4 com.\n");
            printf("k4.........%p......%x\n",&k4,*(&k4));
            break;
        }
        case 5:
        {   
            int k5 = 0xfff5;
            printf("k5 com.\n");
            printf("k5.........%p......%x\n",&k5,*(&k5));
            break;
        }
        case 6:
        {
            int k6 = 0xfff6;
            printf("k6 com.\n");
            printf("k6.........%p......%x\n",&k6,*(&k6));
            break;
        }
        default:
        break;
    }
    
    printf("mem........%p......%x\n",&j + 1,*(&j + 1));
    printf("mem........%p......%x\n",&j + 2,*(&j + 2));
    printf("mem........%p......%x\n",&j + 3,*(&j + 3));

    printf("i..........%p......%x\n",&i,*(&i));
    printf("j..........%p......%x\n",&j,*(&j));
}

int main(int argc, char *argv[])
{
    int a = 5;

    test_mem(a);
    printf("main.......%p........\n",main);
    
    return 0;
}



