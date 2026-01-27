import numpy as np
from gplearn.genetic import SymbolicRegressor
from sklearn.utils.random import check_random_state

# ======================
# 1. 生成训练数据
# ======================
rng = check_random_state(0)
X = np.linspace(-5, 5, 32).reshape(-1, 1)
y = X[:, 0]**2 + X[:, 0] + 1   # 真实函数（模型并不知道）

print(X)
print(y)
# ======================
# 2. 创建符号回归模型
# ======================
est_gp = SymbolicRegressor(
    population_size=2000,      # 种群规模
    generations=20,            # 进化代数
    tournament_size=20,        # 锦标赛选择规模
    stopping_criteria=0.01,    # 误差小于该值则停止
    const_range=(-5.0, 5.0),   # 常数范围
    init_depth=(2, 6),         # 初始树深度
    function_set=('add', 'sub', 'mul', 'div'),  # 可用运算符
    metric='mse',              # 适应度评价指标
    parsimony_coefficient=0.01,# 复杂度惩罚（防止公式过长）
    p_crossover=0.7,           # 交叉概率
    p_subtree_mutation=0.1,    # 子树变异
    p_hoist_mutation=0.05,     # 提升变异
    p_point_mutation=0.1,      # 点变异
    max_samples=0.9,
    verbose=1,
    random_state=0
)

# ======================
# 3. 训练模型
# ======================
est_gp.fit(X, y)

# ======================
# 4. 输出发现的数学表达式
# ======================
print("发现的表达式：")
print(est_gp._program)

# ======================
# 5. 预测测试点
# ======================
X_test = np.array([[-2], [0], [3]])
y_pred = est_gp.predict(X_test)

print("\n预测结果：")
for x_val, pred in zip(X_test[:, 0], y_pred):
    print(f"x = {x_val:.1f}, 预测 y = {pred:.3f}")

