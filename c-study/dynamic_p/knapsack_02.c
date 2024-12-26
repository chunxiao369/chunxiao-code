#include <stdio.h>

// 物品结构体
typedef struct {
    int weight; // 物品重量
    int value;  // 物品价值
} Item;

// 动态规划解决0/1背包问题
int knapsack(Item items[], int n, int W) {
    int i, w;
    int dp[n+1][W+1];

    // 初始化dp数组
    // dp[i][j]存储了在不超过背包承重限制j的情况下，背包中物品的最大价值。
    for (i = 0; i <= n; i++) {
        for (w = 0; w <= W; w++) {
            dp[i][w] = 0;
        }
    }

    // 构建dp数组
    for (i = 1; i <= n; i++) {
        for (w = 1; w <= W; w++) {
            if (items[i-1].weight <= w) {
                // 选择当前物品
                int value_if_picked = items[i-1].value + dp[i-1][w-items[i-1].weight];
                // 不选择当前物品
                int value_if_not_picked = dp[i-1][w];
                // 取两者的最大值
                dp[i][w] = (value_if_picked > value_if_not_picked) ? value_if_picked : value_if_not_picked;
            } else {
                // 如果当前物品重量超过背包容量，则不选择
                dp[i][w] = dp[i-1][w];
            }
        }
    }

    // 返回最大价值
    return dp[n][W];
}

int main() {
    // 物品数组
    Item items[] = {{10, 60}, {20, 100}, {25, 120}};
    int n = sizeof(items) / sizeof(items[0]); // 物品数量
    int W = 50; // 背包最大承重

    // 计算最大价值
    int max_value = knapsack(items, n, W);
    printf("The maximum value of items that can be carried: %d\n", max_value);

    return 0;
}
