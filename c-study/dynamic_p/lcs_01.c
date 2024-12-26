#include <stdio.h>
#include <string.h>

/* 
 * 给定两个序列，找出它们的最长公共子序列。
 * 子序列的定义是：一个序列的子序列是从原序列中删除若干个字符（可以是0个）后得到的序列。
 * 并且子序列中的字符保持原来的顺序。
 */

int max(int a, int b) {
    return a > b ? a : b;
}

int lcs(char *X, char *Y, int m, int n) {
    int L[m+1][n+1];
    int i, j;

    /* Following steps build L[m+1][n+1] in bottom up manner */
    for (i=0; i<=m; i++) {
        for (j=0; j<=n; j++) {
            if (i == 0 || j == 0)
                L[i][j] = 0;
            else if (X[i-1] == Y[j-1])
                L[i][j] = L[i-1][j-1] + 1;
            else
                L[i][j] = max(L[i-1][j], L[i][j-1]);
        }
    }

    /* L[m][n] contains the length of LCS */
    return L[m][n];
}

int main() {
    char X[] = "AGGTAB";
    char Y[] = "GXTXAYB";
    int m = strlen(X);
    int n = strlen(Y);

    printf("Length of LCS is %d\n", lcs(X, Y, m, n));
    return 0;
}
