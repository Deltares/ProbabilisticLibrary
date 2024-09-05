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

    def test_correlation_matrix(self):
        project = project_builder.get_sensitivity_linear_project()
        project.model = project_builder.linear_ab;

        project.settings.sensitivity_method = SensitivityMethod.crude_monte_carlo

        project.run();
        sens1 = project.stochast;

        project.model = project_builder.unbalanced_linear_ab;

        project.run();
        sens2 = project.stochast;

        #correlation_matrix = project.correlation_matrix;
        #self.assertAlmostEqual(0.84, project.correlation_matrix['linear', 'unbalanced_linear'])


if __name__ == '__main__':
    unittest.main()
