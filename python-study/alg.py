import pandas as pd
import numpy as np
data = pd.DataFrame(np.random.randint(low=1,high=10,size=(4,1)))
#data = pd.DataFrame(np.random.randint(low=1,high=10,size=(4,2)))
print(data)
print("mean is: ", data.mean())
print("sum is: ", data.sum())
print("Variance is: ", data.var())
print("standard deviation: ", data.std())

print(data.diff())
print(data.diff().dropna())
#df = [1,2,3,4,5]
#print(df)
