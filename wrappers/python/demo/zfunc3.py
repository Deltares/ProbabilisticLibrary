import math

def nstoch():
    return 2

def stochprop(i):
    if (i == 0):
        a = ["coh", "normal", 0.0, 1.0, -999, -999]
    if (i == 1):
        a = ["phi", "normal", 0.0, 1.0, -999, -999]

    return a

def ncorr():
    return 0

def corrpair(i):
    a = [0, 1, 0.0]
    return a

def method():
    return "form"

def settings(name):
    if (name == "tol"):
        return 0.01
    if (name == "minSamples"):
        return 5000
    if (name == "maxSamples"):
        return 50000
    if (name == "rnd"):
        return "gm"
    if (name == "seed1"):
        return 2345
    if (name == "seed2"):
        return 3456
    if (name == "startmethod"):
        return "one"
    return -999

def z(x):
    
    g1 = 0.1*(x[0]-x[1])**2-(x[0]+x[1])/math.sqrt(2.0)+3
    g2 = 0.1*(x[0]-x[1])**2+(x[0]+x[1])/math.sqrt(2.0)+3
    g3 = x[0]-x[1]+3*math.sqrt(2)
    g4 = -x[0]+x[1]+3*math.sqrt(2)
    
    z = min([g1,g2,g3,g4])
    
    return z
