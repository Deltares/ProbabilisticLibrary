import math

def nstoch():
    return 7

def stochprop(i):
    if (i == 0):
        a = ["aa", "lognormal2", 120, 12.0, 0.0]
    if (i == 1):
        a = ["bb", "lognormal2", 120, 12.0, 0.0]
    if (i == 2):
        a = ["cc", "lognormal2", 120, 12.0, 0.0]
    if (i == 3):
        a = ["dd", "lognormal2", 120, 12.0, 0.0]
    if (i == 4):
        a = ["ee", "lognormal2", 50, 15.0, 0.0]
    if (i == 5):
        a = ["ff", "lognormal2", 40, 12.0, 0.0]
    if (i == 6):
        a = ["gg", "lognormal2", 40, 12.0, 0.0]

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
        return 0.001
    if (name == "startmethod"):
        return "zero"
    return -999

def z(x):
    
    z = x[0]+2*x[1]+2*x[2]+x[3]-5*x[4]-5*x[5]
    s = math.sin(100*x[0])+math.sin(100*x[1])+math.sin(100*x[2])+math.sin(100*x[3])+math.sin(100*x[4])+math.sin(100*x[5])
    
    z = z+0.001*s

    return z
