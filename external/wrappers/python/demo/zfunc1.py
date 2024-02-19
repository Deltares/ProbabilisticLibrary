def nstoch():
    return 2

def stochprop(i):
    if (i == 0):
        a = ["aa", "normal", 3.14, 2.78]
    if (i == 1):
        a = ["bb", "lognormal2", 3.0, 2.0, 1.0 ]

    return a

def ncorr():
    return 1

def corrpair(i):
    a = [0, 1, 0.25]
    return a

def method():
    return "fdir"

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
        return "ray-search"
    return -999

def z(x):
    z = x[0] + x[1]*x[1] - 2.0

    return z
