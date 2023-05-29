import numpy as np
import matplotlib.pyplot as plt
import sys

inputDirectory = "../Output_files/Comp rects/"

outputDirectory = "../Report/Figures/Comp images/"

analysisName = sys.argv[2]
datasetName = sys.argv[1]

width = int(sys.argv[3])

xray1d = np.fromfile(inputDirectory + "xray_manual.dat", dtype=np.uint16)
xray2d = np.array(np.reshape(xray1d, (-1, width)), dtype=np.float32)
drr1d = np.fromfile(inputDirectory + "drr_manual.dat", dtype=np.uint16)
drr2d = np.array(np.reshape(drr1d, (-1, width)), dtype=np.float32)

fig, axs = plt.subplots(2, 2)

axs[0, 0].imshow(xray2d, norm="linear", cmap="gray")
axs[0, 0].xaxis.set_visible(False)
axs[0, 0].yaxis.set_visible(False)

xray_hist_ys, _, _ = axs[1, 0].hist(xray1d[np.logical_not(xray1d == 0)], bins=500, log=False)
axs[1, 0].set_xlabel("intensity")
axs[1, 0].set_ylabel("frequency")

axs[0, 1].imshow(drr2d, norm="linear", cmap="gray")
axs[0, 1].xaxis.set_visible(False)
axs[0, 1].yaxis.set_visible(False)

drr_hist_ys, binStarts, _ = axs[1, 1].hist(drr1d[np.logical_not(drr1d == 0)], bins=500, log=False)
axs[1, 1].set_xlabel("intensity")
axs[1, 1].label_outer()

histLims = [0, max(np.max(xray_hist_ys), np.max(drr_hist_ys))]
axs[1, 0].set_ylim(histLims)
axs[1, 1].set_ylim(histLims)

plt.subplots_adjust(right=0.99, top=0.99, bottom=0.15, left=0.1, wspace=0.02, hspace=-0.1)

fig.set_size_inches(7.28, 3.0) # an A4 page minus 25mm margins all around is 9.73 x 7.28 inches

plt.savefig(outputDirectory + analysisName + "/" + datasetName + ".png", format="png")
plt.clf()

'''
xr1 = Plots(fn_xray)[100]
dr1 = Plots(fn_drr)[100]
#xr2 = Plots(fn_xray2)[100]
#dr2 = Plots(fn_drr2)[100]

plt.show()

#xr1[xr1 == 0] = np.nan
#dr1[dr1 == 0] = np.nan
#xr2[xr2 == 0] = np.nan
#dr2[dr2 == 0] = np.nan

N = len(xr1)
plt.figure()
plt.scatter(np.linspace(0, N - 1, N), xr1, marker='+')
plt.scatter(np.linspace(0, N - 1, N), dr1, marker='+')

plt.figure()
plt.scatter(np.linspace(0, N - 1, N), xr1*4.62386 + 10202.2, marker='+')
plt.scatter(np.linspace(0, N - 1, N), dr1, marker='+')

#N = len(xr2)
#plt.figure()
#plt.scatter(np.linspace(0, N - 1, N), xr2, marker='+')
#plt.scatter(np.linspace(0, N - 1, N), dr2, marker='+')

plt.show()
'''
