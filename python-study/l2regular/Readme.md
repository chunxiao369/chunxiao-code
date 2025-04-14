lance@lanceT470S:~/Summary/chunxiao-code/python-study/l2regular$ python3 test.py 
OLS (无正则化):
训练MSE: 2510.30
测试MSE: 4670.53

Ridge (L2正则化):
训练MSE: 5774.97
测试MSE: 5222.29

解释:
OLS模型的训练MSE低但测试MSE高，表明存在过拟合。
Ridge模型的训练MSE略高，但测试MSE更低，显示更好的泛化能力。
因此，L2正则化有效解决了过拟合问题。

还有L1正则化的方法，之后的代码再进行补充。
