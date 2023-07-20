import numpy as np
import matplotlib
import matplotlib as mpl
import matplotlib.pyplot as plt
from mpl_toolkits.axes_grid1 import make_axes_locatable

detNum = [20, 20, 20]
detSize = [10, 10, 10] # cm
f = open('dose.csv', 'r')

doseRate = np.array([])
for line in f:
    doseRate = np.append(doseRate, float(line.rstrip())/(10**9)*3600)

yValue = np.array([])
zValue = np.array([])
for y in range(detNum[1]):
    yValue = np.append(yValue, y*detSize[1]+1/2*detSize[1])
for z in range(detNum[2]):
    zValue = np.append(zValue, z*detSize[2]+1/2*detSize[2])


k = 0
for k in range(detNum[0]):
    dose = np.reshape(doseRate[k*detNum[1]*detNum[2]:(k+1)*detNum[1]*detNum[2]], (-1, detNum[2]))
    # plot
    fig = plt.figure(figsize=((60/2.54), (60/2.54)))
    ax = fig.add_subplot(111)
    im = ax.imshow(dose)

    ax.set_xticks(np.arange(len(zValue)), labels=zValue)
    ax.set_yticks(np.arange(len(yValue)), labels=yValue)
    ax.set_ylabel('y, (cm)', fontsize=24)
    ax.set_xlabel('z, (cm)', fontsize=24)
    ax.tick_params(axis='both', which='major', labelsize=24)
    for i in range(len(zValue)):
        for j in range(len(yValue)):
            text = ax.text(j, i, round(dose[i, j], 4),
                           ha="center", va="center", color="w")

    divider = make_axes_locatable(ax)
    cax = divider.append_axes("right", size="5%", pad=0.5)

    cbar = ax.figure.colorbar(im, cax=cax)
    cbar.ax.set_ylabel("H, mSv/h", rotation=90, va="bottom", fontsize=24)
    cbar.ax.tick_params(labelsize=24) 
    fig.tight_layout()
    plt.savefig(f'processing/dose/table/{k*detSize[0]+1/2*detSize[0]}_cm.png')