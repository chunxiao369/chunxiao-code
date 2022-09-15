#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if 0
void KMP_matching(char *T, int n, char *P, int m)
{
    int next[m];
    int k = 0;
    int q, i;

    next[0] = 0;
    /* Compute prefix array next[] */
    for (q = 2; q <= m; q++) {
        while (k > 0 && P[k] != P[q - 1])
            k = next[k - 1];
        if (P[k] == P[q - 1])
            k++;
        next[q - 1] = k;
    }

    q = 0;
    for (i = 0; i < n; i++) {
        while (q > 0 && P[q] != T[i])
            q = next[q - 1];
        if (P[q] == T[i])
            q++;
        if (q == m) {// Match!
            for (k = 0; k <= i - m; k++)
                printf("%c", ' ');
            for (k = i - m + 1; k <= i; k++)
                printf("%c", T[k]);
            printf("\n");
            /* For the next match */
            q = next[q - 1];
        }
    }
}
#endif

void cal_next(char *str, int *next, int len)
{
    next[0] = -1;//next[0]初始化为-1，-1表示不存在相同的最大前缀和最大后缀
    int k = -1;//k初始化为-1
    for (int q = 1; q <= len-1; q++) {
        while (k > -1 && str[k + 1] != str[q]) {
			//如果下一个不同，那么k就变成next[k]，注意next[k]是小于k的，无论k取任何值。
            k = next[k];//往前回溯
        }
        if (str[k + 1] == str[q]) {
		//如果相同，k++
            k = k + 1;
        }
        next[q] = k;//这个是把算的k的值（就是相同的最大前缀和最大后缀长）赋给next[q]
    }
}

int KMP(char *str, int slen, char *ptr, int plen)
{
    int *next = NULL;
    int k = -1;
    int i;
    next = malloc(sizeof(int) * plen);
    cal_next(ptr, next, plen);//计算next数组
    for (i = 0; i < plen; i++) {
        printf("%3d ", next[i]);
    }
    printf("\n");
    for (i = 0; i < slen; i++) {
        while (k >-1&& ptr[k + 1] != str[i]) {
            //ptr和str不匹配，且k>-1（表示ptr和str有部分匹配）
            k = next[k];//往前回溯
		}
        if (ptr[k + 1] == str[i]) {
            k = k + 1;
        }
        if (k == plen-1) {
			//说明k移动到ptr的最末端
            //cout << "在位置" << i-plen+1<< endl;
            //k = -1;//重新初始化，寻找下一个
            //i = i - plen + 1;//i定位到该位置，外层for循环i++可以继续找下一个（这里默认存在两个匹配字符串可以部分重叠），感谢评论中同学指出错误。
            free(next);
            return i-plen+1;//返回相应的位置
        }
    }
    free(next);
    return -1;  
}

int main(void)
{
    char *txt = "abcdefghijklmnopqrstuvwxyz";
    char *ptr= "rst";
    KMP(txt, strlen(txt), ptr, strlen(ptr));
    return 0;
}
