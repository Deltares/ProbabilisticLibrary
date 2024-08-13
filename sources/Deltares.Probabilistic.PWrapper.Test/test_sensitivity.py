import unittest
import sys

from ptk import *

import project_builder

margin = 0.01

class Test_sensitivity(unittest.TestCase):

    def test_form_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = 'crude_monte_carlo'

        project.run();

        sens = project.stochast;

        self.assertAlmostEqual(1.8, sens.mean, delta=margin)
        self.assertAlmostEqual(0.83, sens.deviation, delta=margin)

if __name__ == '__main__':
    unittest.main()
