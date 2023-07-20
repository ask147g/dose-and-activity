# Verification with Gamma const of Co60 in air
import matplotlib.pyplot as plt
import numpy as np

# goal activity
mass = 1 # g
mu = 59.9338171 # g per mole
Na = 6.022*10**23
tau = 1925.28*3600*24 # seconds

A = mass*Na/(mu*tau)
modelA = 1*10**6
coeff = A/modelA

# CONSTS
activity = A # Bq

gamma = 84.6 # aGy m2 / (s Bq)

mKu = 2.7027*pow(10, -8)
m = 0.01

atto = pow(10, -18)
pico = pow(10, -9)

# pSv to Sv
r = [5, 10, 15, 20, 25, 50, 75, 100, 125, 150, 175, 200] # cm
geant4 = [35.20411, 8.868605, 3.860845, 2.191883, 1.42956, 0.3570066, 0.156568, 0.08921372, 0.05447549, 0.03968703, 0.02808561, 0.02090691]
error = [0.146379973205202, 0.081948871221295, 0.0656616264140124, 0.0607429754603591, 0.0427934969099019, 0.0230300439001849, 0.0147080785662544, 0.00679606867106107, 0.0105765806475597, 0.00583276695746634, 0.00622979437139693, 0.00481610095904007]

for i in range(len(geant4)):
    geant4[i] *= coeff*pico
    error[i] *= coeff*pico

# Gamma constant
points = 100
rmin = 5
rmax = 200
rr = []
dose = []
for i in range(points):
    rr.append(rmin+i*(rmax-rmin)/(points-1))
    dose.append(activity*gamma/(rr[i]*m)**2*atto)

doseG1 = []
doseG2 = []
for i in range(len(r)):
    doseG1.append(activity*gamma/(r[i]*m)**2*atto)

fig,ax=plt.subplots(1,1)
ax.set_xlabel('r, см')
ax.set_ylabel('E, Зв/с')
plt.errorbar(r, geant4, yerr=error, fmt='ko', label='GEANT4')
plt.plot(rr, dose, color='blue', label='Г = 84.6 $\dfrac{аГр\cdot м^{2}}{с\cdot Бк}$')
ax.set_yscale('log')
plt.legend(loc="upper right")
plt.grid()
plt.show()