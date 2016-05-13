"""
Creating and plotting unstructured triangular grids.
"""
import matplotlib.pyplot as plt
import matplotlib.tri as tri
import numpy as np
import math
import matplotlib.cm as cm
from mpl_toolkits.mplot3d import Axes3D

nY = 32
nX = nY

a = 0.
b = 4.

hX = (b - a) / nX
hY = (b - a) / nY

print(hX)

X = np.arange(a, b + hX, hX)
Y = np.arange(a, b + hY, hY)

Y, X = np.meshgrid(X, Y)

xArr = X.flatten()
yArr = Y.flatten()


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


def ShapeFunction(coefs, x, y):

    form = [1, x, y]

    if len(coefs) == 3:
        form = [1, x, y]
    elif len(coefs) == 6:
        form = [1, x, y, x * x, x * y, y * y]
    else:
        return 0.

    return sum([a * b for (a, b) in zip(coefs, form)])


def DShapeFunction(coefs, area, x, y, dx=1):

    d_ksi_x = 1.
    d_ksi_y = 1.
    d_eta_x = 1.
    d_eta_y = 1.

    d_psi_x = lambda ksi, eta: 0.
    d_psi_y = lambda ksi, eta: 0.

    if len(coefs) == 3:

        if dx == 1:
            return lambda ksi, eta: coefs[1] / 2. / area
        else:
            return lambda ksi, eta: coefs[2] / 2. / area
    else:

        d_psi_x = lambda ksi, eta, coefs: coefs[1] + 2 * ksi * coefs[3] + coefs[4] * eta
        d_psi_y = lambda ksi, eta, coefs: coefs[2] + ksi * coefs[4] + eta * coefs[5] * 2

    d_phi_x = lambda ksi, eta, coefs: d_psi_x(ksi, eta, coefs) * d_ksi_x + d_psi_y(ksi, eta, coefs) * d_eta_x
    d_phi_y = lambda ksi, eta, coefs: d_psi_x(ksi, eta, coefs) * d_ksi_y + d_psi_y(ksi, eta, coefs) * d_eta_y

    return lambda ksi, eta: d_phi_x(ksi, eta, coefs_i) * d_phi_x(ksi, eta, coefs_j) + \
                            d_phi_y(ksi, eta, coefs_i) * d_phi_y(ksi, eta, coefs_j)



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

def CasterianToBarycentric(triangle):
    x = [xArr[value] for value in triangle]
    y = [yArr[value] for value in triangle]

    detT = (y[1] - y[2]) * (x[0] - x[2]) + (x[2] - x[1]) * (y[0] - y[2])

    l1 = lambda ksi, eta: ((y[1] - y[2]) * (ksi - x[2]) + (x[2] - x[1]) * (eta - y[2])) / detT
    l2 = lambda ksi, eta: ((y[2] - y[0]) * (ksi - x[2]) + (x[0] - x[2]) * (eta - y[2])) / detT
    l3 = lambda ksi, eta: 1 - l1(ksi, eta) - l2(ksi, eta)

    return (l1, l2, l3)

triang = tri.Triangulation(xArr, yArr)

def f(x, y):
    return 4.

def p(x, y):
    return 1.

def q(x, y):
    #return 0.
    return -70.

def r(x, y):
    return 0.
    #return -8.

triangles = triang.triangles

def printMatrix(A):
    for row in A:
        print reduce(lambda accum, value: accum + "%2.2f " % value, row, "")

N = (nX + 1) * (nY + 1)

A = [[0. for x in xrange(0, N)] for j in xrange(0, N)]

F = [0. for i in xrange(0, N)]

def GeneratePoints(order, triangle):

    x = [xArr[value] for value in triangle]
    y = [yArr[value] for value in triangle]

    if order == 3:
        return x, y

    elif order == 6:
        xReturn = [x[0], (x[0] + x[1]) / 2, x[1], (x[1] + x[2]) / 2, x[2], (x[2] + x[0]) / 2]
        yReturn = [y[0], (y[0] + y[1]) / 2, y[1], (y[1] + y[2]) / 2, y[2], (y[2] + y[0]) / 2]
        return xReturn, yReturn
    else:
        return 0.


print "Integral: ", integrateTriangle(triangles[1], lambda arg1,arg2: -1. )

