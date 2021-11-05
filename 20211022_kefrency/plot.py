import pandas as pd
import matplotlib.pyplot as plt

data_f = pd.read_csv('f.csv',encoding = 'UTF8')
data_k = pd.read_csv('k.csv',encoding = 'UTF8')

fig_f = plt.figure()
# print(data_f.iloc[:,0], data_f.iloc[:,1])
plt.plot(data_f.iloc[:,0], data_f.iloc[:,1])

fig_k = plt.figure()
plt.plot(data_k.iloc[:,0], data_k.iloc[:,1])

fig_f.savefig("output/png/outf.png")
fig_k.savefig("output/png/outk.png")