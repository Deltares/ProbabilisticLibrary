from io import DEFAULT_BUFFER_SIZE
import unittest
import sys

from model import Stochast
from model import Settings
from model import Project

import project_builder

class Test_model(unittest.TestCase):
    def test_u_to_x(self):
        try:
            stochast = Stochast()
            stochast.distribution = "normal"
            stochast.mean = 5
            stochast.deviation = 1;

            m = stochast.location;
            x = stochast.get_x_from_u(1.0)
        except:
            message = sys.exc_info()[0]
            print('error: ' + message, flush = True)
            raise

        self.assertAlmostEqual(5.0, m, 0.001)
        self.assertAlmostEqual(6.0, x, 0.001)

    def test_form_linear(self):
        try:
            project = project_builder.get_linear_project()

            project.settings.reliability_method = 'form'

            project.run();

            dp = project.design_point;

            beta = dp.reliability_index;
        except:
            message = sys.exc_info()[0]
            print('error: ' + message, flush = True)
            raise

        self.assertAlmostEqual(2.33, beta, 0.01)

if __name__ == '__main__':
    unittest.main()
