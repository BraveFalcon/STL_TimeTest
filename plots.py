import numpy as np
import matplotlib.pyplot as plt
import sys
import glob
from os import path

for mask in sys.argv[1:]:
    for file in glob.glob(mask):
        fname = path.split(file.split('.')[0])[1]
        data = np.loadtxt(file, skiprows=1)

        titles = open(file, 'r').readline().split()[1:]
        nums = data[:, 0]
        results = data[:, 1:]

        fig = plt.figure(figsize=(7, 5))
        ax = fig.add_subplot(111, xscale='log', yscale='log')
        for i in range(len(titles)):
            ax.plot(nums, results[:, i], 'o-', label=titles[i])

        ax.legend()
        ax.set_xlabel(open(file, 'r').readline().split()[0], fontsize=15)
        ax.set_ylabel('time, s', fontsize=15)
        ax.set_title(fname, fontsize=20)
        fig.savefig(file.split('.')[0] + '.png', dpi=200)
        fig.clear()
