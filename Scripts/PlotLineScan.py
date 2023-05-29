import numpy as np
import matplotlib.pyplot as plt
import sys
from PIL import Image

inputDirectory = "../Output_files/Line scans/"
outputDirectory = "../Report/Figures/Line scans/"

analysisName = sys.argv[2]
datasetName = sys.argv[1]
simMetricNames = ["heuristic_gradient", "correlation_coefficient", "correlation_ratio", "gradient_correlation"]
#rangeScaleNames = ["5.0"]#["1.0", "5.0", "25.0"]

inputDirectory = inputDirectory + datasetName + "/"
outputDirectory = outputDirectory + analysisName + "/" + datasetName + "/"

for simMetricName in simMetricNames:
	inputPrefix = inputDirectory + simMetricName
	outputPrefix = outputDirectory + simMetricName
	
	# screenshot:
	img = Image.open(inputPrefix + "_ss.bmp")
	# saved separately just in case:
	img.save(outputPrefix + "_ss.png")
	
	# line scan plots:
	inputFileName = inputPrefix + ".dat"
	outputFileName = outputPrefix + ".png"
	
	# loading data from file
	file = open(inputFileName, "rb")
	
	if not file.read(1):
		print("File\n'" + inputFileName + "'\nis empty.")
		file.close()
		continue
	
	file.seek(0)
	# 11 + D*4 byte header
	N = np.frombuffer(file.read(4), dtype=np.int32)[0]
	D = np.frombuffer(file.read(4), dtype=np.int32)[0]
	centre = np.frombuffer(file.read(4*D), dtype=np.float32)
	ranges = np.frombuffer(file.read(4*3), dtype=np.float32)

	scan = np.reshape(np.frombuffer(file.read(), dtype=np.float32), (-1, N))
	assert(np.shape(scan)[0] == D)

	file.close()
	
	# plots:
	fig, axs = plt.subplots(2, 2)
	
	bottomScans = [scan[0:3], scan[6:8]]
	bsBottom = np.min([np.min(s) for s in bottomScans])
	bsTops = [np.max(s) for s in bottomScans]
	bsLims = [bsBottom, min(bsBottom + 3.0*(np.min(bsTops) - bsBottom), np.max(bsTops))]
	
	vX = np.linspace(-0.5*ranges[0], 0.5*ranges[0], N)
	axs[1, 0].plot(vX, scan[2], label="ct z") # wobbles more, so putting first (behind)
	axs[1, 0].plot(vX, scan[0], label="ct x") # ` + f"{centre[0]:.3}"`
	axs[1, 0].plot(vX, scan[1], label="ct y")
	axs[1, 0].set_ylim(bsLims)
	axs[1, 0].set_xlabel("offset [mm]")
	axs[1, 0].set_ylabel(simMetricName)
	axs[1, 0].legend()
	#axs[1, 0].ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
	
	vX = np.linspace(-0.5*ranges[1], 0.5*ranges[1], N)
	axs[1, 1].plot(vX, scan[6], label="source x")
	axs[1, 1].plot(vX, scan[7], label="source y")
	axs[1, 1].set_ylim(bsLims)
	axs[1, 1].set_xlabel("offset [mm]")
	axs[1, 1].yaxis.set_visible(False)
	axs[1, 1].legend()
	#axs[1, 1].ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
	
	vX = np.linspace(-0.5*ranges[2], 0.5*ranges[2], N)
	axs[0, 0].plot(vX, scan[5], label="ct yaw") # wobbles more, so putting first (behind)
	axs[0, 0].plot(vX, scan[3], label="ct roll")
	axs[0, 0].plot(vX, scan[4], label="ct pitch")
	axs[0, 0].set_xlabel("offset [rad]")
	axs[0, 0].set_ylabel(simMetricName)
	axs[0, 0].legend()
	#axs[0, 0].ticklabel_format(axis='y', style='sci', scilimits=(0, 0))
	
	axs[0, 1].imshow(img)
	axs[0, 1].xaxis.set_visible(False)
	axs[0, 1].yaxis.set_visible(False)

	# Hide x labels and tick labels for top plots and y ticks for right plots.
	#for ax in axs.flat:
	#	ax.label_outer()
	
	plt.subplots_adjust(right=0.99, top=0.98, wspace=0.02, hspace=0.3)
	
	fig.set_size_inches(7.28, 4.85) # an A4 page minus 25mm margins all around is 9.73 x 7.28 inches
	
	#plt.show()
	plt.savefig(outputFileName, format="png")
	plt.clf()
		
	'''
	# separate plots:
	for rangeScaleName in rangeScaleNames:
		# x-y translations:
		vX = np.linspace(-0.5*ranges[0], 0.5*ranges[0], N)
		plt.plot(vX, scan[0], label="x offset varied around " + str(centre[0]))
		plt.plot(vX, scan[1], label="y offset varied around " + str(centre[1]))
		axes = plt.gca()
		axes.set_ylim([np.min(scan[0:2]), np.max(scan[0:2])])
		plt.xlabel("offset [mm]")
		plt.ylabel(simMetricName)
		plt.legend()
		#plt.show()
		plt.savefig(outputPrefixExtended + "_xy.png", format="png")
		plt.clf()

		# z and source x-y translations:
		vX = np.linspace(-0.5*ranges[1], 0.5*ranges[1], N)
		plt.plot(vX, scan[2], label="z offset varied around " + str(centre[2]))
		plt.plot(vX, scan[6], label="source x offset varied around " + str(centre[6]))
		plt.plot(vX, scan[7], label="source y offset varied around " + str(centre[7]))
		axes = plt.gca()
		axes.set_ylim([min(np.min(scan[2]), np.min(scan[6:8])), max(np.max(scan[2]), np.max(scan[6:8]))])
		plt.xlabel("offset [mm]")
		plt.ylabel(simMetricName)
		plt.legend()
		#plt.show()
		plt.savefig(outputPrefixExtended + "_zsxsy.png", format="png")
		plt.clf()

		# rotations:
		vX = np.linspace(-0.5*ranges[2], 0.5*ranges[2], N)
		plt.plot(vX, scan[3], label="roll offset varied around " + str(centre[3]))
		plt.plot(vX, scan[4], label="pitch offset varied around " + str(centre[4]))
		plt.plot(vX, scan[5], label="yaw offset varied around " + str(centre[5]))
		axes = plt.gca()
		axes.set_ylim([np.min(scan[3:6]), np.max(scan[3:6])])
		plt.xlabel("offset [rad]")
		plt.ylabel(simMetricName)
		plt.legend()
		#plt.show()
		plt.savefig(outputPrefixExtended + "_rpy.png", format="png")
		plt.clf()
	'''
		
