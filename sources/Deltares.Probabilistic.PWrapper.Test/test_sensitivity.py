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
import unittest
import sys

from ptk import *

import project_builder

margin = 0.02

class Test_sensitivity(unittest.TestCase):

    def test_crude_monte_carlo_add_one(self):
        project = project_builder.get_sensitivity_add_one_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1, sens.mean, delta=margin)

        sens.distribution = DistributionType.uniform

        self.assertAlmostEqual(0, sens.minimum, delta=margin)
        self.assertAlmostEqual(2, sens.maximum, delta=margin)


    def test_crude_monte_carlo_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

    def test_crude_monte_carlo_auto_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.derive_samples_from_variation_coefficient = True

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

    def test_importance_sampling_add_one(self):
        project = project_builder.get_sensitivity_add_one_project()

        project.settings.sensitivity_method = SensitivityMethod.importance_sampling

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1, sens.mean, delta=margin)

        sens.distribution = DistributionType.uniform

        self.assertAlmostEqual(0, sens.minimum, delta=margin)
        self.assertAlmostEqual(2, sens.maximum, delta=margin)



if __name__ == '__main__':
    unittest.main()
