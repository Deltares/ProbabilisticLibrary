import sys
import interface

from statistics import Stochast
from project import Project

def linear(values):
    return 1.8 - sum(values)

def linear_small(values):
    return 1.98 - sum(values)


def get_linear_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = 'uniform'
    stochast1.minimum = -1
    stochast1.maximum = 1;
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = 'uniform'
    stochast2.minimum = -1
    stochast2.maximum = 1;
    project.variables.append(stochast2)

    project.model = linear

    return project

def get_linear_small_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = 'uniform'
    stochast1.minimum = -1
    stochast1.maximum = 1;
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = 'uniform'
    stochast2.minimum = -1
    stochast2.maximum = 1;
    project.variables.append(stochast2)

    project.model = linear_small

    return project

def get_linear_fully_correlated_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = 'uniform'
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5;
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = 'uniform'
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5;
    project.variables.append(stochast2)

    stochast3 = Stochast()
    stochast3.distribution = 'uniform'
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0
    project.variables.append(stochast3)

    project.correlation_matrix.variables.extend(project.variables)
    project.correlation_matrix.set_correlation(stochast1, stochast2, 1.0)

    project.model = linear

    return project

def get_linear_partially_correlated_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = 'uniform'
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = 'uniform'
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5
    project.variables.append(stochast2)

    stochast3 = Stochast()
    stochast3.distribution = 'uniform'
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0
    project.variables.append(stochast3)

    project.correlation_matrix.variables.extend(project.variables)
    project.correlation_matrix.set_correlation(stochast1, stochast2, 0.5)

    project.model = linear

    return project





