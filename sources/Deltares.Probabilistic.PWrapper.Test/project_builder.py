import sys
import interface
import model

from model import Stochast
from model import Settings
from model import Project


def linear(values):
    return 1.8 - sum(values)


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





