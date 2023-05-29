import numpy as np
import matplotlib.pyplot as plt

scan = np.loadtxt("../Output_files/scan.txt")

# \/

xName = "x"
xCentre = 10.5152
xRange = 0.3

yName = "y"
yCentre = 13.5011
yRange = 0.3

# /\

nY, nP = np.shape(scan)
print(scan)

vX = np.linspace(xCentre - xRange*0.5, xCentre + xRange*0.5, nP)
vY = np.linspace(yCentre - yRange*0.5, yCentre + yRange*0.5, nY)

minis = [np.argmin(r) for r in scan]
minr = np.argmin([min(r) for r in scan])
print("Min = " + str(min([min(r) for r in scan])) + " at " + str(vX[minis[minr]]) + ", " + str(vY[minr]))

mX, mY = np.meshgrid(vX, vY)

scan[scan > 1.0] = np.nan;

fig = plt.figure()
ax = plt.axes(projection='3d')
ax.plot_wireframe(mX, mY, scan, color='green')
ax.set_xlabel(xName)
ax.set_ylabel(yName)
ax.set_zlabel("$1 -\eta$")
plt.show()
