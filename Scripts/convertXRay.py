import pydicom as pd
import numpy as np
import matplotlib.pyplot as plt
import sys

fileNamePrefix = "../Data/"
fileNameSuffix = "/x_ray/x_ray.dcm"

#datasetName = "N18"
#datasetName = "First"
#datasetName = "E1N11"
datasetName = sys.argv[1]

data = pd.dcmread(fileNamePrefix + datasetName + fileNameSuffix)

# !note: in DICOM files, all dimension/coordinate value pairs are ordered [row(s), column(s)] = [vertical, horizontal]

usingScaledSpacing = hasattr(data, 'PixelSpacing')
if not(usingScaledSpacing) and not(hasattr(data, 'ImagerPixelSpacing')):
	raise AttributeError("No pixel spacing field in X-ray DICOM file.")

if usingScaledSpacing:
	pixelSpacing = np.array(data.PixelSpacing, dtype=np.float32)
else:
	pixelSpacing = np.array(data.ImagerPixelSpacing, dtype=np.float32)

sz = np.array([data.Rows, data.Columns], dtype=np.uint16)

sourceDistance = data.DistanceSourceToPatient if usingScaledSpacing else data.DistanceSourceToDetector

#activeDimsMM = np.array(data.DetectorActiveDimensions, dtype=np.float32)
#fovDimsMM = np.array(data.FieldOfViewDimensions, dtype=np.float32)
#fovOriginMM = np.multiply(np.array(data.ImagerPixelSpacing, dtype=np.float32), np.array(data.FieldOfViewOrigin, dtype=np.float32))
#sourcePos = np.zeros(3, dtype=np.float32)
#sourcePos[0:2] = 0.5*fovDimsMM + fovOriginMM - 0.5*activeDimsMM
#sourcePos[2] = np.array(-sourceDistance, dtype=np.float32)

# now flipping to reverse to [horizontal, vertical]
sz = np.flip(sz);
pixelSpacing = np.flip(pixelSpacing);
#sourcePos[0:2] = np.flip(sourcePos[0:2])

outFile = open("/Users/eprager/Projects/General/Volume_Renderer/Volume_Renderer/Inputs/xray.dat", 'wb')

# !not atm 24# 16 byte header:
# 2; uint16: width
# 2; uint16: height
# 4; float32: pixel x spacing
# 4; float32: pixel y spacing
#!not atm# 4; float32: source x position
#!not atm# 4; float32: source y position
# 4; float32: source z position
outFile.write(sz.tobytes())
outFile.write(pixelSpacing.tobytes())
#outFile.write(sourcePos.tobytes())
outFile.write(np.array(-sourceDistance, dtype=np.float32).tobytes())

outFile.write(data.pixel_array.tobytes())

plt.imshow(data.pixel_array)
plt.show()

outFile.close()
