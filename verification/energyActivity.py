import math
import matplotlib.pyplot as plt
import numpy as np


# Energy
energy = [0.1, 0.5, 1] # MeV

# geant4
g401 = [30889, 31011, 30747, 30729, 30972]
g405 = [30997, 30700, 30954, 30964, 31107]
g41 = [17958, 18362, 18376, 18226, 18264]

# consts
AvogadroNumber = 6.02214082e23

# target
density = 10.22 # g/cm3
mu = 98 # g/mol
x, y, z = [1, 1, 1] # cm
area = z*y

# reaction Mo98(N,G)Mo99
# ENDF/B-VIII.0
sigmaNG = [0.0480532, 0.04849691, 0.0280214]
sigmaToCm = 1e-24

# source
Bq = 10000000

# product
lifetime = 342660
N = density/mu*AvogadroNumber # cm-3
V = x*y*z

fluence = Bq/area

# Geant4
mean = [0., 0., 0.]
mean[0] = sum(g401)/len(g401)
mean[1] = sum(g405)/len(g405)
mean[2] = sum(g41)/len(g41)
sko = [0., 0., 0.]
for i in range(len(g41)):
    sko[0] += (mean[0] - g401[i])**2
    sko[1] += (mean[1] - g405[i])**2
    sko[2] += (mean[2] - g41[i])**2
sko[0] = math.sqrt(sko[0]/len(g401))
sko[1] = math.sqrt(sko[1]/len(g405))
sko[2] = math.sqrt(sko[2]/len(g41))

# Calculate
nuclei = []
for i in range(len(energy)):
    nuclei.append(sigmaNG[i]*sigmaToCm*N*V*fluence)
    print(nuclei[i])

activity = []
for i in range(len(nuclei)):
    activity.append(1/lifetime*nuclei[i])
    mean[i] *= 1/lifetime
    sko[i] *= 1/lifetime
    print(f"E={energy[i]}: {mean[i]} +- {sko[i]} {activity[i]} {(math.fabs(mean[i]-activity[i])/activity[i]*100)} Bq")

# Plot
fig,ax=plt.subplots(1,1)
ax.set_xlabel('E, МэВ')
ax.set_ylabel('A, Бк')
plt.errorbar(energy, mean, yerr=sko, fmt='ko', label='GEANT4')
plt.plot(energy, activity, color='blue', label='ENDF')
plt.legend(loc="upper right")
plt.grid()
plt.show()