for i in xrange(0, len(triangles)):
    tri = triangles[i]

    area = integrateTriangle(tri, lambda x, y: 1.)

    linear = True
    x = []
    y = []
    points = []
    if linear:
        x,y = GeneratePoints(3, tri)
        points = [[1, x[k], y[k]] for k in xrange(0, len(x))]
    else:
        x, y = GeneratePoints(6, tri)
        points = [[1, x[k], y[k], x[k] ** 2, x[k] * y[k], y[k] ** 2] for k in xrange(0, len(x))]

    coefs = np.linalg.inv(points).transpose() * hX * hY

    shapeFunc = lambda ksi, eta, index: ShapeFunction(coefs[index], ksi, eta) / 2. / area

    str = ""
    gold = ""
    for j in xrange(0, 3):
        value = integrateTriangle(tri, lambda ksi, eta: shapeFunc(ksi, eta, j) * f(ksi, eta))
        str += "%f " % value
        F[tri[j]] += value

        for k in xrange(0, 3):
            first = integrateTriangle(tri, lambda ksi, eta:
                                     p(ksi, eta) *
                                     (DShapeFunction(coefs[j], area, x, y, dx=1)(ksi, eta) *
                                      DShapeFunction(coefs[k], area, x, y, dx=1)(ksi, eta) +
                                      DShapeFunction(coefs[j], area, x, y, dx=0)(ksi, eta) *
                                      DShapeFunction(coefs[k], area, x, y, dx=0)(ksi, eta)))

            second = integrateTriangle(tri, lambda ksi, eta:
                                             q(ksi, eta) *
                                             (DShapeFunction(coefs[j], area, x, y, dx=1)(ksi, eta) +
                                              DShapeFunction(coefs[j], area, x, y, dx=0)(ksi, eta)) *
                                             shapeFunc(ksi, eta, k))


            third = integrateTriangle(tri, lambda ksi, eta:
                                     r(ksi, eta) *
                                     shapeFunc(ksi, eta, j) *
                                     shapeFunc(ksi, eta, k))
            ak = first + second + third
            A[tri[j]][tri[k]] += ak
    #print str

