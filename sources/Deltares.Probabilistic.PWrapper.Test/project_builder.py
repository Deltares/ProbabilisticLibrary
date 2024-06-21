import sys
import interface

from stochast import *
from project import *

def linear(values):
    return 1.8 - sum(values)

def linear_small(values):
    return 1.98 - sum(values)


def get_linear_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = "uniform"
    stochast1.minimum = -1
    stochast1.maximum = 1;
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = "uniform"
    stochast2.minimum = -1
    stochast2.maximum = 1;
    project.variables.append(stochast2)

    project.model = linear

    return project

def get_linear_small_project():

    project = Project()

    stochast1 = Stochast()
    stochast1.distribution = "uniform"
    stochast1.minimum = -1
    stochast1.maximum = 1;
    project.variables.append(stochast1)

    stochast2 = Stochast()
    stochast2.distribution = "uniform"
    stochast2.minimum = -1
    stochast2.maximum = 1;
    project.variables.append(stochast2)

    project.model = linear_small

    return project





