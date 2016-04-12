"""
Creating and plotting unstructured triangular grids.
"""
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import math
import matplotlib.cm as cm


nY = 20
nX = nY

hX = 1. / nX
hY = 1. / nY

print(hX)

#xArr = np.tile([i * hX for i in xrange(0, nX + 1)], nY + 1)
#yArr = []
#[[yArr.append(hY * i) for j in xrange(0, nX + 1)] for i in xrange(0, nY + 1)]

X = np.arange(0, 1. + hX, hX)
Y = np.arange(0, 1. + hY, hY)

Y, X = np.meshgrid(X, Y)

xArr = X.flatten()
yArr = Y.flatten()

def integrateMasterTriangle(F):
    degree2 = [[[0., 1. / 2.], [1. / 2., 0.], [1. / 2., 1. / 2.]], 1. / 6]

    accum = 0.

    x = [xArr[value] for value in tri]
    y = [yArr[value] for value in tri]

    area = math.fabs(sum([
        x[0] * (y[1] - y[2]),
        x[1] * (y[2] - y[0]),
        x[2] * (y[0] - y[1])])) / 2.

    jac = area * 2

    PFunc = P(xArr[tri[0]], xArr[tri[1]], xArr[tri[2]])
    QFunc = Q(yArr[tri[0]], yArr[tri[1]], yArr[tri[2]])
    for point in degree2[0]:
        accum += F(PFunc(point[0], point[1]), QFunc(point[0], point[1]))

    return degree2[1] * accum * jac

def integrateTriangle(tri, F):
    degree4 = [[1. / 3, 1. / 3., -27. / 96],
               [2. / 15., 11. / 15, 25. / 96],
               [2. / 15., 2. / 15., 25. / 96],
               [11. / 15, 2. / 15, 25. / 96]]

    accum = 0.

    x = [xArr[value] for value in tri]
    y = [yArr[value] for value in tri]

    area = math.fabs(sum([
        x[0] * (y[1] - y[2]),
        x[1] * (y[2] - y[0]),
        x[2] * (y[0] - y[1])])) / 2.

    jac = area * 2

    PFunc = P(xArr[tri[0]], xArr[tri[1]], xArr[tri[2]])
    QFunc = Q(yArr[tri[0]], yArr[tri[1]], yArr[tri[2]])
    for point in degree4:
        accum += F(PFunc(point[0], point[1]), QFunc(point[0], point[1])) * point[2]

    return accum * jac


def NodalFunction(ksi, eta, n):
    if n == 1:
        return 1. - ksi - eta
    elif n == 2:
        return ksi
    elif n == 3:
        return eta
    else:
        return 0.

def DNodalFunction(ksi, eta, n, der=0):
    if n == 1:
        return -1.
    elif n == 2:
        return ksi if der == 0 else 0.
    elif n == 3:
        return eta if der == 1 else 0.
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


def ShapeFunction(coefs, x, y, area):
    return (coefs[0] + coefs[1] * x + coefs[2] * y) / 2. / area


def CalculateCoefsForShapeFunction(xValues, yValues):
    perm = [[1, 1, 2], [2, 2, 0], [3, 0, 1]] # i, j, m (actually j-1 and m-1 for indexes)

    result = []
    for indexes in perm:
        i = indexes[0]
        j = indexes[1]
        m = indexes[2]
        result.append([
            xValues[j] * yValues[m] - xValues[m] * yValues[j],
            yValues[j] - yValues[m],
            xValues[m] - xValues[j]
        ])

    return result

#print len(xArr), len(yArr)

triang = tri.Triangulation(xArr, yArr)

#print triang.triangles

def f(x, y):
    return x ** 2 + y ** 2
    #return 4.
    #return x ** 2 + y ** 2

triangles = triang.triangles

# triangles = [
#     [0, 1, 4],
#     [3, 4, 7],
#     [1, 2, 5],
#     [4, 5, 8],
#     [0, 4, 3],
#     [3, 7, 6],
#     [1, 5, 4],
#     [4, 8, 7]
# ]

