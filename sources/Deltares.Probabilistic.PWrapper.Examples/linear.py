import sys
from ptk import *

margin = 0.01

def linear(a, b):
    return 1.8 - (a+b)

project = Project()

project.model = linear

stochast1 = project.get_variable('a')
stochast1.distribution = 'uniform'
stochast1.minimum = -1
stochast1.maximum = 1

stochast2 = project.get_variable('b')
stochast2.distribution = 'uniform'
stochast2.minimum = -1
stochast2.maximum = 1

project.run()

dp = project.design_point
beta = dp.reliability_index
alphas = dp.alphas

print('beta = ' + str(beta))


