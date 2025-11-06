import matplotlib.pyplot as plt
import numpy as np

# 定义x的范围
x = np.linspace(-10, 10, 100)  # 从-10到10生成400个点

# 定义函数y = e^x和y = sin(x)
y1 = np.exp(x)  # 使用numpy的exp函数计算e^x
y2 = np.sin(x)  # 使用numpy的sin函数计算sin(x)
#y3 = (np.sin(x) + (1/3) * np.sin(3*x) + (1/5) * np.sin(5*x) + (1/7) * np.sin(7 * x)) * 4 / np.pi  # 方波傅里叶 fourier_transform

# 绘制曲线
plt.plot(x, y1, label='y = e^x')  # 给第一个曲线添加标签
plt.plot(x, y2, label='y = sin(x)')  # 给第二个曲线添加标签
#plt.plot(x, y3, label='y = fourier example')  # 给第二个曲线添加标签

# 添加图例
plt.legend()

# 添加标题和标签
plt.title('Plot of some funcs')
plt.xlabel('x')
plt.ylabel('y')

# 显示图形
plt.show()

