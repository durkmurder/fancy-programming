import numpy as np
import matplotlib.pyplot as plt

def func(x):
    return x * np.cos(x);


a = 0
b = np.pi / 4
N =  20
n =  N / 2
h = (b - a) / N

table = []
factorials = []
data = []

factorials.append(1)
data.append(func(a))
for i in xrange(1, N + 1):
    factorials.append(factorials[i - 1] * i)
    data.append(func(a + i * h))
    
    
x = np.pi / 4

table.append(data)

for i in xrange(1, len(data)):
    table.append([])
    for j in xrange(0, len(table[i - 1]) - 1):
        table[i].append(table[i - 1][j + 1] - table[i - 1][j])
        

newton = []
def forwardNewton(x):
    iValue = 0
    t = (x - b) / h

    numerator = 1

    for i in xrange (0, N + 1):
        temp = (table[i][N - i] * numerator) / factorials[i]
        iValue += temp
        numerator *= t
        t -= 1
        newton.append(temp)
    return iValue
    
gauss = []
def backwardGauss(x):
    iValue = table[0][n]
    gauss.append(iValue)
    t = (x - (a + (b - a) / 2.)) / h
    numeratorEven = t
    numeratorOdd = t * (t - 1)
    i = 1
    while i <= n:
        temp = (table[2 * i - 1][n - i + 1] * numeratorEven) / factorials[i * 2 - 1]
        iValue += temp
        gauss.append(temp)
        temp = (table[2 * i][n - i] * numeratorOdd) / factorials[i * 2]
        iValue += temp
        gauss.append(temp)
        i += 1
        numeratorEven *= (t + i - 1) * (t - i + 1)
        numeratorOdd *= (t + i - 1) * (t - i)
    return iValue

print func(x), forwardNewton(x), backwardGauss(0.1), func(0.1)

xValues = [a + i * h for i in xrange (0, N + 1)]
plt.plot(xValues, gauss)
plt.plot(xValues, newton)
plt.plot(xValues, data)
plt.show();
    
    



