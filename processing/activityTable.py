import math
import matplotlib
import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from mpl_toolkits.axes_grid1 import make_axes_locatable

# Setup
notProcessList = ["RadioactiveDecay"] # without processes
ParticleList = "All"
cooltime = 100 # in seconds

detNum = [20, 20, 20]
detSize = [1, 1, 1] # cm

# Read file
f = open('activity.csv', 'r')

activityTotal = np.array([])

yValue = np.array([])
zValue = np.array([])
for y in range(detNum[1]):
    yValue = np.append(yValue, y*detSize[1]+1/2*detSize[1])
for z in range(detNum[2]):
    zValue = np.append(zValue, z*detSize[2]+1/2*detSize[2])

elements = detNum[0]*detNum[1]*detNum[2]
for j in range(elements):
    particle = []
    process = []
    nuclides = []
    lifetime = []
    for line in f:
        if (line.rstrip() == "#END"): break
        part, proc, nucl, life = line.rstrip().rsplit(" ")
        if (nucl == 0): continue
        particle.append(part)
        process.append(proc)
        nuclides.append(nucl)
        lifetime.append(life)
    activity = 0
    if (particle[0] == '-'):
        activityTotal = np.append(activityTotal, float(0))
    else:
        ## TODO Optimize delete same particles from list below ##
        particleRes = []
        nuclidesRes = []
        lifetimeRes = []
        for i in range(len(particle)):
            if (process[i] in notProcessList): continue # not any process from List
            if (particle[i] in particleRes): # if new particle is already in list and counted
                nuclidesRes[particleRes.count(particle[i])] += int(nuclides[i])
            else: # if first particle entry
                particleRes.append(particle[i])
                nuclidesRes.append(int(nuclides[i]))
                lifetimeRes.append(float(lifetime[i]))

        # Activity
        if (ParticleList == "All"):
            for k in range(len(particleRes)):
                activity += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
        else:
            for k in range(len(particleRes)):
                if (ParticleList in particleRes[k]):
                    activity += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
    activityTotal = np.append(activityTotal, float(activity))

k = 0
for k in range(detNum[0]):
    ActivityResult = np.reshape(activityTotal[k*detNum[1]*detNum[2]:(k+1)*detNum[1]*detNum[2]], (-1, detNum[2]))
    # plot
    if (detNum[0]*detNum[1]*detNum[2] > 1):
        # plot
        fig = plt.figure(figsize=((60/2.54), (60/2.54)))
        ax = fig.add_subplot(111)
        im = ax.imshow(ActivityResult)

        ax.set_xticks(np.arange(len(zValue)), labels=zValue)
        ax.set_yticks(np.arange(len(yValue)), labels=yValue)
        ax.set_ylabel('y, (cm)', fontsize=24)
        ax.set_xlabel('z, (cm)', fontsize=24)
        ax.tick_params(axis='both', which='major', labelsize=24)
        for i in range(len(zValue)):
            for j in range(len(yValue)):
                text = ax.text(j, i, round(ActivityResult[i, j], 4),
                               ha="center", va="center", color="w")

        divider = make_axes_locatable(ax)
        cax = divider.append_axes("right", size="5%", pad=0.5)

        cbar = ax.figure.colorbar(im, cax=cax)
        cbar.ax.set_ylabel("A, Bq", rotation=90, va="bottom", fontsize=24)
        cbar.ax.tick_params(labelsize=24) 
        fig.tight_layout()
        plt.savefig(f'processing/activity/table/{k*detSize[0]+1/2*detSize[0]}_cm.png')
    else:
        print(f"{ActivityResult[0][0]} Bq")