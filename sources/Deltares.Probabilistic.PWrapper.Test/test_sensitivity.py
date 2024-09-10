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

        self.assertEqual('L - (a+b)' , sens.name)
        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

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


if __name__ == '__main__':
    unittest.main()
