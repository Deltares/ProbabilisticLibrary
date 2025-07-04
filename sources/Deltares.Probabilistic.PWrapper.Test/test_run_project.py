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
import unittest
import sys

from probabilistic_library import *

import project_builder

margin = 0.02

class Test_run_project(unittest.TestCase):

    def test_run_mean_values(self):
        project = project_builder.get_run_linear_project()
        project.settings.run_values_type = RunValuesType.mean_values

        project.run();

        self.assertAlmostEqual(1.13, project.realization.output_values[0], delta=margin)

    def test_run_median_values(self):
        project = project_builder.get_run_linear_project()
        project.settings.run_values_type = RunValuesType.median_values

        project.run();

        self.assertAlmostEqual(0.97, project.realization.output_values[0], delta=margin)

    def test_run_design_values(self):
        project = project_builder.get_run_linear_project()
        project.settings.run_values_type = RunValuesType.design_values
        project.variables['b'].design_quantile = 0.95
        project.variables['b'].design_factor = 1.5

        project.run();

        self.assertAlmostEqual(0.75, project.realization.output_values[0], delta=margin)

    def test_run_from_reliability_project(self):
        reliability_project = project_builder.get_linear_project()
        reliability_project.variables['a'].distribution = DistributionType.triangular
        reliability_project.variables['a'].shift = 1
        reliability_project.variables['b'].distribution = DistributionType.triangular
        reliability_project.variables['b'].shift = 1

        project = RunProject()
        project.model = reliability_project.model

        project.settings.run_values_type = RunValuesType.mean_values

        project.run();

        self.assertAlmostEqual(1.13, project.realization.output_values[0], delta=margin)

    def test_run_reuse_calculations(self):
        project = RunProject()
        initialized = project_builder.InitializedLinearModel(1.8)
        initialized.delay = 0.002

        project.model = initialized.calculate
        project_builder.assign_distributions(project, DistributionType.uniform)

        project.settings.reuse_calculations = True
        project.run();

        self.assertAlmostEqual(1.8, project.realization.output_values[0], delta=margin)

        initialized._L = 1.7

        project.run();

        # expecting old result
        self.assertAlmostEqual(1.8, project.realization.output_values[0], delta=margin)

        project.settings.reuse_calculations = False
        project.run();

        # expecting new result
        self.assertAlmostEqual(1.7, project.realization.output_values[0], delta=margin)


if __name__ == '__main__':
    unittest.main()
