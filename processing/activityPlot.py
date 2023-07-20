import math
import matplotlib.pyplot as plt
import numpy as np

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

for i in range(detNum[0]*detNum[1]*detNum[2]):
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

    if (particle[0] == '-'):
        activity = 0
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
        activity = 0
        if (ParticleList == "All"):
            for k in range(len(particleRes)):
                activity += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
        else:
            for k in range(len(particleRes)):
                if (ParticleList in particleRes[k]):
                    activity += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
    activityTotal = np.append(activityTotal, float(activity))

# Plot
for i in range(detNum[0]):
    ActivityResult = np.reshape(activityTotal[i*detNum[1]*detNum[2]:(i+1)*detNum[1]*detNum[2]], (-1, detNum[2]))
    # plot
    if (detNum[0]*detNum[1]*detNum[2] > 1):
        fig,ax=plt.subplots(1,1)
        ax.set_ylabel('y, (см)')
        ax.set_xlabel('z, (см)')
        cp = ax.contourf(np.array(zValue[0:20], dtype=float), np.array(yValue[0:20], dtype=float), np.array(ActivityResult, dtype=float))
        fig.colorbar(cp, label='A, Бк')
        plt.savefig(f'processing/activity/plot/{i*detSize[0]+1/2*detSize[0]}_cm.png')
    else:
        print(f"{ActivityResult[0][0]} Bq")