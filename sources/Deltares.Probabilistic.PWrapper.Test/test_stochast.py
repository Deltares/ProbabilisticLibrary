from io import DEFAULT_BUFFER_SIZE
import unittest
import sys

from reliability import *
from stochast import *
from project import *

import project_builder

margin = 0.01

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

        self.assertAlmostEqual(5.0, m, delta=margin)
        self.assertAlmostEqual(6.0, x, delta=margin)

if __name__ == '__main__':
    unittest.main()
