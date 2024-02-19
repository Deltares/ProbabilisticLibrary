import math

def nstoch():
    return 3

def stochprop(i):
    if (i == 0):
        a = ["aa", "uniform", 0, 3.0, 0.0]
    if (i == 1):
        a = ["bb", "uniform", 0, 3.0, 0.0]
    if (i == 2):
        a = ["cc", "uspace"]

    return a

def ncorr():
    return 0

def method():
    return "cm"

def settings(name):
    if (name == "tol"):
        return 0.01
    if (name == "minSamples"):
        return 40000
    if (name == "maxSamples"):
        return 80000
    if (name == "rnd"):
        return "gm"
    if (name == "seed1"):
        return 2345
    if (name == "seed2"):
        return 3456
    if (name == "progressInterval"):
        return 10000
    return -999

def z(x):

    prijs = 3
    if (x[0] < 1.0):
        prijs = 1
    elif (x[0] < 2.0):
        prijs = 2

    gok = 3
    if (x[1] < 1.0):
        gok = 1
    elif (x[1] < 2.0):
        gok = 2

    if (gok == prijs):
        if (x[2] < 0):
            opened = gok + 1
            if (opened == 4):
                 opened = 1
        else:
            opened = gok - 1
            if (opened == 0):
                 opened = 3
    else:
        opened = 6 - gok - prijs

    gok2 = gok

    if (prijs == gok2):
        return 1
    else:
        return -1
