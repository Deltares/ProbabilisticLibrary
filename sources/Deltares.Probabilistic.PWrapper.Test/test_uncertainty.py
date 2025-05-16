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
from io import StringIO

from probabilistic_library import *

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

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

    def test_crude_monte_carlo_array_result_linear(self):

        project = project_builder.get_sensitivity_linear_array_result_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.variation_coefficient = 0
        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 1000
        project.settings.quantiles.append(0.5)
        project.settings.quantiles.append(0.95)

        project.run();

        self.assertEqual(3, len(project.stochasts))

        stochast1 = project.stochasts[0];
        self.assertEqual('x - (a+b) for x in L[0]' , stochast1.name)
        self.assertAlmostEqual(1.8, stochast1.mean, delta=margin)
        self.assertAlmostEqual(0.83, stochast1.deviation, delta=margin)

        stochast2 = project.stochasts[1];
        self.assertEqual('x - (a+b) for x in L[1]' , stochast2.name)
        self.assertAlmostEqual(1.9, stochast2.mean, delta=margin)
        self.assertAlmostEqual(0.83, stochast2.deviation, delta=margin)

        stochast3 = project.stochasts[2];
        self.assertEqual('x - (a+b) for x in L[2]' , stochast3.name)
        self.assertAlmostEqual(1.95, stochast3.mean, delta=margin)
        self.assertAlmostEqual(0.83, stochast3.deviation, delta=margin)

        self.assertLessEqual(2.5 * project.settings.maximum_samples, project.total_model_runs)

        result1 = project.results[0];
        self.assertEqual('x - (a+b) for x in L[0]' , result1.identifier)
        self.assertEqual(2, len(result1.quantile_realizations))
        self.assertAlmostEqual(1.82, result1.quantile_realizations[0].output_values[0], delta=margin)
        self.assertAlmostEqual(3.14, result1.quantile_realizations[1].output_values[0], delta=margin)
        self.assertEqual(project.stochast, project.result.variable)


    def test_crude_monte_carlo_array_result_linear_delayed(self):

        project = project_builder.get_sensitivity_linear_delayed_array_result_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.variation_coefficient = 0
        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 1000
        project.settings.max_parallel_processes = 1

        project.run();

        self.assertEqual(3, len(project.stochasts))

        sens1 = project.stochasts[0];
        self.assertEqual('x - (a+b) for x in L[0]' , sens1.name)
        self.assertAlmostEqual(1.8, sens1.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens1.deviation, delta=margin)

        sens2 = project.stochasts[1];
        self.assertEqual('x - (a+b) for x in L[1]' , sens2.name)
        self.assertAlmostEqual(1.9, sens2.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens2.deviation, delta=margin)

        sens3 = project.stochasts[2];
        self.assertEqual('x - (a+b) for x in L[2]' , sens3.name)
        self.assertAlmostEqual(1.95, sens3.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens3.deviation, delta=margin)

        self.assertEqual(project.settings.maximum_samples, project.total_model_runs)

    def test_crude_monte_carlo_array_result_linear_delayed_parallel(self):

        project = project_builder.get_sensitivity_linear_delayed_array_result_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.variation_coefficient = 0
        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 1000
        project.settings.max_parallel_processes = 4

        project.run();

        self.assertEqual(3, len(project.stochasts))

        s1 = project.stochasts[0];
        self.assertEqual('x - (a+b) for x in L[0]' , s1.name)
        self.assertAlmostEqual(1.8, s1.mean, delta=margin)
        self.assertAlmostEqual(0.83, s1.deviation, delta=margin)

        s2 = project.stochasts[1];
        self.assertEqual('x - (a+b) for x in L[1]' , s2.name)
        self.assertAlmostEqual(1.9, s2.mean, delta=margin)
        self.assertAlmostEqual(0.83, s2.deviation, delta=margin)

        s3 = project.stochasts[2];
        self.assertEqual('x - (a+b) for x in L[2]' , s3.name)
        self.assertAlmostEqual(1.95, s3.mean, delta=margin)
        self.assertAlmostEqual(0.83, s3.deviation, delta=margin)

        self.assertLessEqual(project.settings.maximum_samples, project.total_model_runs)

    def test_crude_monte_carlo_multiple_identical_linear(self):
        project = project_builder.get_sensitivity_multiple_identical_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.calculate_correlations = True

        project.run();

        sens = project.stochast;

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertAlmostEqual(1.0, project.output_correlation_matrix['x','y'], delta=margin)

    def test_crude_monte_carlo_multiple_unbalanced_linear(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.calculate_correlations = True

        project.run();

        sens = project.stochast;

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertAlmostEqual(0.96, project.output_correlation_matrix['x','y'], delta=margin)

    def test_crude_monte_carlo_multiple_unbalanced_linear_input(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.calculate_correlations = True
        project.settings.calculate_input_correlations = True

        project.run();

        sens = project.stochast;

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertEqual(5 , len(project.output_correlation_matrix.variables))

        self.assertAlmostEqual(0.96, project.output_correlation_matrix['x','y'], delta=margin)
        self.assertAlmostEqual(0.0, project.output_correlation_matrix['a','b'], delta=margin)
        self.assertAlmostEqual(-0.21, project.output_correlation_matrix['a','x'], delta=margin)

    def test_crude_monte_carlo_multiple_unbalanced_correlated_linear(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.calculate_correlations = True
        project.settings.calculate_input_correlations = True

        project.correlation_matrix['a', 'b'] = 0.9

        project.run();

        sens = project.stochast;

        self.assertEqual('x', sens.name)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(1.25, sens.deviation, delta=margin)

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertEqual(5 , len(project.output_correlation_matrix.variables))

        self.assertAlmostEqual(0.99, project.output_correlation_matrix['x','y'], delta=margin)
        self.assertAlmostEqual(0.90, project.output_correlation_matrix['a','b'], delta=margin)
        self.assertAlmostEqual(-0.92, project.output_correlation_matrix['a','x'], delta=margin)

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

    def test_numerical_integration_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.numerical_integration

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.histogram, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.81, sens.deviation, delta=margin)

    def test_fosm_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.fosm

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.normal, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(1.04, sens.deviation, delta=margin)

    def test_form_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.form

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.92, sens.deviation, delta=margin)

    def test_directional_sampling_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.directional_sampling
        project.settings.quantiles.append(0.9)

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertEqual(1, len(sens.fragility_values))
        self.assertAlmostEqual(2.89, sens.fragility_values[0].x, delta=margin)
        self.assertAlmostEqual(0.9, sens.fragility_values[0].probability_of_non_failure, delta=margin)

    def test_directional_sampling_linear_parallel(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.directional_sampling
        project.settings.quantiles.append(0.9)
        project.settings.max_parallel_processes = 4

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertEqual(1, len(sens.fragility_values))
        self.assertAlmostEqual(2.89, sens.fragility_values[0].x, delta=margin)
        self.assertAlmostEqual(0.9, sens.fragility_values[0].probability_of_non_failure, delta=margin)

    def test_mc_pile(self):
        project = project_builder.get_sensitivity_pile_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.run();
        mc = project.stochast;

        self.assertAlmostEqual(0.85, mc.get_quantile(StandardNormal.get_p_from_u(1.8)), delta=margin)
        self.assertAlmostEqual(1.09, mc.get_quantile(StandardNormal.get_p_from_u(2.5)), delta=margin)
        self.assertAlmostEqual(1.69, mc.get_quantile(StandardNormal.get_p_from_u(3.3)), delta=margin)

    def test_is_pile(self):
        project = project_builder.get_sensitivity_pile_project()

        project.settings.sensitivity_method = SensitivityMethod.importance_sampling
        project.settings.maximum_samples = 1000
        project.settings.variance_factor = 1
        project.settings.stochast_settings['D'].start_value = -0.5
        project.settings.stochast_settings['Load'].start_value = 0.5
        project.run();
        mc = project.stochast;

        self.assertAlmostEqual(0.85, mc.get_quantile(StandardNormal.get_p_from_u(1.8)), delta=margin)
        self.assertAlmostEqual(1.09, mc.get_quantile(StandardNormal.get_p_from_u(2.5)), delta=margin)
        self.assertAlmostEqual(1.53, mc.get_quantile(StandardNormal.get_p_from_u(3.3)), delta=margin)

    def test_form_pile(self):
        project = project_builder.get_sensitivity_pile_project()

        project.settings.sensitivity_method = SensitivityMethod.form
        project.run();
        mc = project.stochast;

        self.assertAlmostEqual(0.85, mc.get_quantile(StandardNormal.get_p_from_u(1.8)), delta=margin)
        self.assertAlmostEqual(1.09, mc.get_quantile(StandardNormal.get_p_from_u(2.5)), delta=margin)
        self.assertAlmostEqual(1.57, mc.get_quantile(StandardNormal.get_p_from_u(3.3)), delta=margin)


    def test_directional_sampling_pile(self):
        project = project_builder.get_sensitivity_pile_project()

        project.settings.sensitivity_method = SensitivityMethod.directional_sampling
        project.settings.maximum_samples = 10000
        project.settings.variation_coefficient = 0.01
        project.settings.quantiles.append(StandardNormal.get_p_from_u(1.8))
        project.settings.quantiles.append(StandardNormal.get_p_from_u(2.5))
        project.settings.quantiles.append(StandardNormal.get_p_from_u(3.3))

        self.assertAlmostEqual(84276, project.variables['Load'].mean, delta=margin*100)

        project.run();

        sens = project.stochast;

        self.assertEqual('UC' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertEqual(3, len(sens.fragility_values))
        self.assertAlmostEqual(0.85, sens.fragility_values[0].x, delta=margin)
        self.assertAlmostEqual(1.08, sens.fragility_values[1].x, delta=margin)
        self.assertAlmostEqual(0.96, sens.fragility_values[2].x, delta=margin)

        project.settings.quantiles.append(StandardNormal.get_p_from_u(2.8))

        project.run();

        sens2 = project.stochast;

        self.assertEqual('UC' , sens2.name)
        self.assertEqual(4, len(sens2.fragility_values))

        self.assertAlmostEqual(0.85, sens2.fragility_values[0].x, delta=margin)
        self.assertAlmostEqual(1.08, sens2.fragility_values[1].x, delta=margin)
        self.assertAlmostEqual(1.19, sens2.fragility_values[2].x, delta=margin)
        self.assertAlmostEqual(0.96, sens2.fragility_values[3].x, delta=margin)

    def test_sensitivity_print(self):

        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.quantiles.append(0.5)
        project.settings.quantiles.append(0.95)

        project.run();

        sens = project.result;

        sens.print()

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()
        sens.print()
        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        expected = """Variable L - (a+b):
  distribution = histogram
Definition:
  amount[-0.1971, -0.1496] = 1.0
  amount[-0.1496, -0.102] = 8.0
  amount[-0.102, -0.0545] = 12.0
  amount[-0.0545, -0.0069] = 12.0
  amount[-0.0069, 0.0406] = 11.0
  amount[0.0406, 0.0882] = 27.0
  amount[0.0882, 0.1358] = 33.0
  amount[0.1358, 0.1833] = 33.0
  amount[0.1833, 0.2309] = 35.0
  amount[0.2309, 0.2784] = 33.0
  amount[0.2784, 0.326] = 52.0
  amount[0.326, 0.3735] = 66.0
  amount[0.3735, 0.4211] = 61.0
  amount[0.4211, 0.4686] = 54.0
  amount[0.4686, 0.5162] = 66.0
  amount[0.5162, 0.5637] = 65.0
  amount[0.5637, 0.6113] = 77.0
  amount[0.6113, 0.6588] = 76.0
  amount[0.6588, 0.7064] = 84.0
  amount[0.7064, 0.754] = 104.0
  amount[0.754, 0.8015] = 84.0
  amount[0.8015, 0.8491] = 85.0
  amount[0.8491, 0.8966] = 95.0
  amount[0.8966, 0.9442] = 118.0
  amount[0.9442, 0.9917] = 124.0
  amount[0.9917, 1.0393] = 105.0
  amount[1.0393, 1.0868] = 104.0
  amount[1.0868, 1.1344] = 128.0
  amount[1.1344, 1.1819] = 129.0
  amount[1.1819, 1.2295] = 127.0
  amount[1.2295, 1.277] = 138.0
  amount[1.277, 1.3246] = 136.0
  amount[1.3246, 1.3721] = 143.0
  amount[1.3721, 1.4197] = 146.0
  amount[1.4197, 1.4673] = 153.0
  amount[1.4673, 1.5148] = 146.0
  amount[1.5148, 1.5624] = 143.0
  amount[1.5624, 1.6099] = 171.0
  amount[1.6099, 1.6575] = 139.0
  amount[1.6575, 1.705] = 172.0
  amount[1.705, 1.7526] = 185.0
  amount[1.7526, 1.8001] = 163.0
  amount[1.8001, 1.8477] = 178.0
  amount[1.8477, 1.8952] = 161.0
  amount[1.8952, 1.9428] = 167.0
  amount[1.9428, 1.9903] = 169.0
  amount[1.9903, 2.0379] = 150.0
  amount[2.0379, 2.0855] = 153.0
  amount[2.0855, 2.133] = 159.0
  amount[2.133, 2.1806] = 137.0
  amount[2.1806, 2.2281] = 138.0
  amount[2.2281, 2.2757] = 140.0
  amount[2.2757, 2.3232] = 138.0
  amount[2.3232, 2.3708] = 127.0
  amount[2.3708, 2.4183] = 130.0
  amount[2.4183, 2.4659] = 99.0
  amount[2.4659, 2.5134] = 117.0
  amount[2.5134, 2.561] = 119.0
  amount[2.561, 2.6085] = 105.0
  amount[2.6085, 2.6561] = 89.0
  amount[2.6561, 2.7037] = 102.0
  amount[2.7037, 2.7512] = 90.0
  amount[2.7512, 2.7988] = 105.0
  amount[2.7988, 2.8463] = 100.0
  amount[2.8463, 2.8939] = 82.0
  amount[2.8939, 2.9414] = 79.0
  amount[2.9414, 2.989] = 88.0
  amount[2.989, 3.0365] = 68.0
  amount[3.0365, 3.0841] = 52.0
  amount[3.0841, 3.1316] = 66.0
  amount[3.1316, 3.1792] = 63.0
  amount[3.1792, 3.2267] = 68.0
  amount[3.2267, 3.2743] = 53.0
  amount[3.2743, 3.3218] = 54.0
  amount[3.3218, 3.3694] = 39.0
  amount[3.3694, 3.417] = 32.0
  amount[3.417, 3.4645] = 29.0
  amount[3.4645, 3.5121] = 23.0
  amount[3.5121, 3.5596] = 25.0
  amount[3.5596, 3.6072] = 21.0
  amount[3.6072, 3.6547] = 15.0
  amount[3.6547, 3.7023] = 15.0
  amount[3.7023, 3.7498] = 9.0
  amount[3.7498, 3.7974] = 2.0
Derived values:
  mean = 1.792
  deviation = 0.8254
  variation = 0.4606
Quantiles:
  quantile 0.5: [-0.2641, 0.2754] -> [1.7887] -> 1.7887
  quantile 0.95: [-0.4336, -0.9487] -> [3.1823] -> 3.1823
"""
        self.assertEqual(expected, printed)



if __name__ == '__main__':
    unittest.main()
