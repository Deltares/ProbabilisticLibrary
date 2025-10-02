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
import unittest
import sys
import numpy as np
from io import StringIO
import matplotlib.pyplot as plt

from probabilistic_library import *

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
        self.assertAlmostEqual(0.84, stochast.get_cdf(6), delta=margin)
        self.assertAlmostEqual(0.24, stochast.get_pdf(6), delta=margin)

    def test_log_normal(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.log_normal
        stochast.mean = 0.0
        stochast.deviation = 3.052E-002
        stochast.shift = -0.79
        x = stochast.get_x_from_u(0.1)

        self.assertAlmostEqual(0.002465603026085228, x, delta=margin)

    def test_log_normal_fit(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.log_normal

        values = [4.1, 4.2, 4.45, 4.75]
        stochast.fit(values)

        self.assertAlmostEqual(3.92, stochast.shift, delta=margin)
        self.assertAlmostEqual(4.41, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.37, stochast.deviation, delta=margin)

        prior = Stochast()
        prior.distribution = DistributionType.log_normal
        prior.mean = 3.5
        prior.deviation = 0.5

        stochast.shift = 0

        stochast.fit_prior(prior, values)

        self.assertAlmostEqual(4.32, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.14, stochast.deviation, delta=margin)

        negative_values = [-2.2, 1.4, 1.8]

        stochast.shift = 0
        stochast.fit_prior(prior, negative_values)

        self.assertAlmostEqual(3.33, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.48, stochast.deviation, delta=margin)
        self.assertAlmostEqual(-4.4, stochast.shift, delta=margin)

    def test_exponential(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.exponential

        stochast.mean = 2.0
        self.assertAlmostEqual(0.0, stochast.deviation, delta=margin)
        self.assertAlmostEqual(2.0, stochast.shift, delta=margin)

        stochast.deviation = 0.5
        self.assertAlmostEqual(0.5, stochast.deviation, delta=margin)
        self.assertAlmostEqual(1.5, stochast.shift, delta=margin)

        stochast.shift = -0.5
        self.assertAlmostEqual(2.0, stochast.mean, delta=margin)
        self.assertAlmostEqual(2.5, stochast.deviation, delta=margin)

    def test_exponential_fit(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.exponential

        stochast.fit([2.0, 3.0, 5.0, 9.0, 17.0])

        self.assertAlmostEqual(7.2, stochast.mean, delta=margin)
        self.assertAlmostEqual(8.2, stochast.deviation, delta=margin)
        self.assertAlmostEqual(-1.0, stochast.shift, delta=margin)

        prior = Stochast()
        prior.distribution = DistributionType.exponential
        prior.mean = 5.0
        prior.deviation = 2.5

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()

        stochast.fit_prior(prior, [3.5, 4.5, 8.0])

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("""Fit with prior is not supported for distribution type exponential\n""", printed)

    def test_plot(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 2
        stochast.deviation = 0.4

        test_file_name = 'normal.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        pl = stochast.get_plot()
        pl.savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)

    def test_invalid_plot(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 2
        stochast.deviation = -0.4

        pl = stochast.get_plot()

        self.assertEqual(pl, None)

    def test_series(self):
        stochast1 = Stochast()
        stochast1.name = 'stochast 1'
        stochast1.distribution = DistributionType.normal
        stochast1.mean = 2
        stochast1.deviation = 0.4

        self.assertEqual(2, len(stochast1.get_series(number_of_points=0)))
        self.assertEqual(2, len(stochast1.get_series(number_of_points=1)))
        self.assertEqual(2, len(stochast1.get_series(number_of_points=2)))
        self.assertEqual(3, len(stochast1.get_series(number_of_points=3)))

        stochast2 = Stochast()
        stochast2.name = 'stochast 2'
        stochast2.distribution = DistributionType.log_normal
        stochast2.mean = 2
        stochast2.deviation = 0.2
        stochast2.shift = 0.5

        special = stochast2.get_special_values();
        self.assertEqual(1, len(special))
        self.assertAlmostEqual(0.5, special[0], delta=margin)

        self.assertEqual(3, len(stochast2.get_series(number_of_points=0)))
        self.assertEqual(3, len(stochast2.get_series(number_of_points=1)))
        self.assertEqual(3, len(stochast2.get_series(number_of_points=2)))
        self.assertEqual(4, len(stochast2.get_series(number_of_points=3)))

        self.assertEqual(3, len(stochast2.get_series(xmin = 1, number_of_points=3)))

    def test_multiple_plot(self):
        stochast1 = Stochast()
        stochast1.name = 'stochast 1'
        stochast1.distribution = DistributionType.log_normal
        stochast1.mean = 2
        stochast1.deviation = 0.4

        stochast2 = Stochast()
        stochast2.name = 'stochast 2'
        stochast2.distribution = DistributionType.log_normal
        stochast2.mean = 2
        stochast2.deviation = 0.2
        stochast2.shift = 0.5

        stochast3 = Stochast()
        stochast3.name = 'uniform'
        stochast3.distribution = DistributionType.uniform
        stochast3.minimum = 1
        stochast3.maximum = 5

        values1 = stochast1.get_series()
        pdf1 = [stochast1.get_pdf(x) for x in values1]

        values2 = stochast2.get_series(0)
        pdf2 = [stochast2.get_pdf(x) for x in values2]

        values3 = stochast3.get_series(xmin = -1, xmax = 10, number_of_points=0)
        pdf3 = [stochast3.get_pdf(x) for x in values3]

        plt.close()
    
        _, ax1 = plt.subplots()
        ax1.set_xlabel("value [x]")
        ax1.set_ylabel("pdf [-]")

        ax1.plot(values1, pdf1, label = stochast1.name)
        ax1.plot(values2, pdf2, label = stochast2.name)
        ax1.plot(values3, pdf3, label = stochast3.name)

        plt.grid()
        plt.legend()

        # uncomment next line to display plot
        #plt.show()

        test_file_name = 'multiple-stochasts.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        plt.savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)

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

        # check mean value
        self.assertAlmostEqual(2.75, stochast.mean, delta=margin)

        # check pdf and cdf
        self.assertAlmostEqual(0.125, stochast.get_cdf(2.0), delta=margin)
        self.assertAlmostEqual(0.25, stochast.get_pdf(2.0), delta=margin)
        
        self.assertAlmostEqual(0.25, stochast.get_cdf(2.5), delta=margin)
        self.assertAlmostEqual(0.0, stochast.get_pdf(2.5), delta=margin)

        # check special values
        special_values = stochast.get_special_values()
        self.assertEqual(8, len(special_values))
        self.assertAlmostEqual(dv1.x, special_values[1], delta=margin)
        self.assertAlmostEqual(dv2.x, special_values[6], delta=margin)

        # check updates to the list of discrete values
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

    def test_normal_fit(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.normal

        values = []
        values.extend({4.1, 4.2, 4.4, 4.5})

        stochast.fit(values)

        self.assertAlmostEqual(4.3, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.18, stochast.deviation, delta=margin)

        prior = Stochast()
        prior.distribution = DistributionType.normal
        prior.mean = 3.5
        prior.deviation = 0.2

        stochast.fit_prior(prior, values)

        self.assertAlmostEqual(4.16, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.08, stochast.deviation, delta=margin)

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()

        prior.distribution = DistributionType.log_normal
        stochast.fit_prior(prior, values)

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("Fit from prior with another distribution type is not supported\n", printed)

    def test_bernoulli(self):
        stochast = Stochast()
        stochast.distribution = DistributionType.bernoulli

        values = []
        values.append(0)
        values.append(1)
        values.append(0)
        values.append(0)
        values.append(0)

        stochast.fit(values)

        self.assertAlmostEqual(0.2, stochast.mean, delta=margin)
        self.assertAlmostEqual(0.4, stochast.deviation, delta=margin)

        prior = Stochast()
        prior.distribution = DistributionType.bernoulli
        prior.mean = 0.8
        prior.observations = 2

        stochast.fit_prior(prior, values)

        self.assertAlmostEqual(0.37, stochast.mean, delta=margin)

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

    def test_changing_distribution(self):

        stochast1 = Stochast()
        stochast1.distribution = DistributionType.normal
        self.assertAlmostEqual(0, stochast1.mean, delta=margin)
        self.assertAlmostEqual(0, stochast1.deviation, delta=margin)

        stochast1.mean = 5
        stochast1.deviation = 1
        stochast1.truncated = True

        self.assertAlmostEqual(5, stochast1.mean, delta=margin)
        self.assertAlmostEqual(1, stochast1.deviation, delta=margin)

        self.assertLess(stochast1.minimum, -1000)
        self.assertGreater(stochast1.maximum, 1000)

        stochast1.minimum = 4
        self.assertAlmostEqual(5, stochast1.mean, delta=margin)

        stochast1.distribution = DistributionType.uniform
        self.assertAlmostEqual(5, stochast1.mean, delta=margin)
        self.assertAlmostEqual(1, stochast1.deviation, delta=margin)
        self.assertAlmostEqual(3.26, stochast1.minimum, delta=margin)
        self.assertAlmostEqual(6.73, stochast1.maximum, delta=margin)

        stochast2 = Stochast()
        stochast2.distribution = DistributionType.uniform

        self.assertTrue(np.isnan(stochast2.mean))
        self.assertAlmostEqual(np.inf, stochast2.deviation, delta=margin)
        self.assertAlmostEqual(-np.inf, stochast2.minimum, delta=margin)
        self.assertAlmostEqual(np.inf, stochast2.maximum, delta=margin)

        stochast2.minimum = 0
        stochast2.maximum = 8

        self.assertAlmostEqual(4, stochast2.mean, delta=margin)

        stochast2.distribution = DistributionType.normal
        self.assertAlmostEqual(4, stochast2.mean, delta=margin)

        stochast2.truncated = True
        self.assertAlmostEqual(4, stochast2.mean, delta=margin)

    def test_composite(self):
        stochasts = []

        stochast1 = Stochast()
        stochast1.distribution = DistributionType.uniform
        stochast1.minimum = 0
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

    def test_conditional_truncated(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 0
        stochast.deviation = 1
        stochast.truncated = True

        stochast.conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = 0
        conditional1.location = 0
        conditional1.scale = 1
        conditional1.minimum = 0
        conditional1.maximum = np.inf
        stochast.conditional_values.append(conditional1)

        conditional2 = ConditionalValue()
        conditional2.x = 1
        conditional2.location = 1
        conditional2.scale = 2
        conditional2.minimum = 0
        conditional2.maximum = np.inf
        stochast.conditional_values.append(conditional2)

        self.assertGreater(stochast.get_x_from_u_and_source(-5.0, 0.5), 0.0)

    def test_composite_conditional(self):
        stochast1 = Stochast()
        stochast1.distribution = DistributionType.deterministic
        stochast1.location = 0

        stochast2 = Stochast()
        stochast2.distribution = DistributionType.normal

        stochast2.conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = 0
        conditional1.location = 0
        conditional1.scale = 0
        stochast2.conditional_values.append(conditional1)

        conditional2 = ConditionalValue()
        conditional2.x = 10
        conditional2.location = 10
        conditional2.scale = 1
        stochast2.conditional_values.append(conditional2)

        conditional3 = ConditionalValue()
        conditional3.x = 20
        conditional3.location = 20
        conditional3.scale = 0
        stochast2.conditional_values.append(conditional3)

        stochast = Stochast()
        stochast.distribution = DistributionType.composite
        stochast.contributing_stochasts.append(ContributingStochast.create(0.4, stochast1))
        stochast.contributing_stochasts.append(ContributingStochast.create(0.6, stochast2))

        u1 = StandardNormal.get_u_from_p(0.2)
        u2 = StandardNormal.get_u_from_p(0.7)
        u22 = StandardNormal.get_u_from_p(0.4 + StandardNormal.get_p_from_u(1) * 0.6)

        self.assertAlmostEqual(0.0, stochast.get_x_from_u_and_source(u1, 0.0), delta=margin)
        self.assertAlmostEqual(0.0, stochast.get_x_from_u_and_source(u1, 10.0), delta=margin)
        self.assertAlmostEqual(0.0, stochast.get_x_from_u_and_source(u2, 0.0), delta=margin)
        self.assertAlmostEqual(10.0, stochast.get_x_from_u_and_source(u2, 10.0), delta=margin)
        self.assertAlmostEqual(20.0, stochast.get_x_from_u_and_source(u2, 20.0), delta=margin)
        self.assertAlmostEqual(0.0, stochast.get_x_from_u_and_source(u22, 0.0), delta=margin)
        self.assertAlmostEqual(11.0, stochast.get_x_from_u_and_source(u22, 10.0), delta=margin)
        self.assertAlmostEqual(20.0, stochast.get_x_from_u_and_source(u22, 20.0), delta=margin)

    def test_composite_conditional_truncated(self):
        stochast1 = Stochast()
        stochast1.distribution = DistributionType.deterministic
        stochast1.location = 0

        stochast2 = Stochast()
        stochast2.distribution = DistributionType.normal
        stochast2.truncated = True

        stochast2.conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = 0
        conditional1.location = 0
        conditional1.scale = 1
        conditional1.minimum = 0
        conditional1.maximum = np.inf
        stochast2.conditional_values.append(conditional1)

        conditional2 = ConditionalValue()
        conditional2.x = 10
        conditional2.location = 10
        conditional2.scale = 5
        conditional2.minimum = 0
        conditional2.maximum = np.inf
        stochast2.conditional_values.append(conditional2)

        stochast = Stochast()
        stochast.distribution = DistributionType.composite
        stochast.contributing_stochasts.append(ContributingStochast.create(0.4, stochast1))
        stochast.contributing_stochasts.append(ContributingStochast.create(0.6, stochast2))

        for u in range(-5, 5, 1):
            for x in range (0, 10, 1):
                print (f'u={u}, x={x} => {stochast.get_x_from_u_and_source(u, x)}')
                #self.assertGreaterEqual(stochast.get_x_from_u_and_source(u, x), 0.0)

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

    def test_stochast_print(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 10
        stochast.deviation = 1

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()
        stochast.print()
        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        expected = """Variable:
  distribution = normal
Definition:
  location = 10
  scale = 1
Derived values:
  mean = 10
  deviation = 1
  variation = 0.1
"""
        self.assertEqual(expected, printed)

    def test_conditional_stochast_print(self):

        stochast = Stochast()
        stochast.distribution = DistributionType.normal
        stochast.mean = 10
        stochast.deviation = 1

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

        stochast.print()

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()
        stochast.print()
        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        expected = """Variable:
  distribution = normal
Definition:
  conditional x values = [0, 1]
  location = [0, 1]
  scale = [1, 2]
"""
        self.assertEqual(expected, printed)

    def test_return_time(self):
        for i in range(6):
            beta = float(i)
            rt = StandardNormal.get_t_from_u(beta)
            betaCalculated = StandardNormal.get_u_from_t(rt)
            self.assertAlmostEqual(beta, betaCalculated, delta=margin)

if __name__ == '__main__':
    unittest.main()
