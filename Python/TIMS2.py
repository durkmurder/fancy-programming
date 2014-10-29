import math
import numpy

def phiSmall(x):
	return ((1.0) / math.sqrt(2 * math.pi)) * math.exp(-x ** 2 / 2.0)

def phiLarge (x):
	return 0.5 * math.erf(x / math.sqrt(2))

inputData = {150: 24, 160: 112, 170: 273, 180: 302, 190: 212, 200: 66, 210: 11}

workData = [x[1] for x in sorted(inputData.items())]
print workData
segments = [x[0] for x in sorted(inputData.items())]
print segments

n = sum(inputData.values())
mainData = [inputData.keys()[i] * inputData.values()[i] for i in range(len(inputData))]
print mainData

a = sum(mainData) / n
sigma = math.sqrt(sum(inputData.keys()[i] ** 2 * inputData.values()[i] for i in range(len(inputData))) / n - a ** 2)

print a, sigma

niValues = []

piValues = [phiLarge((segments[i + 1] - a) / sigma) - phiLarge((segments[i] - a) / sigma) for i in range(len(inputData) - 1)]
piValues += [1 - sum(piValues)]

print sum(piValues)

npiValues = [n * piValues[i] for i in range(len(inputData))]

xiEmp = sum((workData[i] - npiValues[i]) ** 2 / npiValues[i] for i in range(len(workData) - 1))

print xiEmp

#cryt = 9.488

raw_input("Press any key to continue...")
