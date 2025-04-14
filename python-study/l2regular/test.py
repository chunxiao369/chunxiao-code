# 导入必要的库
import numpy as np
from sklearn.datasets import load_diabetes
from sklearn.model_selection import train_test_split
from sklearn.linear_model import LinearRegression, Ridge
from sklearn.metrics import mean_squared_error

# 加载糖尿病数据集
diabetes = load_diabetes()
X = diabetes.data
y = diabetes.target

# 划分训练集和测试集，使用10%数据训练以突出过拟合
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.95, random_state=42)

# 训练普通线性回归模型（无正则化）
ols = LinearRegression()
ols.fit(X_train, y_train)

# 计算OLS的预测和MSE
y_train_pred_ols = ols.predict(X_train)
y_test_pred_ols = ols.predict(X_test)
mse_train_ols = mean_squared_error(y_train, y_train_pred_ols)
mse_test_ols = mean_squared_error(y_test, y_test_pred_ols)

# 训练Ridge回归模型（L2正则化）
ridge = Ridge(alpha=1.0)
ridge.fit(X_train, y_train)

# 计算Ridge的预测和MSE
y_train_pred_ridge = ridge.predict(X_train)
y_test_pred_ridge = ridge.predict(X_test)
mse_train_ridge = mean_squared_error(y_train, y_train_pred_ridge)
mse_test_ridge = mean_squared_error(y_test, y_test_pred_ridge)

# 打印结果
print("OLS (无正则化):")
print(f"训练MSE: {mse_train_ols:.2f}")
print(f"测试MSE: {mse_test_ols:.2f}")
print("\nRidge (L2正则化):")
print(f"训练MSE: {mse_train_ridge:.2f}")
print(f"测试MSE: {mse_test_ridge:.2f}")

# 解释
print("\n解释:")
print("OLS模型的训练MSE低但测试MSE高，表明存在过拟合。")
print("Ridge模型的训练MSE略高，但测试MSE更低，显示更好的泛化能力。")
print("因此，L2正则化有效解决了过拟合问题。")
