"""
Creating and plotting unstructured triangular grids.
"""
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import math


nY = 5
nX = nY

hX = 1. / nX
hY = 1. / nY

print(hX)

xArr = np.tile([i * hX for i in xrange(0, nX + 1)], nY + 1)
yArr = []
for i in xrange(0, nX + 1):
    # np.repeat([hY * i], nY + 1)) #np.asarray([0., 0., 0., 0.5, 0.5, 0.5, 1.0, 1.0, 1.0])
    for j in xrange(0, nY + 1):
        yArr.append(hY * i)

print yArr
triangles = []

print len(xArr), len(yArr)

'''
[0, 1, 4],
[1, 2, 5],
[3, 4, 7],
[4, 5, 8]
'''

for i in xrange(0, nY):
    for j in xrange(0, nX):
        index = j + i * (nX + 1)
        triangles.append([index, index + 1, index + nX + 2])
    for j in xrange(0, nX):
        index = j + (i + 1) * (nX + 1)
        triangles.append([index, j + i * (nX + 1), index + 1]);

# Rather than create a Triangulation object, can simply pass x, y and triangles
# arrays to triplot directly.  It would be better to use a Triangulation object
# if the same triangulation was to be used more than once to save duplicated
# calculations.
plt.figure()
plt.gca().set_aspect('equal')
plt.triplot(xArr, yArr, triangles, 'go-')
plt.title('triplot of user-specified triangulation')
plt.xlim(-0.5, 1.5)
plt.ylim(-0.5, 1.5)
plt.xlabel('Longitude (degrees)')
plt.ylabel('Latitude (degrees)')

plt.show()