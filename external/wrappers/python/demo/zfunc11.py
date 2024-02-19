import math

def nstoch():
    return 6

def stochprop(i):
    if (i == 0):
        a = ["aa", "lognormal2", 120, 12.0, 0.0, -999]
    if (i == 1):
        a = ["bb", "lognormal2", 120, 12.0, 0.0, -999]
    if (i == 2):
        a = ["cc", "lognormal2", 120, 12.0, 0.0, -999]
    if (i == 3):
        a = ["dd", "lognormal2", 120, 12.0, 0.0, -999]
    if (i == 4):
        a = ["ee", "lognormal2", 50, 15.0, 0.0, -999]
    if (i == 5):
        a = ["ff", "lognormal2", 40, 12.0, 0.0, -999]

    return a

def ncorr():
    return 0

def corrpair(i):
    a = [0, 1, 0.0]
    return a

def method():
    return "ds"

def settings(name):
    if (name == "tol"):
        return 0.05
    if (name == "minSamples"):
        return 5000
    if (name == "maxSamples"):
        return 50000
    if (name == "rnd"):
        return "gm"
    if (name == "seed1"):
        return 1234
    if (name == "seed2"):
        return 1234
    if (name == "startmethod"):
        return "zero"
    if (name == "varianceFactor"):
        return 2.0
    if (name == "progressInterval"):
        return 1000
    if (name == "epsilonDu"):
        return 0.001
    return -999

def z(x):
    
    x1 = x[0]
    x2 = x[1]
    x3 = x[2]
    x4 = x[3]
    x5 = x[4]
    x6 = x[5]
    z = x1+2*x2+2*x3+x4-5*x5-5*x6
    s = math.sin(100*x1)+math.sin(100*x2)+math.sin(100*x3)+math.sin(100*x4)+math.sin(100*x5)+math.sin(100*x6)
    
    z = z+0.001*s
    
    return z
