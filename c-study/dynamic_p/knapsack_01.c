#include <stdio.h>

#define MAX_WEIGHT 50

int knapsack(int W, int wt[], int val[], int n) 
{
    int i, j;
    int a, b;
    int cnt1 = 0, cnt2 = 0;
    // dp[i][j]存储了在不超过背包承重限制j的情况下，背包中物品的最大价值。
    int dp[n+1][W+1];

    // 初始化
    for (i = 0; i <= n; i++) {
        for (j = 0; j <= W; j++) {
            dp[i][j] = 0;
        }
    }

    // 动态规划
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= W; j++) {
            if (wt[i-1] <= j) {
                // a 理解为选当前i物品时的最大价值
                // 所以就先加上当前物品的价值 val[i-1]
                // 这时背包的剩余容量变为j-wt[i-1]，再拿到剩余容量下的最大价值，
                // 此时的最大价值为前i-1个物品在容量为j-wt[i-1]的背包中所能获得的最大价值
                a = val[i-1] + dp[i-1][j-wt[i-1]];
                // b 理解为不选当前i物品，只考虑前i-1个物品且承重为j时的最大价值
                b = dp[i-1][j];
                if (a > b) {
                    cnt1++;
                    dp[i][j] = a;
                } else {
                    cnt2++;
                    dp[i][j] = b;
                }
            } else {
                dp[i][j] = dp[i-1][j];
            }
        }
    }
#if 0
    for (j = 1; j <= W; j++) {
        for (i = 1; i <= n; i++) {
            printf("j: %2d, i: %2d, dp: %3d.\n", j, i, dp[i][j]);
        }
    }
    printf("cnt1: %d, cnt2: %d.\n", cnt1, cnt2);
#endif
    return dp[n][W];
}

int main()
{
    int val[] = {100, 60, 120, 3};
    int wt[] = {20, 10, 25, 3};
    int W = 50;
    int n = sizeof(val)/sizeof(val[0]);

    printf("Knapsack problem solution : %d\n", knapsack(W, wt, val, n));
    return 0;
}
