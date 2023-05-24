import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier

df = pd.read_csv("possum.csv")
df.sample(5, random_state=44)
df = df.dropna()
df.info()

X = df.drop(["case", "site", "Pop", "sex"], axis=1)
y = df["sex"]
#print(y)

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=44)

rf_model = RandomForestClassifier(n_estimators=50, random_state=44)
rf_model.fit(X_train, y_train)

imports = rf_model.feature_importances_
columns = X.columns
i=0
while i < len(columns):
    print(f"importance of fea '{columns[i]}' is {round(imports[i] * 100, 2)}%.")
    i+=1

predictions = rf_model.predict(X_test)
print(predictions)
print(y_test)
new_possum = [[7, 83.2, 54.3, 81.0, 37.0, 70.0, 46.3, 14.7, 25.0, 32.0]]
predictions = rf_model.predict(new_possum)
print(predictions)

