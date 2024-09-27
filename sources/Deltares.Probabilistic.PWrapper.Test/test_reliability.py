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
import unittest
import sys

from streams import *

import project_builder

margin = 0.01

class Test_reliability(unittest.TestCase):

    def test_form_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

        self.assertEqual(len(project.variables), len(project.design_point.alphas))
        self.assertTrue(project.design_point.alphas[0].variable in project.variables)
        self.assertEqual('a', project.design_point.alphas[0].variable.name)

        self.assertEqual(0, len(dp.messages))

    def test_form_limit_state_functions(self):
        project = project_builder.get_multiple_unbalanced_linear_project()

        self.assertEqual(3, len(project.model.output_parameters))
        self.assertEqual('x', project.model.output_parameters[0].name)
        self.assertEqual('y', project.model.output_parameters[1].name)
        self.assertEqual('z', project.model.output_parameters[2].name)

        project.limit_state_function.parameter = 'y'
        project.limit_state_function.compare_type = CompareType.less_than
        project.limit_state_function.critical_value = 0.1

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.65, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        # self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        # self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        # self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
        # self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

        # self.assertEqual(len(project.variables), len(project.design_point.alphas))
        # self.assertTrue(project.design_point.alphas[0].variable in project.variables)
        # self.assertEqual('a', project.design_point.alphas[0].variable.name)

        # self.assertEqual(0, len(dp.messages))


    # def test_form_linear_fragility_curve(self):
    #     project = project_builder.get_linear_project()

    #     project.settings.reliability_method = 'form'
    #     project.settings.reliability_result = 'fragility_curve'
    #     project.settings.fragility_variable = 'a'
    #     project.settings.fragility_values = [1, 2, 3]

    #     project.run();

    #     dp = project.fragility_curve

    #     self.assertAlmostEqual(2.33, beta, delta=margin)
    #     self.assertEqual(2, len(alphas))

    #     self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
    #     self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

    #     self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
    #     self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

    #     self.assertEqual(len(project.variables), len(project.design_point.alphas))
    #     self.assertTrue(project.design_point.alphas[0].variable in project.variables)
    #     self.assertEqual('a', project.design_point.alphas[0].variable.name)


    def test_form_linear_run_twice(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

        self.assertEqual(len(project.variables), len(project.design_point.alphas))
        self.assertTrue(project.design_point.alphas[0].variable in project.variables)

        project.correlation_matrix[(project.variables[0], project.variables[1])] = 0.5
        project.run();

        dp = project.design_point;

        self.assertAlmostEqual(1.90, dp.reliability_index, delta=margin)


    def test_form_linear_fully_correlated(self):
        project = project_builder.get_linear_fully_correlated_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(3, len(alphas))

        self.assertAlmostEqual(0.45, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.45, alphas[1].x, delta=margin)
        self.assertAlmostEqual(0.9, alphas[2].x, delta=margin)

        self.assertAlmostEqual(alphas[0].alpha_correlated, alphas[1].alpha_correlated, delta=margin)

    def test_form_linear_partially_correlated(self):
        project = project_builder.get_linear_partially_correlated_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.15, beta, delta=margin)

        self.assertAlmostEqual(0.45, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.45, alphas[1].x, delta=margin)

        self.assertAlmostEqual(alphas[0].alpha_correlated, alphas[1].alpha_correlated, delta=margin)

    def test_crude_monte_carlo_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo
        project.settings.random_type = RandomType.mersenne_twister

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.58, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.70, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_directional_sampling_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling
        project.settings.random_type = RandomType.mersenne_twister

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.7, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_importance_sampling_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.importance_sampling
        project.settings.random_type = RandomType.mersenne_twister

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.58, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.7, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_importance_sampling_linear_small(self):
        project = project_builder.get_linear_small_project()

        project.settings.reliability_method = ReliabilityMethod.importance_sampling
        project.settings.random_type = RandomType.mersenne_twister
        project.settings.minimum_samples = 5000
        project.settings.maximum_samples = 10000

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.91, beta, delta=margin)

    def test_numerical_integration_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        self.assertEqual(2, len(project.settings.stochast_settings))

        for stochastSettings in project.settings.stochast_settings:
            stochastSettings.start_value = 0
            stochastSettings.variance_factor = 2

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.93, alphas[1].x, delta=margin)

    def test_numerical_integration_limited_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        project.settings.stochast_settings['a'].min_value = -1
        project.settings.stochast_settings['b'].min_value = -1

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_numerical_integration_bligh(self):
        project = project_builder.get_bligh_project()

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 50000
        project.settings.variation_coefficient = 0.02

        project.run()

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        project.settings.stochast_settings['m'].intervals = 20
        project.settings.stochast_settings['L'].intervals = 20
        project.settings.stochast_settings['delta_H'].intervals = 20

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertEqual(20*20*20+1, dp.total_model_runs)
        self.assertAlmostEqual(1.55, beta, delta=margin)
        self.assertEqual(4, len(alphas))

    def test_directional_sampling_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling

        project.settings.minimum_directions = 1000
        project.settings.maximum_directions = 2000
        project.settings.variation_coefficient = 0.02

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.88, beta, delta=margin)
        self.assertEqual(5, len(alphas))

if __name__ == '__main__':
    unittest.main()
