import sys
import math

from ptk import *

def linear_ab(a, b):
    L = 1.8
    return L - (a+b)

def linear_bc(b, c):
    L = 1.8
    return L - (b + c)

def linear_abc(a, b, c):
    return 1.8 - (a+b+c)

def linear_small_ab(a, b):
    return 1.98 - (a+b)

def linear_small_abc(a, b, c):
    return 1.98 - (a+b+c)

def bligh(m, L, c_creep, delta_H):
    return m * L / c_creep - delta_H

def get_linear_project():

    project = ReliabilityProject()

    project.model = linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution = 'uniform'
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution = 'uniform'
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_small_project():

    project = ReliabilityProject()

    project.model = linear_small_ab

    stochast1 = project.variables['a']
    stochast1.distribution = 'uniform'
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution = 'uniform'
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_fully_correlated_project():

    project = ReliabilityProject()

    project.model = linear_abc

    stochast1 = project.variables['a']
    stochast1.distribution = 'uniform'
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5;

    stochast2 = project.variables['b']
    stochast2.distribution = 'uniform'
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5;

    stochast3 = project.variables['c']
    stochast3.distribution = 'uniform'
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0

    project.correlation_matrix[('a', 'b')] = 1.0

    return project

def get_linear_partially_correlated_project():

    project = ReliabilityProject()

    project.model = linear_abc

    stochast1 = project.variables['a']
    stochast1.distribution = 'uniform'
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5

    stochast2 = project.variables['b']
    stochast2.distribution = 'uniform'
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5

    stochast3 = project.variables['c']
    stochast3.distribution = 'uniform'
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0

    project.correlation_matrix[('a', 'b')] = 0.5

    return project

def get_bligh_project():

    project = ReliabilityProject()

    project.model = bligh

    project.variables["m"].distribution = "log_normal"
    project.variables["m"].mean = 1.76
    project.variables['m'].deviation = 1.69

    project.variables["L"].distribution = "normal"
    project.variables["L"].mean = 50
    project.variables["L"].deviation = 2.5

    project.variables["c_creep"].distribution = "deterministic"
    project.variables["c_creep"].mean = 18

    project.variables["delta_H"].distribution = "gumbel"
    project.variables["delta_H"].shift = 0.53
    project.variables["delta_H"].scale = 0.406

    return project

def get_design_point(beta : float, count : int):
    design_point = DesignPoint()
    design_point._set_reliability_index(beta)

    for i in range(count):
        alpha = Alpha()
        
        variable = Stochast()
        variable.distribution = 'uniform'
        variable.minimum = 0
        variable.maximum = 1

        design_point._add_alpha(variable, - math.sqrt (1.0 / float(count)))

    return design_point







