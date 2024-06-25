import unittest
import sys

from statistic import Stochast, DiscreteValue, CorrelationMatrix

margin = 0.01

class Test_statistics(unittest.TestCase):
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

    def test_discrete(self):
        stochast = Stochast()
        stochast.distribution = "discrete"

        dv1 = DiscreteValue()
        dv1.x = 2
        dv1.amount = 1
        stochast.discrete_values.append(dv1)

        dv2 = DiscreteValue()
        dv2.x = 3
        dv2.amount = 3
        stochast.discrete_values.append(dv2)

        self.assertAlmostEqual(2.75, stochast.mean, delta=margin)

        stochast.discrete_values.remove(dv1)

        self.assertAlmostEqual(3.0, stochast.mean, delta=margin)

        new_discrete_values = []
        new_discrete_values.append(dv1)

        stochast.discrete_values.extend(new_discrete_values)

        self.assertAlmostEqual(2.75, stochast.mean, delta=margin)

        stochast.discrete_values.clear()
        stochast.discrete_values.append(dv1)
        stochast.discrete_values[0] = dv2

        self.assertAlmostEqual(3.0, stochast.mean, delta=margin)
        
    def test_correlation(self):
        stochasts = []

        stochast1 = Stochast()
        stochast1.distribution = "normal"
        stochasts.append(stochast1)

        stochast2 = Stochast()
        stochast2.distribution = "normal"
        stochasts.append(stochast2)

        stochast3 = Stochast()
        stochast3.distribution = "normal"
        stochasts.append(stochast3)

        stochast4 = Stochast()
        stochast4.distribution = "normal"
        # do not add to stochasts

        correlation_matrix = CorrelationMatrix();

        correlation_matrix.variables.extend(stochasts)

        correlation_matrix.set_correlation(stochast1, stochast2, 0.8)
        correlation_matrix.set_correlation(stochast2, stochast3, 1.0)
        correlation_matrix.set_correlation(stochast3, stochast4, 0.2)

        # registered stochasts
        self.assertAlmostEqual(0.8, correlation_matrix.get_correlation(stochast1, stochast2), delta=margin)
        self.assertAlmostEqual(0.8, correlation_matrix.get_correlation(stochast2, stochast1), delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix.get_correlation(stochast2, stochast3), delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix.get_correlation(stochast3, stochast2), delta=margin)

        # unregistered stochasts
        self.assertAlmostEqual(0.0, correlation_matrix.get_correlation(stochast3, stochast4), delta=margin)
        self.assertAlmostEqual(0.0, correlation_matrix.get_correlation(stochast4, stochast3), delta=margin)

        # default values
        self.assertAlmostEqual(0.0, correlation_matrix.get_correlation(stochast1, stochast3), delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix.get_correlation(stochast2, stochast2), delta=margin)

    def test_fit(self):
        stochasts = []

        stochast = Stochast()
        stochast.distribution = "normal"

        values = []
        values.extend({4.1, 4.2, 4.4, 4.5})

        stochast.fit(values)

        # registered stochasts
        self.assertAlmostEqual(4.3, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.18, stochast.deviation, delta=margin)

        
if __name__ == '__main__':
    unittest.main()