def printMatrix(A):
    for row in A:
        print reduce(lambda accum, value: accum + "%2.2f " % value, row, "")

N = (nX + 1) * (nY + 1)

A = [[0. for x in xrange(0, N)] for j in xrange(0, N)]

F = [0. for i in xrange(0, N)]


print "Integral: ", integrateTriangle(triangles[1], lambda arg1,arg2: -1. )

# for i in xrange(0, len(triangles)):
#     tri = triangles[i]
#
#     Pe = []
#     for x in tri:
#         Pe.append([1, xArr[x], yArr[x]])
#     C = np.linalg.inv(Pe)
#
#     grad = C[1:3, :]
#
#     area = integrateTriangle(tri, lambda arg1,arg2: 1.)
#
#     Ke = area * np.dot(grad.transpose(), grad)
#
#     printMatrix(Ke)
#     print ""
#
#     for j in xrange(0, 3):
#         for k in xrange(0, 3):
#             A[tri[j]][tri[k]] += Ke[j][k]
#         F[tri[j]] += (area / 3.)

print triangles

for i in xrange(0, len(triangles)):
    tri = triangles[i]

    area = integrateTriangle(tri, lambda x, y: 1.)

    x = [xArr[value] for value in tri]
    y = [yArr[value] for value in tri]

    d_ksi_x = (y[2] - y[0]) / area / 2.
    d_ksi_y = (x[2] - x[0]) / area / -2.
    d_eta_x = (y[1] - y[0]) / area / -2.
    d_eta_y = (x[1] - x[0]) / area / 2.

    dphi_x = lambda ksi, eta, index: DNodalFunction(ksi, eta, index, 0) * d_ksi_x + DNodalFunction(ksi, eta, index, 1) * d_eta_x
    dphi_y = lambda ksi, eta, index: DNodalFunction(ksi, eta, index, 0) * d_ksi_y + DNodalFunction(ksi, eta, index, 1) * d_eta_y

    coefs = CalculateCoefsForShapeFunction(x, y)

    shapeFunc = lambda ksi, eta, index: ShapeFunction(coefs[index], ksi, eta, area)

    str = ""
    gold = ""
    for j in xrange(0, 3):
        value = integrateTriangle(tri, lambda ksi, eta: shapeFunc(ksi, eta, j) * f(ksi, eta))
        str += "%f " % value
        F[tri[j]] += value

        for k in xrange(0, 3):
            ak = (coefs[j][1] * coefs[k][1] + coefs[j][2] * coefs[k][2]) / 4. / area
            A[tri[j]][tri[k]] += ak
    #print str


#print "F:", F

#A[len(triangles) - 1][:] = [0. for i in triangles]
#A[:][len(triangles) - 1] = [0. for i in triangles]
#F[-1] = 0.

#print len(A), len(A[0]# )

boundary = [ np.arange(0, nY + 1, 1),
             np.arange(nY + 1, (nY + 1) * (nX + 1), nY + 1),
             np.arange(2 * (nY + 1) - 1, (nY + 1) * (nX + 1) + 1, nY + 1),
             np.arange((nY + 1) * (nX + 1) - nY, (nY + 1) * (nX + 1) - 1, 1)]

#print boundary


for row in boundary:
    for value in row:
        for i in xrange(0, len(A)):
            A[value][i] = 0.
            A[i][value] = 0.
            if i == value:
                A[i][value] = 1.
        F[value] = 0.


#printMatrix(A)


U = np.linalg.solve(A, F)
print "U: ", U

plt.figure()
plt.gca().set_aspect('equal')
cmap = cm.get_cmap(name='terrain', lut=None)
#plt.triplot(triang, 'bo-')
#plt.triplot(xArr, yArr, triangles)
plt.tripcolor(xArr, yArr, triangles, U, edgecolors='k')
#lt.colorbar()
plt.xlim(-0.5, 1.5)
plt.ylim(-0.5, 1.5)
plt.title('triplot of Delaunay triangulation')



plt.show()