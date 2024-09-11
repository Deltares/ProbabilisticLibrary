# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of the Probabilistic Library.
#
# The Probabilistic Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
import sys
import math

from ptk import *

def sum_ab(a, b):
    return a+b;

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

def get_design_point(beta : float, count : int):
    design_point = DesignPoint()
    design_point._set_reliability_index(beta)

    for i in range(count):
        alpha = Alpha()
        
        variable = Stochast()
        variable.distribution = DistributionType.uniform
        variable.minimum = 0
        variable.maximum = 1

        design_point._add_alpha(variable, - math.sqrt (1.0 / float(count)))

    return design_point







