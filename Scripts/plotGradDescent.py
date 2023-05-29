import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt("../Output_files/gradientDescent.txt")

N, D = np.shape(data)

vX = np.linspace(0.0, 1.0, N)
for col in np.transpose(data):
	col -= np.mean(col)
	plt.plot(vX, col)

plt.show()
