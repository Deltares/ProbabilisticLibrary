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
import os
import platform
from io import StringIO

from probabilistic_library.utils import FrozenList, FrozenObject, CallbackList
from probabilistic_library.uncertainty import UncertaintyResult, UncertaintyMethod
from probabilistic_library.project import UncertaintyProject
from probabilistic_library.statistic import Stochast, DistributionType, StandardNormal

import project_builder

margin = 0.02

class Test_uncertainty(unittest.TestCase):

    def test_invalid_project(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        self.assertTrue(project.is_valid())

        # change an unused property to an invalid value
        project.variables['a'].scale = -1
        self.assertTrue(project.is_valid())

        # now use the property, reassign the invalid property, because by changing the scale is reset
        project.variables['a'].distribution = DistributionType.normal
        project.variables['a'].scale = -1
        self.assertFalse(project.is_valid())

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()

        # run an invalid project
        project.run();
        self.assertIsNone(project.result)

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        if platform.system() == 'Windows':
            self.assertEqual("""Error: abc => scale value -1.5 is less than 0.\n""", printed)
        else:
            self.assertEqual("""Error: abc => scale value -1.500000 is less than 0.000000.\n""", printed)

    def test_crude_monte_carlo_add_one(self):
        project = project_builder.get_uncertainty_add_one_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1, sens.mean, delta=margin)

        sens.distribution = DistributionType.uniform

        self.assertAlmostEqual(0, sens.minimum, delta=margin)
        self.assertAlmostEqual(2, sens.maximum, delta=margin)


    def test_crude_monte_carlo_linear(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

    def test_crude_monte_carlo_plot(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        project.settings.quantiles.append(0.5)
        project.settings.quantiles.append(0.95)

        project.run();

        sens = project.result;

        self.assertTrue(sens.variable.is_valid())

        test_file_name = 'mc.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        plot = sens.get_plot()

        self.assertNotEqual(plot, None)

        plot.savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)

    def test_crude_monte_carlo_array_result_linear(self):

        project = project_builder.get_uncertainty_linear_array_result_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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

        project = project_builder.get_uncertainty_linear_delayed_array_result_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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

        project = project_builder.get_uncertainty_linear_delayed_array_result_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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
        project = project_builder.get_uncertainty_multiple_identical_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        project.settings.calculate_correlations = True

        project.run();

        sens = project.stochast;

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertAlmostEqual(1.0, project.output_correlation_matrix['x','y'], delta=margin)

    def test_crude_monte_carlo_multiple_unbalanced_linear(self):
        project = project_builder.get_uncertainty_multiple_unbalanced_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        project.settings.calculate_correlations = True

        project.run();

        sens = project.stochast;

        self.assertEqual(3, len(project.stochasts))
        self.assertEqual('x' , project.stochasts[0].name)
        self.assertEqual('y' , project.stochasts[1].name)

        self.assertAlmostEqual(0.96, project.output_correlation_matrix['x','y'], delta=margin)

    def test_crude_monte_carlo_multiple_unbalanced_linear_input(self):
        project = project_builder.get_uncertainty_multiple_unbalanced_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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
        project = project_builder.get_uncertainty_multiple_unbalanced_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        project.settings.derive_samples_from_variation_coefficient = True

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

    def test_importance_sampling_add_one(self):
        project = project_builder.get_uncertainty_add_one_project()

        project.settings.uncertainty_method = UncertaintyMethod.importance_sampling

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1, sens.mean, delta=margin)

        sens.distribution = DistributionType.uniform

        self.assertAlmostEqual(0, sens.minimum, delta=margin)
        self.assertAlmostEqual(2, sens.maximum, delta=margin)

    def test_numerical_integration_linear(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.numerical_integration

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.histogram, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.81, sens.deviation, delta=margin)

    def test_fosm_linear(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.fosm

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.normal, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(1.04, sens.deviation, delta=margin)

    def test_form_linear(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.form

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.92, sens.deviation, delta=margin)

    def test_directional_sampling_linear(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.directional_sampling
        project.settings.quantiles.append(0.9)

        project.run();

        sens = project.stochast;

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertEqual(DistributionType.cdf_curve, sens.distribution)
        self.assertEqual(1, len(sens.fragility_values))
        self.assertAlmostEqual(2.89, sens.fragility_values[0].x, delta=margin)
        self.assertAlmostEqual(0.9, sens.fragility_values[0].probability_of_non_failure, delta=margin)

    def test_directional_sampling_linear_parallel(self):
        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.directional_sampling
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
        project = project_builder.get_uncertainty_pile_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
        project.run();
        mc = project.stochast;

        self.assertAlmostEqual(0.85, mc.get_quantile(StandardNormal.get_p_from_u(1.8)), delta=margin)
        self.assertAlmostEqual(1.09, mc.get_quantile(StandardNormal.get_p_from_u(2.5)), delta=margin)
        self.assertAlmostEqual(1.69, mc.get_quantile(StandardNormal.get_p_from_u(3.3)), delta=margin)

    def test_is_pile(self):
        project = project_builder.get_uncertainty_pile_project()

        project.settings.uncertainty_method = UncertaintyMethod.importance_sampling
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
        project = project_builder.get_uncertainty_pile_project()

        project.settings.uncertainty_method = UncertaintyMethod.form
        project.run();
        mc = project.stochast;

        self.assertAlmostEqual(0.85, mc.get_quantile(StandardNormal.get_p_from_u(1.8)), delta=margin)
        self.assertAlmostEqual(1.09, mc.get_quantile(StandardNormal.get_p_from_u(2.5)), delta=margin)
        self.assertAlmostEqual(1.57, mc.get_quantile(StandardNormal.get_p_from_u(3.3)), delta=margin)


    def test_directional_sampling_pile(self):
        project = project_builder.get_uncertainty_pile_project()

        project.settings.uncertainty_method = UncertaintyMethod.directional_sampling
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

    def test_uncertainty_print(self):

        project = project_builder.get_uncertainty_linear_project()

        project.settings.uncertainty_method = UncertaintyMethod.crude_monte_carlo
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
  amount[-0.1971, -0.1496] = 1
  amount[-0.1496, -0.102] = 8
  amount[-0.102, -0.05446] = 12
  amount[-0.05446, -0.006907] = 12
  amount[-0.006907, 0.04065] = 11
  amount[0.04065, 0.0882] = 27
  amount[0.0882, 0.1358] = 33
  amount[0.1358, 0.1833] = 33
  amount[0.1833, 0.2309] = 35
  amount[0.2309, 0.2784] = 33
  amount[0.2784, 0.326] = 52
  amount[0.326, 0.3735] = 66
  amount[0.3735, 0.4211] = 61
  amount[0.4211, 0.4686] = 54
  amount[0.4686, 0.5162] = 66
  amount[0.5162, 0.5637] = 65
  amount[0.5637, 0.6113] = 77
  amount[0.6113, 0.6588] = 76
  amount[0.6588, 0.7064] = 84
  amount[0.7064, 0.754] = 104
  amount[0.754, 0.8015] = 84
  amount[0.8015, 0.8491] = 85
  amount[0.8491, 0.8966] = 95
  amount[0.8966, 0.9442] = 118
  amount[0.9442, 0.9917] = 124
  amount[0.9917, 1.039] = 105
  amount[1.039, 1.087] = 104
  amount[1.087, 1.134] = 128
  amount[1.134, 1.182] = 129
  amount[1.182, 1.229] = 127
  amount[1.229, 1.277] = 138
  amount[1.277, 1.325] = 136
  amount[1.325, 1.372] = 143
  amount[1.372, 1.42] = 146
  amount[1.42, 1.467] = 153
  amount[1.467, 1.515] = 146
  amount[1.515, 1.562] = 143
  amount[1.562, 1.61] = 171
  amount[1.61, 1.657] = 139
  amount[1.657, 1.705] = 172
  amount[1.705, 1.753] = 185
  amount[1.753, 1.8] = 163
  amount[1.8, 1.848] = 178
  amount[1.848, 1.895] = 161
  amount[1.895, 1.943] = 167
  amount[1.943, 1.99] = 169
  amount[1.99, 2.038] = 150
  amount[2.038, 2.085] = 153
  amount[2.085, 2.133] = 159
  amount[2.133, 2.181] = 137
  amount[2.181, 2.228] = 138
  amount[2.228, 2.276] = 140
  amount[2.276, 2.323] = 138
  amount[2.323, 2.371] = 127
  amount[2.371, 2.418] = 130
  amount[2.418, 2.466] = 99
  amount[2.466, 2.513] = 117
  amount[2.513, 2.561] = 119
  amount[2.561, 2.609] = 105
  amount[2.609, 2.656] = 89
  amount[2.656, 2.704] = 102
  amount[2.704, 2.751] = 90
  amount[2.751, 2.799] = 105
  amount[2.799, 2.846] = 100
  amount[2.846, 2.894] = 82
  amount[2.894, 2.941] = 79
  amount[2.941, 2.989] = 88
  amount[2.989, 3.037] = 68
  amount[3.037, 3.084] = 52
  amount[3.084, 3.132] = 66
  amount[3.132, 3.179] = 63
  amount[3.179, 3.227] = 68
  amount[3.227, 3.274] = 53
  amount[3.274, 3.322] = 54
  amount[3.322, 3.369] = 39
  amount[3.369, 3.417] = 32
  amount[3.417, 3.465] = 29
  amount[3.465, 3.512] = 23
  amount[3.512, 3.56] = 25
  amount[3.56, 3.607] = 21
  amount[3.607, 3.655] = 15
  amount[3.655, 3.702] = 15
  amount[3.702, 3.75] = 9
  amount[3.75, 3.797] = 2
Derived values:
  mean = 1.792
  deviation = 0.8254
  variation = 0.4606
Quantiles:
  quantile 0.5: [-0.2641, 0.2754] -> [1.789] -> 1.789
  quantile 0.95: [-0.4336, -0.9487] -> [3.182] -> 3.182
"""
        self.assertEqual(expected, printed)



if __name__ == '__main__':
    unittest.main()
