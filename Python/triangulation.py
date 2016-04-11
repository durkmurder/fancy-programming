"""
Creating and plotting unstructured triangular grids.
"""
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import math
import matplotlib.cm as cm


nY = 5
nX = 5

hX = 1. / nX
hY = 1. / nY

print(hX)

xArr = np.tile([i * hX for i in xrange(0, nX + 1)], nY + 1)
yArr = []
[[ yArr.append(hY * i) for j in xrange(0, nX + 1)] for i in xrange(0, nY + 1)]

def integrateTriangle(tri, F):
    degree2 = [ [[1 / 6., 1 / 6.], [2. / 3., 1 / 6.], [1 / 6., 2. / 3.]], 1. / 3 ]

    accum = 0.

    area = math.fabs(sum([
        xArr[tri[0]] * (yArr[tri[1]] - yArr[tri[2]]),
        xArr[tri[1]] * (yArr[tri[2]] - yArr[tri[0]]),
        xArr[tri[2]] * (yArr[tri[0]] - yArr[tri[1]])
                         ])) / 2.

    PFunc = P(xArr[tri[0]], xArr[tri[1]], xArr[tri[2]])
    QFunc = Q(yArr[tri[0]], yArr[tri[1]], yArr[tri[2]])
    for point in degree2[0]:
        accum += F(PFunc(point[0], point[1]), QFunc(point[0], point[1]))

    return area * degree2[1] * accum


def NodalFunction(ksi, eta, n):
    if n == 1:
        return 1 - ksi - eta
    elif n == 2:
        return ksi
    elif n == 3:
        return eta
    else:
        return 0.

def P(x1, x2, x3):
    return lambda ksi, eta: x1 * NodalFunction(ksi, eta, 1) + \
                            x2 * NodalFunction(ksi, eta, 2) + \
                            x3 * NodalFunction(ksi, eta, 3)

def Q(y1, y2, y3):
    return lambda ksi, eta: y1 * NodalFunction(ksi, eta, 1) + \
                            y2 * NodalFunction(ksi, eta, 2) + \
                            y3 * NodalFunction(ksi, eta, 3)

#print len(xArr), len(yArr)

triang = tri.Triangulation(xArr, yArr)

#print triang.triangles

def f(x, y):
    return 4.
    return x ** 2 + y ** 2

print integrateTriangle(triang.triangles[0], f)

triangles = triang.triangles

A = [[0. for x in xrange(0, len(xArr))] for j in xrange(0, len(xArr))]

F = [0. for i in xrange(0, len(xArr))]

for i in xrange(0, len(triangles)):
    for j in xrange(0, 3):
        for k in xrange(0, 3):
            A[triangles[i][j]][triangles[i][k]] += integrateTriangle(triangles[i], lambda arg1,arg2: -1.)

        F[triangles[i][j]] += integrateTriangle(triangles[i], f)



#A[len(triangles) - 1][:] = [0. for i in triangles]
#A[:][len(triangles) - 1] = [0. for i in triangles]
#F[-1] = 0.

#print len(A), len(A[0])

for row in A:
    print row

U = np.linalg.solve(A, F)

print U

plt.figure()
plt.gca().set_aspect('equal')
cmap = cm.get_cmap(name='terrain', lut=None)
#plt.triplot(triang, 'bo-')
plt.tripcolor(triang, U)
plt.colorbar()
plt.title('triplot of Delaunay triangulation')

plt.show()