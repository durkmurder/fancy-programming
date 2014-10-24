import numpy as np
import matplotlib.pyplot as plt

tvalues = []
yvalues = []

def euler(f, y0, a, b, h, prev):
    del tvalues[:]
    del yvalues[:]
    result = []
    t, y = a, y0
    i = 0
    while t < (b + h):
	tvalues.append(t)
        yvalues.append(y)
	
	if not prev:
	    t += h
	    y += h * f(t, y)
	else:
	    t += h
	    y += h * (f(t, y) + f(t, prev[i])) / 2.0
	    i += 1
	    
	result.append(y)
    return result
    
 
def function(time, y):
	return 0.218 * (time ** 2 + np.sin(1.6 * time)) + 0.718 * y
 
  
  
  
'''  
euler(function, 0.30, 0.25, 1.25, 0.2)
plt.plot(tvalues, yvalues)
euler(function, 0.30, 0.25, 1.25, 0.25)
plt.plot(tvalues, yvalues)
'''




def solveEquation(func, y0, a, b, h):
    
    prev = []
    current = euler(func, y0, a, b, h, prev)
    epsilon = 10 ** (-4)

    while True:
	prev = current
	current = euler(function, y0, a, b, h, prev)
	if (abs(prev[-1] - current[-1]) < epsilon):
	    break;

    #euler(function, 0.30, 0.25, 1.25, 0.1)
    plt.plot(tvalues, yvalues)

solveEquation(function, 0.30, 0.25, 1.25, 0.1)
solveEquation(function, 0.30, 0.25, 1.25, 0.01)
solveEquation(function, 0.30, 0.25, 1.25, 0.2)

plt.show()

