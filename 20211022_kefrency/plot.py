import pandas as pd
import matplotlib.pyplot as plt

data_w = pd.read_csv('testing4.csv',encoding = 'UTF8')
data_f = pd.read_csv('f.csv',encoding = 'UTF8')
data_fdb = pd.read_csv('fdb.csv',encoding = 'UTF8')
data_k = pd.read_csv('k.csv',encoding = 'UTF8')
data_invk = pd.read_csv('invk.csv',encoding = 'UTF8')
data_ftow2 = pd.read_csv('spectowave2.csv',encoding = 'UTF8')
data_ftow = pd.read_csv('spectowave.csv',encoding = 'UTF8')

fig_w = plt.figure()
plt.plot(data_w.iloc[:,0], data_w.iloc[:,1])

fig_f = plt.figure()
# print(data_f.iloc[:,0], data_f.iloc[:,1])
plt.plot(data_f.iloc[:,0], data_f.iloc[:,1])

fig_fdb = plt.figure()
plt.plot(data_fdb.iloc[:,0], data_fdb.iloc[:,1])

fig_k = plt.figure()
plt.plot(data_k.iloc[:,0], data_k.iloc[:,1])

fig_invk = plt.figure()
plt.plot(data_invk.iloc[:,0], data_invk.iloc[:,1])

fig_ftow2 = plt.figure()
plt.plot(data_ftow2.iloc[:,0], data_ftow2.iloc[:,1])

fig_ftow = plt.figure()
plt.plot(data_ftow.iloc[:,0], data_ftow.iloc[:,1])

fig_w.savefig("output/png/outw2.png")
