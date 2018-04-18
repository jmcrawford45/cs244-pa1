points = [('0.1', '2.48', '275'), ('2', '2.72', '264'), ('0.5', '2.95', '261'), ('1.0 (Debug)', '3.4', '238'), ('1.0 (No debug)', '3.83', '350')]

import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
from collections import defaultdict
import math

y = defaultdict(float)
x = defaultdict(float)
for p in points:
	y[p[0]] += float(p[1])
	x[p[0]] += float(p[2])
avg = list()
for z in set([p[0] for p in points]):
	avg.append((z,y[z],x[z]))


z,throughput,delay = [p[0] for p in avg], [p[1] for p in avg], [p[2] for p in avg]
fig, ax = plt.subplots()
for p in avg:
	print p[2]
	print 'Size:{} Score:{}'.format(p[0], p[1]/(p[2]*0.001))
ax.scatter(delay,throughput)

for X, Y, Z in zip(delay, throughput, z):
    # Annotate the points 5 _points_ above and to the left of the vertex
    ax.annotate('{}'.format(Z), xy=(X,Y), xytext=(5, 5), ha='right',
                textcoords='offset points')
plt.ylabel('Throughput (Mbit/s)')
plt.xlabel('Delay (ms)')
plt.xscale('log')
plt.gca().invert_xaxis()
plt.show()