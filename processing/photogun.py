import matplotlib.pyplot as plt
import numpy as np

plane = 'xz'

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

xValue = np.array([])
yValue = np.array([])
zValue = np.array([])
for x in range(detNum[0]):
    xValue = np.append(xValue, x*detSize[0]+1/2*detSize[0])
for y in range(detNum[1]):
    yValue = np.append(yValue, y*detSize[1]+1/2*detSize[1])
for z in range(detNum[2]):
    zValue = np.append(zValue, z*detSize[2]+1/2*detSize[2])

if (plane == 'yz'):
    for i in range(detNum[0]):
        dose = np.reshape(doseRate[i*detNum[1]*detNum[2]:(i+1)*detNum[1]*detNum[2]], (-1, detNum[2]))
        # plot
        if (detNum[0]*detNum[1]*detNum[2] > 1):
            fig,ax=plt.subplots(1,1)
            ax.set_ylabel('y, (cm)')
            ax.set_xlabel('z, (cm)')
            cp = ax.contourf(np.array(zValue, dtype=float), np.array(yValue, dtype=float), np.array(dose, dtype=float))
            fig.colorbar(cp, label='E, Зв/ч')
            plt.savefig(f'processing/dose/plot/{i*detSize[0]+1/2*detSize[0]}_cm.png')
        else:
            print(f"{dose[0][0]} Зв/ч")


elif (plane == 'xz'):
    for y in range(detNum[1]): # y
        ddose = []
        for z in range(detNum[2]): # z
            for x in range(detNum[0]):
                ddose.append(doseRate[400*x+20*z+y])
        dose = np.reshape(ddose, (-1, detNum[1]))

        # plot
        if (detNum[0]*detNum[1]*detNum[2] > 1):
            fig,ax=plt.subplots(1,1)
            ax.set_ylabel('z, (cm)')
            ax.set_xlabel('x, (cm)')
            cp = ax.contourf(np.array(xValue, dtype=float), np.array(zValue, dtype=float), np.array(dose, dtype=float), cmap=plt.cm.jet)
            fig.colorbar(cp, label='E, Зв/ч')
            plt.savefig(f'processing/dose/plot/{y*detSize[1]+1/2*detSize[1]}_cm.png')
        else:
            print(f"{dose[0][0]} Зв/ч")

else:
    for z in range(detNum[2]): # y
        ddose = []
        for y in range(detNum[1]): # z
            for x in range(detNum[0]):
                ddose.append(doseRate[400*x+20*z+y])
        dose = np.reshape(ddose, (-1, detNum[2]))

        # plot
        if (detNum[0]*detNum[1]*detNum[2] > 1):
            fig,ax=plt.subplots(1,1)
            ax.set_ylabel('y, (cm)')
            ax.set_xlabel('x, (cm)')
            cp = ax.contourf(np.array(xValue, dtype=float), np.array(yValue, dtype=float), np.array(dose, dtype=float))
            fig.colorbar(cp, label='E, Зв/ч')
            plt.savefig(f'processing/dose/plot/{z*detSize[2]+1/2*detSize[2]}_cm.png')
        else:
            print(f"{dose[0][0]} Зв/ч")