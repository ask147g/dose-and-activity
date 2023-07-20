import matplotlib.pyplot as plt
import numpy as np

detNum = [20, 20, 20]
detSize = [32.7, 15, 46.45] # cm
f = open('dose.csv', 'r')

hour = 3600
# goal activity
I = 0.5*10**(-6) # A
q = 1.6*10**(-19) # C
t = 1 # second
A = I*t/q

modelA = 10**7
coeff = A/modelA

doseRate = np.array([])
for line in f:
    doseRate = np.append(doseRate, float(line.rstrip())/(10**12)*coeff*hour)

yValue = np.array([])
zValue = np.array([])
for y in range(detNum[1]):
    yValue = np.append(yValue, y*detSize[1]+1/2*detSize[1])
for z in range(detNum[2]):
    zValue = np.append(zValue, z*detSize[2]+1/2*detSize[2])

for i in range(detNum[0]):
    dose = np.reshape(doseRate[i*detNum[1]*detNum[2]:(i+1)*detNum[1]*detNum[2]], (-1, detNum[2]))
    # plot
    if (detNum[0]*detNum[1]*detNum[2] > 1):
        fig,ax=plt.subplots(1,1)
        ax.set_ylabel('y, (cm)')
        ax.set_xlabel('z, (cm)')
        cp = ax.contourf(np.array(zValue[0:20], dtype=float), np.array(yValue[0:20], dtype=float), np.array(dose, dtype=float))
        fig.colorbar(cp, label='E, Зв/ч')
        plt.savefig(f'processing/dose/plot/{i*detSize[0]+1/2*detSize[0]}_cm.png')
    else:
        print(f"{dose[0][0]} Зв/ч")