boundary = [np.arange(0, nY + 1, 1),
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

U = np.linalg.solve(A, F)
#print "U: ", U
print len(U)

'''
END OF SOLVING FOR U, STARTING ERROR ESTIMATION
'''

E = [[0. for x in xrange(0, N)] for j in xrange(0, N)]
EF = [0. for i in xrange(0, N)]

def DShapeBubble(coefs, x, y, dx=1):

    a1, b1, c1 = coefs[0][0], coefs[0][1], coefs[0][2]
    a2, b2, c2 = coefs[1][0], coefs[1][1], coefs[1][2]
    a3, b3, c3 = coefs[2][0], coefs[2][1], coefs[2][2]

    if dx == 1:
        return b3 * (a1 + b1 * x + c1 * y) * (a2+b2*x+c2*y) + b2 * (a1+b1*x+c1*y) * (a3+b3*x+c3*y) + b1 * (a2+b2*x+c2*y) * (a3+b3*x+c3*y)
    else:
        return c3 * (a1 + b1 * x + c1 * y) * (a2+b2*x+c2*y) + c2 * (a1+b1*x+c1*y) * (a3+b3*x+c3*y) + c1 * (a2+b2*x+c2*y) * (a3+b3*x+c3*y)


for i in xrange(0, len(triangles)):
    tri = triangles[i]

    area = integrateTriangle(tri, lambda x, y: 1.)

    linear = True
    x = []
    y = []
    points = []
    x, y = GeneratePoints(3, tri)
    points = [[1, x[k], y[k]] for k in xrange(0, len(x))]

    coefs = np.linalg.inv(points).transpose() * hX * hY

    shapeFunc = lambda ksi, eta, index: ShapeFunction(coefs[0], ksi, eta) * \
                                 ShapeFunction(coefs[1], ksi, eta) * \
                                 ShapeFunction(coefs[2], ksi, eta) #* 27 / 8. / (area ** 3)

    dShapeFunc = lambda ksi, eta, dx: DShapeFunction(coefs[0], area, x, y, dx)(ksi, eta) * ShapeFunction(coefs[1], ksi, eta) * ShapeFunction(coefs[2], ksi, eta) + \
                                      DShapeFunction(coefs[1], area, x, y, dx)(ksi, eta) * ShapeFunction(coefs[0], ksi, eta) * ShapeFunction(coefs[2], ksi, eta) + \
                                      DShapeFunction(coefs[2], area, x, y, dx)(ksi, eta) * ShapeFunction(coefs[0], ksi, eta) * ShapeFunction(coefs[1], ksi, eta) #* \
                                      #27 / 8. / (area ** 3)

    uh = lambda ksi, eta: sum([U[tri[k]] * ShapeFunction(coefs[k], ksi, eta) for k in xrange(0, 3)]) / 2. / area
    duh = lambda ksi, eta, dx: sum([U[tri[k]] * DShapeFunction(coefs[k], area, x, y, dx)(ksi, eta) for k in xrange(0, 3)])

    first = integrateTriangle(tri, lambda ksi, eta:
                                     p(ksi, eta) *
                                     (dShapeFunc(ksi, eta, dx=1) *
                                      duh(ksi, eta, dx=1) +
                                      dShapeFunc(ksi, eta, dx=0) *
                                      duh(ksi, eta, dx=0)))

    second = integrateTriangle(tri, lambda ksi, eta:
                                             q(ksi, eta) *
                                             (duh(ksi, eta, dx=1) +
                                              duh(ksi, eta, dx=0)) * shapeFunc(ksi, eta, k))


    third = integrateTriangle(tri, lambda ksi, eta:
                                     r(ksi, eta) *
                                     shapeFunc(ksi, eta, j) *
                                     uh(ksi, eta))
    str = ""
    gold = ""
    for j in xrange(0, 3):
        value = integrateTriangle(tri, lambda ksi, eta: shapeFunc(ksi, eta, j) * f(ksi, eta)) - first - second - third
        str += "%f " % value
        EF[tri[j]] += value

        for k in xrange(0, 3):
            first = integrateTriangle(tri, lambda ksi, eta:
                                     p(ksi, eta) *
                                     (dShapeFunc(ksi, eta, dx=1) *
                                      dShapeFunc(ksi, eta, dx=1) +
                                      dShapeFunc(ksi, eta, dx=0) *
                                      dShapeFunc(ksi, eta, dx=0)))

            second = integrateTriangle(tri, lambda ksi, eta:
                                             q(ksi, eta) *
                                             (dShapeFunc(ksi, eta, dx=1) +
                                              dShapeFunc(ksi, eta, dx=0)) *
                                             shapeFunc(ksi, eta, k))


            third = integrateTriangle(tri, lambda ksi, eta:
                                     r(ksi, eta) *
                                     shapeFunc(ksi, eta, j) *
                                     shapeFunc(ksi, eta, k))

            ak = first + second + third
            E[tri[j]][tri[k]] += ak
    #print str

# printMatrix(E)


for row in boundary:
    for value in row:
        for i in xrange(0, len(E)):
            E[value][i] = 0.
            E[i][value] = 0.
            if i == value:
                E[i][value] = 1.
        EF[value] = 0.

U2 = np.linalg.solve(E, EF)

print N, len(U2)

Z2 = np.array([0. for i in xrange(0, N)])

for i in xrange(0, len(triangles)):
    tri = triangles[i]

    area = integrateTriangle(tri, lambda x, y: 1.)

    linear = True
    x = []
    y = []
    points = []
    x, y = GeneratePoints(3, tri)
    points = [[1, x[k], y[k]] for k in xrange(0, len(x))]

    coefs = np.linalg.inv(points).transpose() * hX * hY

    shapeFunc = lambda index, ksi, eta: ShapeFunction(coefs[0], ksi, eta) * \
                                 ShapeFunction(coefs[1], ksi, eta) * \
                                 ShapeFunction(coefs[2], ksi, eta) / 2. / area

    for j in xrange(0, 3):
        Z2[tri[j]] += abs(integrateTriangle(tri, lambda ksi, eta: (abs(sum([shapeFunc(coefs[k], ksi, eta) * U2[tri[k]] for k in xrange(0, 3)])) ** 2)))


Z2 = Z2.reshape(X.shape)
Z = U.reshape(X.shape)

plt.figure(1)
plt.gca().set_aspect('equal')
cmap = cm.get_cmap(name='terrain', lut=None)
plt.tripcolor(xArr, yArr, triangles, U, edgecolors='k')
plt.colorbar()
plt.title('triplot of Delaunay triangulation')

fig = plt.figure(2)
ax = fig.gca(projection='3d')
surf = ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

fig = plt.figure(3)
ax = fig.gca(projection='3d')
surf = ax.plot_surface(X, Y, Z2, rstride=1, cstride=1, cmap=cm.coolwarm,
                       linewidth=0, antialiased=False)

plt.show()