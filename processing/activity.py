import math
# Setup
notProcessList = ["RadioactiveDecay"] # without processes
ParticleList = ["All", "Mo99"] # Plot is always in zero position
cooltime = 100 # in seconds


# Read file
f = open('activity.csv', 'r')

particle = []
process = []
nuclides = []
lifetime = []
for line in f:
    part, proc, nucl, life = line.rstrip().rsplit(" ")
    particle.append(part)
    process.append(proc)
    nuclides.append(nucl)
    lifetime.append(life)

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
activity = []
for i in range(len(ParticleList)):
    activity.append(0)
    if (ParticleList[i] == "All"):
        for k in range(len(particleRes)):
            activity[i] += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
    else:
        for k in range(len(particleRes)):
            if (ParticleList[i] in particleRes[k]):
                activity[i] += 1/lifetimeRes[k]*nuclidesRes[k]*math.exp(-cooltime/lifetimeRes[k])
    print(f"{ParticleList[i]}: {activity[i]} Bq")