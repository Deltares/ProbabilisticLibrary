from io import DEFAULT_BUFFER_SIZE
import unittest
import sys

from ptk import *

import project_builder

margin = 0.01

class Test_reliability(unittest.TestCase):

    def test_form_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = 'form'

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
        self.assertEqual(project.variables[0], project.design_point.alphas[0].variable)


    def test_form_linear_fully_correlated(self):
        project = project_builder.get_linear_fully_correlated_project()

        project.settings.reliability_method = 'form'

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

        project.settings.reliability_method = 'form'

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

        project.settings.reliability_method = 'crude_monte_carlo'
        project.settings.random_type = 'mersenne_twister'

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.61, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.69, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.73, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_directional_sampling_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = 'directional_sampling'
        project.settings.random_type = 'mersenne_twister'

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

        project.settings.reliability_method = 'importance_sampling'
        project.settings.random_type = 'mersenne_twister'

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.62, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.7, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_importance_sampling_linear_small(self):
        project = project_builder.get_linear_small_project()

        project.settings.reliability_method = 'importance_sampling'
        project.settings.random_type = 'mersenne_twister'
        project.settings.minimum_samples = 5000
        project.settings.maximum_samples = 10000

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.97, beta, delta=margin)

    def test_numerical_integration_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = 'numerical_integration'

        for variable in project.variables:
            stochastSettings = StochastSettings();
            stochastSettings.variable = variable
            stochastSettings.start_value = 0
            stochastSettings.variance_factor = 2
            project.settings.stochast_settings.append(stochastSettings)

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

        project.settings.reliability_method = 'numerical_integration'

        for variable in project.variables:
            stochastSettings = StochastSettings();
            stochastSettings.variable = variable
            stochastSettings.min_value = -1
            project.settings.stochast_settings.append(stochastSettings)

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(2, len(alphas))

if __name__ == '__main__':
    unittest.main()
