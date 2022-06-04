from cmath import log
from math import ceil, floor, log2

intSum = 1
intMul = 3
intDiv = 90
floatDiv = 27
lambdas = 6
beta = 125*(10^6)

def calculo(n,p):
    tbf = (3*intSum + 2*intMul)*(n/p)+intSum*(2*p-1)+floatDiv+(p-1)*(2*lambdas + (8*(n/p)+4)/beta)
    tbt = 5*intSum*(ceil(log2(p))+1)+(n/p)*(2*intSum+intMul)+intDiv*ceil(log2(p))+floatDiv
    for i in range(ceil(log2(p))):
        for j in range(ceil(log2(i)), ceil(log2(p))):
            tbt += (lambdas+(8*(n/2^(j+1)) + 4)/beta) 
    return (tbf, tbt)

def calculoH(n,p,c):
    tbf = (3*intSum + 2*intMul)*((n/p)/c)+intSum*(2*p-1)+floatDiv+(p-1)*(2*lambdas + (8*(n/p)+4)/beta)
    tbt = 5*intSum*(ceil(log2(p))+1)+((n/p)/c)*(2*intSum+intMul)+intDiv*ceil(log2(p))+floatDiv
    for i in range(ceil(log2(p))):
        for j in range(ceil(log2(i)), ceil(log2(p))):
            tbt += (lambdas+(8*(n/2^(j+1)) + 4)/beta) 
    return (tbf, tbt)

def main():
    pass