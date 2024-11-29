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

    def test_form_linear_array(self):
        project = project_builder.get_linear_array_project()

        self.assertEqual(False, project.variables['L'].is_array)
        self.assertEqual(True, project.variables['a'].is_array)
        self.assertEqual(True, project.variables['b'].is_array)

        project.variables['a'].is_array = 10
        project.variables['b'].is_array = 10

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        # self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(21, len(alphas))

        # self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        # self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        # self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
        # self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

        # self.assertEqual(len(project.variables), len(project.design_point.alphas))
        # self.assertTrue(project.design_point.alphas[0].variable in project.variables)
        # self.assertEqual('a', project.design_point.alphas[0].variable.name)

        # self.assertEqual(0, len(dp.messages))

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

        self.assertAlmostEqual(1.99, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.31, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.95, alphas[1].alpha, delta=margin)

        project.limit_state_function.critical_value = 'z'

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.0, beta, delta=margin)
        self.assertEqual(2, len(alphas))


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

    def test_form_start_methods(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.sphere_search
        project.settings.all_quadrants = True

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 150)
        self.assertFalse(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 1)

        self.assertAlmostEqual(1.9877, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_start_methods_given_vector(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.fixed_value
        project.settings.stochast_settings["a"].start_value = 1.2
        project.settings.stochast_settings["b"].start_value = 2.4

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 150)
        self.assertFalse(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 0)

        self.assertAlmostEqual(1.9877, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_start_methods_max_steps(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.sphere_search
        project.settings.all_quadrants = True
        project.settings.max_steps_sphere_search = 25

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 6)
        self.assertTrue(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 1)
        self.assertEqual(176, dp.contributing_design_points[0].total_model_runs)

        self.assertAlmostEqual(2.3258, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_numerical_bisection_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.numerical_bisection

        project.settings.minimum_iterations = 5
        project.settings.maximum_iterations = 6
        project.settings.epsilon_beta = 0.1

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.861, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_latin_hypercube_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.latin_hypercube

        project.settings.minimum_samples = 25000

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.8987, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_cobyla_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.cobyla_reliability

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.326, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

    def test_fdir_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.form_then_directional_sampling

        project.settings.maximum_iterations = 50
        project.settings.epsilon_beta = 0.05

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.05, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_dsfi_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling_then_form

        project.settings.maximum_iterations = 50
        project.settings.epsilon_beta = 0.05

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.87, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_fragility_curve(self):
        fragility_curve = FragilityCurve()
        fragility_curve.name = 'q'
        
        value1 = FragilityValue()
        value1.x = 0
        value1.reliability_index = 4.2
        fragility_curve.fragility_values.append(value1)

        value2 = FragilityValue()
        value2.x = 10
        value2.reliability_index = 2.6
        fragility_curve.fragility_values.append(value2)
        
        integrand = Stochast()
        integrand.name = 'h'
        integrand.distribution = DistributionType.normal
        integrand.mean = 5
        integrand.deviation = 1
        
        dp = fragility_curve.integrate(integrand)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.35, beta, delta=margin)
        self.assertEqual(2, len(alphas))
        self.assertEqual(integrand, alphas[0].variable)
        self.assertEqual(fragility_curve, alphas[1].variable)

    def test_fragility_curve_hunt(self):
        project = project_builder.get_hunt_project()
        project.settings.reliability_method = ReliabilityMethod.form
        
        h_stochast = Stochast()
        h_stochast.name = 'h'
        h_stochast.copy_from(project.variables['h'])

        fragility_curve = FragilityCurve()
        fragility_curve.name = 'q'
        
        for h_value in [1, 2, 3]:
            project.variables['h'].distribution = DistributionType.deterministic
            project.variables['h'].mean = h_value
            project.run()
            dp = project.design_point;
        
            value = FragilityValue()
            value.x = 10
            value.design_point = dp
            fragility_curve.fragility_values.append(value)

        dp = fragility_curve.integrate(h_stochast)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.79, beta, delta=margin)
        self.assertEqual(6, len(alphas))
        self.assertEqual(h_stochast, alphas[0].variable)

if __name__ == '__main__':
    unittest.main()
