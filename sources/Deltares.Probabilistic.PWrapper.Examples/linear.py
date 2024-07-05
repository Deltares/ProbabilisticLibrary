import sys
from ptk import *

margin = 0.01

def linear(values):
    return 1.8 - sum(values)

project = Project()

stochast1 = Stochast()
stochast1.distribution = 'uniform'
stochast1.minimum = -1
stochast1.maximum = 1
project.variables.append(stochast1)

stochast2 = Stochast()
stochast2.distribution = 'uniform'
stochast2.minimum = -1
stochast2.maximum = 1
project.variables.append(stochast2)

project.model = linear

project.run()

dp = project.design_point
beta = dp.reliability_index
alphas = dp.alphas

print('beta = ' + str(beta))


