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
from tkinter import PROJECTING
import unittest
import sys

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

        self.assertEqual(3 * project.settings.maximum_samples, project.total_model_runs)

    def test_crude_monte_carlo_array_result_linear_delayed(self):

        project = project_builder.get_sensitivity_linear_delayed_array_result_project()

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo
        project.settings.variation_coefficient = 0
        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 1000

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


if __name__ == '__main__':
    unittest.main()
