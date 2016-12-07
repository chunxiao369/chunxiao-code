#include <stdio.h>
#define TRUE 1
#define FALSE 0
#define SIZE 100
int main()
{
    int i;                      /*i表示整数和对应的下标 */
    int j;                      /*j表示正要处理的质数j之前的已处理j之后的未处理 */
    int k;                      /*k表示正在处理的j的倍数从2开始到j*k<SIZE */
    int a[SIZE];                /*下标表示整数内容判断是否为质数 */
    int *p;                     /*控制循环 */
    for (p = a; p < a + SIZE; ++p) {    /*初始化数组全是TRUE */
        *p = TRUE;
    }
    a[0] = a[1] = FALSE;        /*设置前面两个不是质数的数的状态为FALSE */
    i = 2;
    while (i < SIZE) {          /*找到下一个质数 */
        while (a[i++] == TRUE) {
            j = i - 1;
            break;
        }
        for (k = 2; j * k < SIZE && i < SIZE; ++k) {    /*处理质数的倍数 */
            a[j * k] = FALSE;
        }
    }
    for (p = a; p < a + SIZE; ++p) {    /*打印出质数 */
        if (*p == TRUE) {
            printf("%d ", (int)(p - a));
        }
    }
    printf("\n");
    return 0;
}
