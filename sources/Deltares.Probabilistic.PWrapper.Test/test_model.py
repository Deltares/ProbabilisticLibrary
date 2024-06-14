import unittest
from model import Stochast

class Test_model(unittest.TestCase):
    def test_u_to_x(self):
        try:
            stochast = Stochast()
            stochast.distribution = "Normal"
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

if __name__ == '__main__':
    unittest.main()
