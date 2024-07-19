import numpy as np
from scipy.optimize import curve_fit

def func(x, a, b):
    return a * x + b

xdata = np.array([1, 2, 3, 4, 5])
ydata = np.array([2, 3.9, 5.9, 8.1, 10.1])
popt, pcov = curve_fit(func, xdata, ydata)
print(popt)
print(pcov)

