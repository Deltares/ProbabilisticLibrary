# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of Streams.
#
# Streams is free software: you can redistribute it and/or modify
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
from streams import ReliabilityProject

margin = 0.01

def linear(a, b):
    return 1.8 - (a+b)

project = ReliabilityProject()

project.model = linear

stochast1 = project.variables['a']
stochast1.distribution = 'uniform'
stochast1.minimum = -1
stochast1.maximum = 1

stochast2 = project.variables['b']
stochast2.distribution = 'uniform'
stochast2.minimum = -1
stochast2.maximum = 1

project.run()

dp = project.design_point
beta = dp.reliability_index
alphas = dp.alphas

print('beta = ' + str(beta))


