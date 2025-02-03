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

class Test_combine(unittest.TestCase):
    def test_directional_sampling_series(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = CombinerMethod.directional_sampling
        project.settings.combine_type = CombineType.series

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(2 * q - q * q), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])
        
    def test_importance_sampling_series(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = CombinerMethod.importance_sampling
        project.settings.combine_type = CombineType.series

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(2 * q - q * q), project.design_point.reliability_index, delta=margin)
        
    def test_directional_sampling_parallel(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = CombinerMethod.directional_sampling
        project.settings.combine_type = CombineType.parallel

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(q * q), project.design_point.reliability_index, delta=margin)
        
    def test_importance_sampling_parallel(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method =  CombinerMethod.importance_sampling
        project.settings.combine_type = CombineType.parallel

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(q * q), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])

    def test_calculated_design_points(self):

        project = ReliabilityProject()

        project.model = project_builder.linear_ab;

        project.variables['a'].distribution = DistributionType.uniform
        project.variables['a'].minimum = -1
        project.variables['a'].maximum = 1

        project.variables['b'].distribution = DistributionType.uniform
        project.variables['b'].minimum = -1
        project.variables['b'].maximum = 1

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo
        project.settings.maximum_samples = 100000
        project.run()
        dp1 = project.design_point

        beta_expected = StandardNormal.get_u_from_q(0.005)
        self.assertAlmostEqual(beta_expected, dp1.reliability_index, delta=margin)

        project.model = project_builder.linear_bc;

        self.assertEqual(DistributionType.uniform,  project.variables['b'].distribution)

        project.variables['c'].distribution = DistributionType.uniform
        project.variables['c'].minimum = -1
        project.variables['c'].maximum = 1

        project.run()
        dp2 = project.design_point

        self.assertAlmostEqual(beta_expected, dp2.reliability_index, delta=margin)

        project = CombineProject()
        project.design_points.append(dp1)
        project.design_points.append(dp2)

        project.settings.combiner_method = CombinerMethod.hohenbichler
        project.settings.combine_type = CombineType.series
        project.run()

        dp_correlated = project.design_point

        self.assertEqual(2,  len(dp_correlated.contributing_design_points))
        self.assertEqual(3,  len(dp_correlated.alphas))

        # correlated result
        beta_expected = 2.34
        self.assertAlmostEqual(beta_expected, dp_correlated.reliability_index, delta=margin)

        project.correlation_matrix['b'] = 0

        project.run()
        dp_uncorrelated = project.design_point

        self.assertNotEqual(dp_correlated.reliability_index, dp_uncorrelated.reliability_index)
        
        # uncorrelated result
        beta_expected = StandardNormal.get_u_from_q(0.01)
        self.assertAlmostEqual(beta_expected, dp_uncorrelated.reliability_index, delta=margin)

    def test_excluding_weighted_sum(self):

        project = ExcludingCombineProject()

        q1 = 0.01;
        beta1 = StandardNormal.get_u_from_q(q1)
        dp1 = project_builder.get_design_point(beta1, 2)
        project.design_points.append(dp1)
        project.scenarios.append(0.25)

        q2= 0.04;
        beta2 = StandardNormal.get_u_from_q(q2)
        dp2 = project_builder.get_design_point(beta2, 2)
        project.design_points.append(dp2)
        project.scenarios.append(0.55)

        self.assertFalse(project.is_valid());

        for message in project.validate():
            print(str(message))

        project.scenarios[1] = 0.75

        self.assertTrue(project.is_valid());

        project.settings.combiner_method =  ExcludingCombinerMethod.weighted_sum

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(0.25 * q1 + 0.75 * q2), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])
        self.assertEqual(len(dp1.alphas) + len(dp2.alphas), len(project.design_point.alphas))

    def test_excluding_hohenbichler(self):

        project = ExcludingCombineProject()

        q1 = 0.01;
        beta1 = StandardNormal.get_u_from_q(q1)
        dp1 = project_builder.get_design_point(beta1, 2)
        project.design_points.append(dp1)
        project.scenarios.append(0.25)

        q2= 0.04;
        beta2 = StandardNormal.get_u_from_q(q2)
        dp2 = project_builder.get_design_point(beta2, 2)
        project.design_points.append(dp2)
        project.scenarios.append(0.75)

        project.settings.combiner_method =  ExcludingCombinerMethod.hohenbichler

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(0.25 * q1 + 0.75 * q2), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])
        self.assertEqual(len(dp1.alphas) + len(dp2.alphas), len(project.design_point.alphas))

        
if __name__ == '__main__':
    unittest.main()
