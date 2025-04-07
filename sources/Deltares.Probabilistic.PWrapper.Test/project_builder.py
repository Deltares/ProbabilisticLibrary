# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of the Probabilistic Library.
#
# The Probabilistic Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
import sys
import math
import numpy as np

from probabilistic_library import *
from time import sleep

def sum_ab(a, b):
    return a+b;

def linear_ab(a, b):
    L = 1.8
    return L - (a+b)

def linear_ab_array_result(a, b):
    L = [1.8, 1.9, 1.95]

    return [x - (a+b) for x in L]

def linear_delayed_ab_array_result(a, b):
    L = [1.8, 1.9, 1.95]

    sleep(0.001)

    return [x - (a+b) for x in L]

class InitializedLinearModel:
    def __init__(self, L):
        self._L = L
    def calculate(self, a, b):
        return self._L - (a+b)

def linear_ab_array(L:float, a:list[float], b:list[float]):
    z = L
    for i in range(len(a)):
        z = z - a[i] - b[i]
    return z

def unbalanced_linear_ab(a, b):
    L = 1.8
    return L - (0.3 * a + 1.9 * b)

def multiple_unbalanced_linear_ab(a, b):
    L = 1.8
    x = L - (0.3 * a + 1.9 * b)
    y = L - (0.6 * a + 1.3 * b)
    z = L - (0.1 * a + 1.6 * b)
    return x, y, z

def multiple_identical_linear_ab(a, b):
    L = 1.8
    x = L - (a + b)
    y = L - (a + b)
    z = L - (a + b)
    return x, y, z

def linear_bc(b, c):
    L = 1.8
    return L - (b + c)

def linear_cd(c, d):
    L = 1.8
    return L - (c + d)

def linear_abc(a, b, c):
    return 1.8 - (a+b+c)

def linear_small_ab(a, b):
    return 1.98 - (a+b)

def linear_small_abc(a, b, c):
    return 1.98 - (a+b+c)

def bligh(m, L, c_creep, delta_H):
    return m * L / c_creep - delta_H

def hunt(T_p, tan_alpha, H_s, h_crest, h):
    g = 9.81
    L_0 = g  * T_p * T_p
    xi = tan_alpha / np.sqrt(2 * np.pi * H_s / L_0)
    R_u = xi * H_s

    return h_crest - (h + R_u)

def pile(Load, z, q_clay, q_sand, D, L):
    A = 0.25 * math.pi * D * D
    L_clay = min(L, z)
    L_sand = max(0, L - z)

    q_i = q_sand if L > z else q_sand * ((z-L) / 0.7 * A) + q_clay * (1 - (z-L)/0.7 * A)
    q_ii = q_i
    q_iii = q_sand if L_sand > 8 * A else q_sand * (L_sand/8*A) + q_clay * (1 - L_sand/(8*A))
    q_tip = 0.25 * q_i + 0.25 * q_ii + 0.5 * q_iii
    p_tip = A * q_tip
    p_shaft = math.pi * D * (q_clay * L_clay + q_sand * L_sand)
    p = p_tip + p_shaft

    UC = Load / p

    return UC

def get_linear_project():

    project = ReliabilityProject()

    project.model = linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_initialized_project():

    project = ReliabilityProject()
    initialized = InitializedLinearModel(1.8)

    project.model = initialized.calculate

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_array_result_project():

    project = ReliabilityProject()

    project.model = linear_ab_array_result, 3

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project
def get_linear_array_project():

    project = ReliabilityProject()

    project.model = linear_ab_array

    stochast1 = project.variables['L']
    stochast1.distribution = DistributionType.deterministic
    stochast1.mean = 1.8

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_multiple_linear_project():

    project = ReliabilityProject()

    project.model = multiple_linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_add_one_project():

    project = SensitivityProject()

    project.model = sum_ab

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.deterministic
    stochast1.mean = 1

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_linear_project():

    project = SensitivityProject()

    project.model = linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_linear_array_result_project() ->SensitivityProject:

    project = SensitivityProject()

    project.model = (linear_ab_array_result, 3)

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_linear_delayed_array_result_project():

    project = SensitivityProject()

    project.model = (linear_delayed_ab_array_result, 3)

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_pile_project():

    project = SensitivityProject()

    project.model = pile

    load = project.variables['Load']
    load.distribution =  DistributionType.gumbel
    load.design_quantile = 0.95
    load.mean = 1
    load.variation = 0.1
    load.design_value = 1E5;

    z = project.variables['z']
    z.distribution =  DistributionType.normal
    z.mean = 10
    z.deviation = 0.2

    d = project.variables['D']
    d.distribution =  DistributionType.normal
    d.mean = 0.2
    d.deviation = 0.04
    d.truncated = True
    d.minimum = 0
    d.maximum = 1

    l = project.variables['L']
    l.distribution =  DistributionType.normal
    l.mean = 12
    l.deviation = 0.8

    s = project.variables['q_sand']
    s.distribution =  DistributionType.log_normal
    s.mean = 500
    s.deviation = 50

    c = project.variables['q_clay']
    c.distribution =  DistributionType.log_normal
    c.mean = 25000
    c.deviation = 400

    return project

