points = [('25', '3.43', '341'), ('25', '3.36', '343'), ('25', '3.45', '340'), ('37', '4.12', '473'), ('37', '4.27', '474'), ('37', '4.20', '466'), ('55', '4.65', '651'), ('55', '4.57', '649'), ('55', '4.77', '651'), ('82', '4.87', '907'), ('82', '4.86', '910'), ('82', '4.91', '908'), ('123', '4.87', '1212'), ('123', '4.88', '1214'), ('123', '4.99', '1207'), ('184', '4.98', '1629'), ('184', '5.02', '1634'), ('184', '5.00', '1633'), ('276', '5.04', '2194'), ('276', '5.03', '2179'), ('276', '5.03', '2195'), ('414', '5.04', '3259'), ('414', '5.04', '3258'), ('414', '5.04', '3215'), ('621', '5.04', '4561'), ('621', '5.04', '4555'), ('621', '5.04', '4566'), ('931', '5.04', '6343'), ('931', '5.04', '6341'), ('931', '2.98', '59884'), ('1000', '5.04', '6657'), ('1000', '5.04', '6655'), ('1000', '5.04', '6631')]

import numpy as np 
import matplotlib.pyplot as plt
import pandas as pd
from collections import defaultdict

y = defaultdict(float)
x = defaultdict(float)
for p in points:
	y[p[0]] += float(p[1])
	x[p[0]] += float(p[2])
for k,v in y.items():
	y[k] = v/3.0
for k,v in x.items():
	x[k] = v/3.0
avg = list()
for z in set([p[0] for p in points]):
	avg.append((z,y[z],x[z]))

z,y,x = [p[0] for p in avg], [p[1] for p in avg], [p[2] for p in avg]
fig, ax = plt.subplots()
print x
print y
print z
ax.scatter(x,y)

for X, Y, Z in zip(x, y, z):
    # Annotate the points 5 _points_ above and to the left of the vertex
    ax.annotate('{}'.format(Z), xy=(X,Y), xytext=(5, 5), ha='right',
                textcoords='offset points')
plt.ylabel('Throughput (Mbit/s)')
plt.xlabel('Delay (ms)')
plt.xscale('log')
plt.gca().invert_xaxis()
plt.show()