import numpy as np
from matplotlib import pyplot as plt

da = [0.1,0.2]
di = 0.1
n = 161.144
rho = 3000
h = 0.07
t0 = 4

d = []
M = []

i = da[0]
while i<= da[1]:
    M.append((n*i*np.square(np.pi)*h*rho)/(32*t0)*(np.power(i,4)-np.power(di,4)))
    d.append(i)
    i = i + 0.001

fig, ax = plt.subplots()

ax.plot( d,M)

ax.set(ylabel='M in Nm ', xlabel='d in m',
       )


ax.grid()


plt.show()
fig.savefig('M_m.png')