def get_sensitivity_multiple_identical_linear_project():

    project = SensitivityProject()

    project.model = multiple_identical_linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_multiple_unbalanced_linear_project():

    project = ReliabilityProject()

    project.model = multiple_unbalanced_linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_sensitivity_multiple_unbalanced_linear_project():

    project = SensitivityProject()

    project.model = multiple_unbalanced_linear_ab

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution =  DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_small_project():

    project = ReliabilityProject()

    project.model = linear_small_ab

    stochast1 = project.variables['a']
    stochast1.distribution =  DistributionType.uniform
    stochast1.minimum = -1
    stochast1.maximum = 1;

    stochast2 = project.variables['b']
    stochast2.distribution = DistributionType.uniform
    stochast2.minimum = -1
    stochast2.maximum = 1;

    return project

def get_linear_fully_correlated_project():

    project = ReliabilityProject()

    project.model = linear_abc

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5;

    stochast2 = project.variables['b']
    stochast2.distribution = DistributionType.uniform
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5;

    stochast3 = project.variables['c']
    stochast3.distribution = DistributionType.uniform
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0

    project.correlation_matrix[('a', 'b')] = 1.0

    return project

def get_linear_partially_correlated_project():

    project = ReliabilityProject()

    project.model = linear_abc

    stochast1 = project.variables['a']
    stochast1.distribution = DistributionType.uniform
    stochast1.minimum = -0.5
    stochast1.maximum = 0.5

    stochast2 = project.variables['b']
    stochast2.distribution = DistributionType.uniform
    stochast2.minimum = -0.5
    stochast2.maximum = 0.5

    stochast3 = project.variables['c']
    stochast3.distribution = DistributionType.uniform
    stochast3.minimum = -1.0
    stochast3.maximum = 1.0

    project.correlation_matrix[('a', 'b')] = 0.5

    return project

def get_bligh_project():

    project = ReliabilityProject()

    project.model = bligh

    project.variables["m"].distribution = DistributionType.log_normal
    project.variables["m"].mean = 1.76
    project.variables['m'].deviation = 1.69

    project.variables["L"].distribution = DistributionType.normal
    project.variables["L"].mean = 50
    project.variables["L"].deviation = 2.5

    project.variables["c_creep"].distribution = DistributionType.deterministic
    project.variables["c_creep"].mean = 18

    project.variables["delta_H"].distribution = DistributionType.gumbel
    project.variables["delta_H"].shift = 0.53
    project.variables["delta_H"].scale = 0.406

    return project

def get_hunt_project():

    project = ReliabilityProject()

    project.model = hunt

    project.variables["T_p"].distribution = DistributionType.log_normal
    project.variables["T_p"].mean = 6
    project.variables["T_p"].deviation = 2

    project.variables["tan_alpha"].distribution = DistributionType.deterministic
    project.variables["tan_alpha"].mean = 0.333333

    project.variables["H_s"].distribution = DistributionType.log_normal
    project.variables["H_s"].mean = 3
    project.variables["H_s"].deviation = 1

    project.variables["h_crest"].distribution = DistributionType.log_normal
    project.variables["h_crest"].mean = 10
    project.variables["h_crest"].deviation = 0.05

    project.variables["h"].distribution = DistributionType.exponential
    project.variables["h"].shift = 0.5
    project.variables["h"].scale = 1

    return project

def get_design_point(beta : float, count : int):
    design_point = DesignPoint()
    design_point._set_reliability_index(beta)

    for i in range(count):
        variable = Stochast()
        variable.distribution = DistributionType.uniform
        variable.minimum = 0
        variable.maximum = 1

        design_point._add_alpha(variable, - math.sqrt (1.0 / float(count)))

    return design_point

def get_design_point_with_name(beta : float, count : int, names):
    design_point = DesignPoint()
    design_point._set_reliability_index(beta)

    for i in range(count):
        variable = Stochast()
        variable.distribution = DistributionType.uniform
        variable.minimum = 0
        variable.maximum = 1
        variable.name = names[i]

        design_point._add_alpha(variable, - math.sqrt (1.0 / float(count)))

    return design_point

