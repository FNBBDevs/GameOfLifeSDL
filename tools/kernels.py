import math
import numpy as np

# def computeVariance(halfWidth, epsilon=0.001):
#     return -(halfWidth + 1.0) * (halfWidth + 1.0) / 2.0 / math.log(epsilon)

# def gaussianExponent(y, x, sigma):
#     return math.e ** (- (x*x + y*y) / (2 * sigma))

# def makeGaussian(halfWidth):

#     kernel = [[0 for _ in range(halfWidth * 2)] for _ in range(halfWidth * 2)]

#     # sigma = computeVariance(halfWidth)
#     sigma = 1

#     sum = 0

#     for iloop in range(-halfWidth, halfWidth):
#         i = halfWidth + iloop
#         for jloop in range(-halfWidth, halfWidth):
#             j = halfWidth + jloop
#             print(j)
#             kval = gaussianExponent(iloop, jloop, sigma)
#             kernel[i][j] = kval
#             sum += kval
#         print()
    
#     for i in range(halfWidth * 2 - 1):
#         for j in range(halfWidth * 2 - 1):
#             kernel[i][j] /= sum
#     print("GAUSSIAN = {",end="")
#     for i in range(halfWidth * 2 - 1):
#         print("{",end="")
#         for j in range(halfWidth * 2 - 1):
#             print(kernel[i][j], end=", ")
#         print("},")
#     print("};")

# makeGaussian(2)

# from math import exp
# def gaussian(x, mu, sigma):
#     return exp( -(((x-mu)/(sigma))**2)/2.0 )

# #kernel_height, kernel_width = 7, 7
# kernel_radius = 1 # for an 7x7 filter
# sigma = kernel_radius/2. # for [-2*sigma, 2*sigma]

# # compute the actual kernel elements
# hkernel = [gaussian(x, kernel_radius, sigma) for x in range(2*kernel_radius+1)]
# vkernel = [x for x in hkernel]
# kernel2d = [[xh*xv for xh in hkernel] for xv in vkernel]

# # normalize the kernel elements
# kernelsum = sum([sum(row) for row in kernel2d])
# kernel2d = [[x/kernelsum for x in row] for row in kernel2d]

# print("{")
# for outer in kernel2d:
#     print("{",end='')
#     for inner in outer:
#         print(inner, end=",")
#     print("},")
# print("};")

# def laplacianOuterFull(size):
#     assert size % 2 == 1
#     kernel = [[0 for _ in range(size)] for _ in range(size)]
#     centerCell = int((size / 2))
#     for i in range(size):
#         for j in range(size):
#             if i == centerCell and j == centerCell:
#                 kernel[i][j] = (-size * size) + 1
#             else:
#                 kernel[i][j] = 1
#     print("{")
#     for outer in kernel:
#         print("{",end='')
#         for inner in outer:
#             print(inner, end=",")
#         print("},")
#     print("};")

def laplacianOuterEmpty(size, epsilon):
    assert size % 2 == 1
    kernel = [[0 for _ in range(size)] for _ in range(size)]
    left = -1 / (math.pi * epsilon**4)
    
    for i in range(size):
        for j in range(size):
            scaledI = i - ((size - 1) / 2)
            scaledJ = j - ((size - 1) / 2)
            inner = 1 - ((scaledI**2 + scaledJ**2) / (2 * epsilon ** 2))

            product = left * inner

            right = -((scaledI**2 + scaledJ**2) / (2 * epsilon ** 2))
            e = math.e ** right
            p = product * e
            kernel[i][j] = p * (-40/-0.1624)

def LoG(sigma, x, y):
    laplace = -1/(np.pi*sigma**4)*(1-(x**2+y**2)/(2*sigma**2))*np.exp(-(x**2+y**2)/(2*sigma**2))
    return laplace

def LoG_discrete(sigma, n):
    l = np.zeros((n,n))
    for i in range(n):
        for j in range(n):
            l[i,j] = LoG(sigma, (i-(n-1)/2),(j-(n-1)/2))
    return l

sigma = 1.4

l = np.round(LoG_discrete(sigma, 5)*(-16/LoG(sigma,0,0)))

print("{")
for outer in l:
    print("{",end='')
    for inner in outer:
        print(inner, end=",")
    print("},")
print("};")

# def laplacianOuterEmpty(size, epsilon):
#     assert size % 2 == 1
#     kernel = [[0 for _ in range(size)] for _ in range(size)]
    
#     for i in range(size):
#         for j in range(size):
#             inp = i - (size-1)/2
#             in2 = j - (size-1)/2
#             Xpower = -((inp**2)/(2 * epsilon ** 2))
#             Ypower = -((in2**2)/(2 * epsilon ** 2))
#             root = (1 / (math.sqrt(2 ** math.pi) * epsilon))
#             Xexp = math.e ** Xpower
#             Yexp = math.e ** Ypower

#             Xtotal = (root * Xexp)
#             YTotal = (root * Yexp)

#             kernel[i][j] = (Xtotal * YTotal) * 100
            


#     print("{")
#     for outer in kernel:
#         print("{",end='')
#         for inner in outer:
#             print(inner, end=",")
#         print("},")
#     print("};")
