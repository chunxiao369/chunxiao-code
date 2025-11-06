'''
定义一个周期为 T=2π 的方波 
f(t)，其表达式为：
f(t)={ 1 当 0<t<π 或−1当 π<t<2π

且满足周期性：
f(t+2π)=f(t)
（即每过 2π  重复一次）。
这个方波在时域上的图像是 “跳变的矩形波”，看起来与平滑的正弦波完全不同，但它可以被拆解为正弦波的叠加。
'''
import numpy as np
import matplotlib.pyplot as plt  # 用于绘图查看效果

# 生成x轴数据（0到2π，1000个点，覆盖一个周期）
x = np.linspace(0, 8*np.pi, 1000, endpoint=False)

# 计算傅里叶级数前4项的叠加（包含4/π系数）
y = (4 / np.pi) * (np.sin(x) + (1/3)*np.sin(3*x) + (1/5)*np.sin(5*x) + (1/7)*np.sin(7*x))

# 绘图对比（可选）
plt.plot(x, y)
plt.title('fourier transform')
plt.xlabel('t')
plt.ylabel('f(t)')
plt.grid(True)
plt.show()
