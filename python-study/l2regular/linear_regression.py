import numpy as np
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt

# ======================
# 1. 构造示例数据
# ======================
# 自变量 X（必须是二维）
X = np.array([
    [1],
    [2],
    [3],
    [4],
    [5],
    [6]
])

# 因变量 y
y = np.array([2, 4, 5, 4, 5, 7])

# ======================
# 2. 创建并训练模型
# ======================
model = LinearRegression()
model.fit(X, y)

# ======================
# 3. 输出模型参数
# ======================
print("回归系数 (w):", model.coef_[0])
print("截距 (b):", model.intercept_)

# 线性回归方程
print(f"回归方程: y = {model.coef_[0]:.3f} * x + {model.intercept_:.3f}")

# ======================
# 4. 进行预测
# ======================
X_test = np.array([[7], [8]])
y_pred = model.predict(X_test)

print("预测结果:")
for x_val, pred in zip(X_test, y_pred):
    print(f"x = {x_val[0]}, 预测 y = {pred:.3f}")

# ======================
# 5. 可视化
# ======================
plt.scatter(X, y, label="原始数据")
plt.plot(X, model.predict(X), label="回归直线")
plt.scatter(X_test, y_pred, marker='x', s=100, label="预测点")
plt.xlabel("X")
plt.ylabel("y")
plt.legend()
plt.title("Linear Regression Example")
plt.show()

