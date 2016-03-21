"""
Creating and plotting unstructured triangular grids.
"""
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import math


nY = 5
nX = 5

hX = 1. / nX
hY = 1. / nY

print(hX)

xArr = np.tile([i * hX for i in xrange(0, nX + 1)], nY + 1)
yArr = []
[[ yArr.append(hY * i) for j in xrange(0, nX + 1)] for i in xrange(0, nY + 1)]

#for i in xrange(0, len(xArr)):
#    print (i, xArr[i], yArr[i])

triangles = []

print len(xArr), len(yArr)

for i in xrange(0, nY):
    for j in xrange(0, nX):
        index = j + i * (nX + 1)
        triangles.append([index, index + 1, index + nX + 2])
    for j in xrange(0, nX):
        index = j + (i + 1) * (nX + 1)
        triangles.append([index, j + i * (nX + 1), index + 1]);

plt.figure()
plt.gca().set_aspect('equal')
plt.triplot(xArr, yArr, triangles, 'go-')
plt.title('triplot of user-specified triangulation')
plt.xlim(-0.1, 1.1)
plt.ylim(-0.1, 1.1)
plt.xlabel('Longitude (degrees)')
plt.ylabel('Latitude (degrees)')

plt.show()