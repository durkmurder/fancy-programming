import numpy as np

def func(x):
    return (x / 2. + 1) * np.sin(x);


a = 1.0
b = 3.0
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
    
    
x = 2.4

table.append(data)

for i in xrange(1, len(data)):
    table.append([])
    for j in xrange(0, len(table[i - 1]) - 1):
        table[i].append(table[i - 1][j + 1] - table[i - 1][j])
        
    
iValue = 0
t = (x - b) / h

numerator = 1

for i in xrange (N, -1, -1):
   iValue += (table[N - i][i] * numerator) / factorials[N - i]
   numerator *= t
   t += 1

print func(x)
print iValue
    
    



