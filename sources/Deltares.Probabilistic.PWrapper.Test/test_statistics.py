# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of Streams.
#
# Streams is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
import unittest
import sys

from streams import *

margin = 0.01

class Test_statistics(unittest.TestCase):
    def test_u_to_x(self):
        try:
            stochast = Stochast()
            stochast.distribution = DistributionType.normal
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
        stochast.distribution = DistributionType.discrete

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
        stochast1.distribution = DistributionType.normal
        stochasts.append(stochast1)

        stochast2 = Stochast()
        stochast2.distribution = DistributionType.normal
        stochasts.append(stochast2)

        stochast3 = Stochast()
        stochast3.distribution = DistributionType.normal
        stochasts.append(stochast3)

        stochast4 = Stochast()
        stochast4.distribution = DistributionType.normal
        # do not add to stochasts

        correlation_matrix = CorrelationMatrix();

        correlation_matrix._set_variables(stochasts)

        correlation_matrix[(stochast1, stochast2)] = 0.8
        correlation_matrix[(stochast2, stochast3)] = 1.0
        correlation_matrix[(stochast3, stochast4)] = 0.2

        # registered stochasts
        self.assertAlmostEqual(0.8, correlation_matrix[(stochast1, stochast2)], delta=margin)
        self.assertAlmostEqual(0.8, correlation_matrix[(stochast2, stochast1)], delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix[(stochast2, stochast3)], delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix[(stochast3, stochast2)], delta=margin)

        # unregistered stochasts
        self.assertAlmostEqual(0.0, correlation_matrix[(stochast3, stochast4)], delta=margin)
        self.assertAlmostEqual(0.0, correlation_matrix[(stochast4, stochast3)], delta=margin)

        # default values
        self.assertAlmostEqual(0.0, correlation_matrix[(stochast1, stochast3)], delta=margin)
        self.assertAlmostEqual(1.0, correlation_matrix[(stochast2, stochast2)], delta=margin)

    def test_correlation_by_name(self):
        stochasts = []

        stochast1 = Stochast()
        stochast1.name = 'a'
        stochast1.distribution = DistributionType.normal
        stochasts.append(stochast1)

        stochast2 = Stochast()
        stochast2.name = 'b'
        stochast2.distribution = DistributionType.exponential
        stochasts.append(stochast2)

        correlation_matrix = CorrelationMatrix();

        correlation_matrix._set_variables(stochasts)

        correlation_matrix[('a', 'b')] = 0.8

        # registered stochasts
        self.assertAlmostEqual(0.8, correlation_matrix[(stochast1, stochast2)], delta=margin)
        self.assertAlmostEqual(0.8, correlation_matrix[('a', 'b')], delta=margin)

    def test_fit(self):
        stochast = Stochast()
        stochast.distribution = "normal"

        values = []
        values.extend({4.1, 4.2, 4.4, 4.5})

        stochast.fit(values)

        self.assertAlmostEqual(4.3, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.18, stochast.deviation, delta=margin)

    def test_ks_test(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 4.2
        stochast.deviation = 0.2

        values = []
        values.extend({4.1, 4.2, 4.4, 4.5})

        ks = stochast.get_ks_test(values)

        self.assertAlmostEqual(0.34, ks, delta=margin)

    def test_histogram(self):
        stochasts = []

        stochast = Stochast()
        stochast.distribution = DistributionType.histogram
        
        stochast.histogram_values.append(HistogramValue.create(1, 3, 2))
        stochast.histogram_values.append(HistogramValue.create(5, 7, 6))
        stochast.histogram_values.append(HistogramValue.create(7, 9, 2))

        u = StandardNormal.get_u_from_p(0.1)
        self.assertAlmostEqual(2, stochast.get_x_from_u(u), delta=margin)

        stochast.histogram_values[0].lower_bound = 0
        stochast.histogram_values[0].upper_bound = 2

        self.assertAlmostEqual(1, stochast.get_x_from_u(u), delta=margin)

        values = []
        values.extend({2, 3, 5})

        stochast.fit(values)
        self.assertEqual(3, len(stochast.histogram_values))

        # registered stochasts
        self.assertAlmostEqual(1.5, stochast.histogram_values[0].lower_bound, delta=margin)

    def test_composite(self):
        stochasts = []

        stochast1 = Stochast()
        stochast1.distribution = DistributionType.uniform
        stochast1.minimium = 0
        stochast1.maximum = 8

        stochast2 = Stochast()
        stochast2.distribution = DistributionType.uniform
        stochast2.minimum = 6
        stochast2.maximum = 10

        stochast = Stochast()
        stochast.distribution = DistributionType.composite
        stochast.contributing_stochasts.append(ContributingStochast.create(0.4, stochast1))
        stochast.contributing_stochasts.append(ContributingStochast.create(0.6, stochast2))

        self.assertAlmostEqual(StandardNormal.get_u_from_p(0.4 * 0.125 + 0.6 * 0), stochast.get_u_from_x(1.0), delta=margin)
        self.assertAlmostEqual(StandardNormal.get_u_from_p(0.4 * 0.875 + 0.6 * 0.25), stochast.get_u_from_x(7.0), delta=margin)
        self.assertAlmostEqual(StandardNormal.get_u_from_p(0.4 * 1 + 0.6 * 0.75), stochast.get_u_from_x(9.0), delta=margin)

    def test_conditional(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 0
        stochast.deviation = 1

        self.assertAlmostEqual(1.0, stochast.get_x_from_u(1.0), delta=margin)
        
        stochast.conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = 0
        conditional1.location = 0
        conditional1.scale = 1
        stochast.conditional_values.append(conditional1)

        conditional2 = ConditionalValue()
        conditional2.x = 1
        conditional2.location = 1
        conditional2.scale = 2
        stochast.conditional_values.append(conditional2)

        self.assertAlmostEqual(2.0, stochast.get_x_from_u_and_source(1.0, 0.5), delta=margin)

    def test_conditional_from_mean_values(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 0
        stochast.deviation = 1

        self.assertAlmostEqual(1.0, stochast.get_x_from_u(1.0), delta=margin)
        
        stochast.conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = 0
        conditional1.mean = 0
        conditional1.deviation = 1
        stochast.conditional_values.append(conditional1)

        conditional2 = ConditionalValue()
        conditional2.x = 1
        conditional2.mean = 1
        conditional2.deviation = 2
        stochast.conditional_values.append(conditional2)

        self.assertAlmostEqual(2.0, stochast.get_x_from_u_and_source(1.0, 0.5), delta=margin)

    def test_design_value(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 10
        stochast.deviation = 2

        self.assertAlmostEqual(10.0, stochast.design_value, delta=margin)

        stochast.design_factor = 2;
        stochast.design_quantile = StandardNormal.get_p_from_u(2);

        self.assertAlmostEqual((10 + 2*2) /2.0, stochast.design_value, delta=margin)

        stochast.design_value = (15 + 2 * 3) / 2.0

        self.assertAlmostEqual(15, stochast.mean, delta=margin)
        self.assertAlmostEqual(3, stochast.deviation, delta=margin)

    def test_variation_coefficient(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 10
        stochast.variation = 0.1

        self.assertAlmostEqual(1.0, stochast.deviation, delta=margin)

        # variation is kept constant
        stochast.mean = 20

        self.assertAlmostEqual(0.1, stochast.variation, delta=margin)
        self.assertAlmostEqual(2.0, stochast.deviation, delta=margin)

        # deviation is kept constant
        stochast.deviation = 4

        self.assertAlmostEqual(0.2, stochast.variation, delta=margin)
        self.assertAlmostEqual(4.0, stochast.deviation, delta=margin)

        stochast.mean = 10

        self.assertAlmostEqual(0.4, stochast.variation, delta=margin)
        self.assertAlmostEqual(4.0, stochast.deviation, delta=margin)

    def test_return_time(self):
        for i in range(6):
            beta = float(i)
            rt = StandardNormal.get_t_from_u(beta)
            betaCalculated = StandardNormal.get_u_from_t(rt)
            self.assertAlmostEqual(beta, betaCalculated, delta=margin)

if __name__ == '__main__':
    unittest.main()
