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
import os

from io import StringIO

from probabilistic_library.utils import FrozenList, FrozenObject
from probabilistic_library.reliability import (DesignPoint, DesignPointMethod, ReliabilityMethod, CompareType, StartMethod,
                                               FragilityCurve, FragilityValue)
from probabilistic_library.project import ReliabilityProject
from probabilistic_library.statistic import (Stochast, DistributionType, ContributingStochast, ConditionalValue,
                                             CorrelationType, CopulaType)

import project_builder

margin = 0.01

class Test_reliability(unittest.TestCase):

    def test_invalid_project(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        self.assertTrue(project.is_valid())

        # change an unused property to an invalid value
        project.variables['a'].scale = -1
        self.assertTrue(project.is_valid())

        # now use the property, reassign the invalid property, because by changing the scale is reset
        project.variables['a'].distribution = DistributionType.normal
        project.variables['a'].scale = -1
        self.assertFalse(project.is_valid())

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()

        # run an invalid project
        project.run();
        self.assertIsNone(project.design_point)

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("""Error: a => scale value -1 is less than 0.\n""", printed)

    def test_form_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

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
        self.assertTrue(project.design_point.alphas[0].variable in project.variables)
        self.assertEqual('a', project.design_point.alphas[0].variable.name)

        self.assertEqual(0, len(dp.messages))

    def test_form_initialized_linear(self):
        project = project_builder.get_linear_initialized_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_generated_model(self):

        project = ReliabilityProject()

        func_code = """
def f(a,b):
    z = 1.8 - (a+b)
    return z
"""
        project.model = func_code
        project_builder.assign_distributions(project, DistributionType.uniform)

        self.assertEqual('a', project.model.input_parameters[0].name)
        self.assertEqual('z', project.model.output_parameters[0].name)
        self.assertEqual('a', project.variables[0].name)
        self.assertEqual('f', project.model.name)

        project.settings.reliability_method = ReliabilityMethod.form

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_invalid_generated_model(self):

        project = ReliabilityProject()

        func_code = """
z = 1.8 + (a+b)
def f(a,b):
    z = 1.8 + (a+b)
    return z
"""
        project.model = func_code

        self.assertFalse(project.is_valid())

        func_code = """
def g(a,b):
    z = 1.8 - (a+b)
    return z
"""
        project.model = func_code

        self.assertTrue(project.is_valid())
        self.assertEqual(2, len(project.model.input_parameters))
        self.assertEqual('a', project.model.input_parameters[0].name)
        self.assertEqual('z', project.model.output_parameters[0].name)
        self.assertEqual('a', project.variables[0].name)
        self.assertEqual('g', project.model.name)

        func_code = """
def h(a,b,c):
    z = 1.8 - (a+b+c)
    return z
"""
        project.model = func_code

        self.assertTrue(project.is_valid())
        self.assertEqual(3, len(project.model.input_parameters))
        self.assertEqual('a', project.model.input_parameters[0].name)
        self.assertEqual('c', project.model.input_parameters[2].name)
        self.assertEqual('z', project.model.output_parameters[0].name)
        self.assertEqual('a', project.variables[0].name)
        self.assertEqual('h', project.model.name)

    def test_form_initialized_linear_parallel(self):
        project = project_builder.get_linear_initialized_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.max_parallel_processes = 4

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_array_result_linear(self):
        project = project_builder.get_linear_array_result_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_crude_monte_carlo_linear_composite(self):
        project = project_builder.get_linear_project()

        project.variables['b'].distribution = DistributionType.composite

        composite1 = Stochast()
        composite1.distribution = DistributionType.uniform
        composite1.minimum = -1.5
        composite1.maximum = -1
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.4, composite1))

        composite2 = Stochast()
        composite2.distribution = DistributionType.uniform
        composite2.minimum = 0.5
        composite2.maximum = 1
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.6, composite2))

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.32, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.78, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)

        self.assertAlmostEqual(-0.62, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(0.93, alphas[1].x, delta=margin)

    def test_crude_monte_carlo_linear_composite_conditional(self):
        project = project_builder.get_linear_project()
        project.settings.save_realizations = True

        project.variables['b'].distribution = DistributionType.composite

        composite1 = Stochast()
        composite1.distribution = DistributionType.uniform
        composite1.minimum = -5
        composite1.maximum = -4
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.4, composite1))

        composite2 = Stochast()
        composite2.distribution = DistributionType.uniform

        project_builder.assign_conditional_values(composite2)
        composite2.conditional_source = 'a'

        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.6, composite2))

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run();

        dp = project.design_point;

        for real in dp.realizations:
            first = real.input_values[1] >= -5 and real.input_values[1] <= -4
            second = abs(real.input_values[1] - real.input_values[0]) <= 0.2
            self.assertTrue(first or second)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.91, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.89, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(0.91, alphas[0].x, delta=margin)

        self.assertAlmostEqual(-0.45, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(0.98, alphas[1].x, delta=margin)

    def test_crude_monte_carlo_linear_composite_conditional_array(self):
        project = project_builder.get_linear_array_project()
        project.settings.save_realizations = True

        project.variables['a'].is_array = True
        project.variables['b'].is_array = True

        project.variables['a'].array_size = 5
        project.variables['b'].array_size = 5

        project.variables['b'].distribution = DistributionType.composite

        composite1 = Stochast()
        composite1.distribution = DistributionType.uniform
        composite1.minimum = -5
        composite1.maximum = -4
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.4, composite1))

        composite2 = Stochast()
        composite2.distribution = DistributionType.uniform

        project_builder.assign_conditional_values(composite2)
        composite2.conditional_source = 'a'

        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.6, composite2))

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run();

        dp = project.design_point;

        for real in dp.realizations:
            first = real.input_values[6] >= -5 and real.input_values[6] <= -4
            second = abs(real.input_values[6] - real.input_values[1]) <= 0.2
            real.print()
            self.assertTrue(first or second)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.06, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(-0.30, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(0.47, alphas[1].x, delta=margin)

        self.assertAlmostEqual(-0.32, alphas[6].alpha, delta=margin)
        self.assertAlmostEqual(0.49, alphas[6].x, delta=margin)

    def test_crude_monte_carlo_linear_composite_conditional_array_nonarray(self):

        import numpy as np

        project = project_builder.get_linear_array_project()
        project.settings.save_realizations = True

        project.variables['L'].distribution = DistributionType.normal
        project.variables['L'].mean = 0.4
        project.variables['L'].deviation = 0.05
        project.variables['L'].truncated = True
        project.variables['L'].minimum = 0
        project.variables['L'].maximum = np.inf

        project.variables['a'].is_array = True
        project.variables['a'].array_size = 12

        project.variables['b'].distribution = DistributionType.composite
        project.variables['b'].is_array = True
        project.variables['b'].array_size = 12

        ba_top_zero = Stochast()
        ba_top_zero.distribution = DistributionType.deterministic
        ba_top_zero.mean = 0
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.2, ba_top_zero)) 

        ba_top = Stochast()
        ba_top.distribution = DistributionType.normal
        ba_top.mean = 0.2
        ba_top.deviation = 0.03
        ba_top.truncated = True
        ba_top.conditional = True
        ba_top.conditional_source = 'L'
        ba_top.conditional_values.append(ConditionalValue())
        ba_top.conditional_values.append(ConditionalValue())
        ba_top.conditional_values[0].x = 0
        ba_top.conditional_values[0].mean = 0
        ba_top.conditional_values[0].deviation = 0.06
        ba_top.conditional_values[0].minimum = 0
        ba_top.conditional_values[0].maximum = np.inf
        ba_top.conditional_values[1].x = 1
        ba_top.conditional_values[1].mean = 1
        ba_top.conditional_values[1].deviation = 0.06
        ba_top.conditional_values[1].minimum = 0
        ba_top.conditional_values[1].maximum = np.inf
        project.variables['b'].contributing_stochasts.append(ContributingStochast.create(0.8, ba_top)) 

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run()

        dp = project.design_point;

        for real in dp.realizations:
            first = real.input_values[0] >= 0
            second = real.input_values[15] >= 0 and real.input_values[16] >= 0 and real.input_values[17] >= 0
            self.assertTrue(first or second)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(-1.61, beta, delta=margin)
        self.assertEqual(25, len(alphas))


    def test_form_linear_conditional(self):
        project = project_builder.get_linear_project()

        project_builder.assign_conditional_values(project.variables['b'])
        project.variables['b'].conditional_source = 'a'

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.56, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.96, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(0.86, alphas[0].x, delta=margin)

        self.assertAlmostEqual(-0.29, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(0.93, alphas[1].x, delta=margin)


    def test_form_linear_array(self):
        project = project_builder.get_linear_array_project()

        self.assertEqual(False, project.variables['L'].is_array)
        self.assertEqual(True, project.variables['a'].is_array)
        self.assertEqual(True, project.variables['b'].is_array)

        project.variables['a'].array_size = 5
        project.variables['b'].array_size = 5

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.71, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(0, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(1.8, alphas[0].x, delta=margin)
        self.assertEqual(project.variables['L'], alphas[0].variable)

        self.assertAlmostEqual(-0.31, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(0.18, alphas[1].x, delta=margin)
        self.assertEqual(0, alphas[1].index)
        self.assertEqual(4, alphas[5].index)
        self.assertEqual(project.variables['a'], alphas[1].variable)
        self.assertEqual('a[0]', str(alphas[1]))

        self.assertAlmostEqual(-0.31, alphas[6].alpha, delta=margin)
        self.assertAlmostEqual(0.18, alphas[6].x, delta=margin)
        self.assertEqual(0, alphas[6].index)
        self.assertEqual(project.variables['b'], alphas[6].variable)

    def test_form_linear_varying_array(self):
        project = project_builder.get_linear_array_project()

        self.assertEqual(False, project.variables['L'].is_array)
        self.assertEqual(True, project.variables['a'].is_array)
        self.assertEqual(True, project.variables['b'].is_array)

        project.variables['a'].distribution = DistributionType.deterministic
        project.variables['b'].distribution = DistributionType.deterministic

        project.variables['a'].array_size = 5
        project.variables['b'].array_size = 5

        for i in range(project.variables['a'].array_size):
            a_array = Stochast()
            a_array.distribution = DistributionType.uniform
            a_array.minimum = i-3
            a_array.maximum = i-1
            project.variables['a'].array_variables.append(a_array)

        for i in range(project.variables['b'].array_size):
            b_array = Stochast()
            b_array.distribution = DistributionType.uniform
            b_array.minimum = i-3
            b_array.maximum = i-1
            project.variables['b'].array_variables.append(b_array)

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.71, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(0, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(1.8, alphas[0].x, delta=margin)
        self.assertEqual(project.variables['L'], alphas[0].variable)

        self.assertAlmostEqual(-0.31, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(-1.82, alphas[1].x, delta=margin)
        self.assertAlmostEqual(2.18, alphas[5].x, delta=margin)
        self.assertEqual(0, alphas[1].index)
        self.assertEqual(4, alphas[5].index)
        self.assertEqual(project.variables['a'].array_variables[0], alphas[1].variable)
        self.assertEqual('a[0]', str(alphas[1]))

        self.assertAlmostEqual(-0.31, alphas[6].alpha, delta=margin)
        self.assertAlmostEqual(-1.82, alphas[6].x, delta=margin)
        self.assertEqual(0, alphas[6].index)
        self.assertEqual(project.variables['b'].array_variables[0], alphas[6].variable)

    def test_form_linear_conditional_array(self):
        project = project_builder.get_linear_array_project()

        project.variables['a'].array_size = 5
        project.variables['b'].array_size = 5

        project_builder.assign_conditional_values(project.variables['b'])

        project.variables['b'].conditional_source = 'a'

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.51, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(0, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.44, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(-0.046, alphas[6].alpha, delta=margin)

        self.assertEqual(0, alphas[0].index)
        self.assertEqual(0, alphas[1].index)
        self.assertEqual(4, alphas[5].index)
        self.assertEqual(0, alphas[6].index)

        self.assertEqual(project.variables['L'], alphas[0].variable)
        self.assertEqual(project.variables['a'], alphas[1].variable)
        self.assertEqual(project.variables['a'], alphas[5].variable)
        self.assertEqual(project.variables['b'], alphas[6].variable)

        self.assertAlmostEqual(1.8, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.18, alphas[1].x, delta=margin)
        self.assertAlmostEqual(0.18, alphas[6].x, delta=margin)

    def test_form_linear_conditional_array_nonarray(self):
        project = project_builder.get_linear_array_project()

        project.variables['a'].array_size = 5
        project.variables['b'].array_size = 5

        project.variables['b'].conditional = True

        conditional1 = ConditionalValue()
        conditional1.x = -1.8
        conditional1.minimum = -1
        conditional1.maximum = 1
        project.variables['b'].conditional_values.append(conditional1)

        project.variables['b'].conditional_source = 'L'

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.72, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(0, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.32, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(-0.31, alphas[6].alpha, delta=margin)

        self.assertEqual(0, alphas[0].index)
        self.assertEqual(0, alphas[1].index)
        self.assertEqual(4, alphas[5].index)
        self.assertEqual(0, alphas[6].index)

        self.assertEqual(project.variables['L'], alphas[0].variable)
        self.assertEqual(project.variables['a'], alphas[1].variable)
        self.assertEqual(project.variables['a'], alphas[5].variable)
        self.assertEqual(project.variables['b'], alphas[6].variable)

        self.assertAlmostEqual(1.8, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.18, alphas[1].x, delta=margin)
        self.assertAlmostEqual(0.18, alphas[6].x, delta=margin)

    def test_form_linear_conditional_varying_array(self):
        project = project_builder.get_linear_array_project()

        project.variables['a'].array_size = 5
        for i in range(project.variables['a'].array_size):
            a_array = Stochast()
            a_array.distribution = DistributionType.uniform
            a_array.minimum = -1 + 0.4 * i
            a_array.maximum = -0.6 + 0.4 * i
            project.variables['a'].array_variables.append(a_array)

        project.variables['b'].array_size = 5

        project_builder.assign_conditional_values(project.variables['b'])
        project.variables['b'].conditional_source = 'a'

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(11, len(alphas))

        self.assertAlmostEqual(0, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.37, alphas[1].alpha, delta=margin)
        self.assertAlmostEqual(-0.25, alphas[6].alpha, delta=margin)

        self.assertEqual(0, alphas[0].index)
        self.assertEqual(0, alphas[1].index)
        self.assertEqual(4, alphas[5].index)
        self.assertEqual(0, alphas[6].index)

        self.assertEqual(project.variables['L'], alphas[0].variable)
        self.assertEqual(project.variables['a'].array_variables[0], alphas[1].variable)
        self.assertEqual(project.variables['a'].array_variables[4], alphas[5].variable)
        self.assertEqual(project.variables['b'], alphas[6].variable)

        self.assertAlmostEqual(1.8, alphas[0].x, delta=margin)
        self.assertAlmostEqual(-0.67, alphas[1].x, delta=margin)
        self.assertAlmostEqual(-0.57, alphas[6].x, delta=margin)

    def test_form_limit_state_functions(self):
        project = project_builder.get_multiple_unbalanced_linear_project()

        self.assertEqual(3, len(project.model.output_parameters))
        self.assertEqual('x', project.model.output_parameters[0].name)
        self.assertEqual('y', project.model.output_parameters[1].name)
        self.assertEqual('z', project.model.output_parameters[2].name)

        project.limit_state_function.parameter = 'y'
        project.limit_state_function.compare_type = CompareType.less_than
        project.limit_state_function.critical_value = 0.1

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.99, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.31, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.95, alphas[1].alpha, delta=margin)

        project.limit_state_function.critical_value = 'z'

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(0.0, beta, delta=margin)
        self.assertEqual(2, len(alphas))


    # def test_form_linear_fragility_curve(self):
    #     project = project_builder.get_linear_project()

    #     project.settings.reliability_method = 'form'
    #     project.settings.reliability_result = 'fragility_curve'
    #     project.settings.fragility_variable = 'a'
    #     project.settings.fragility_values = [1, 2, 3]

    #     project.run();

    #     dp = project.fragility_curve

    #     self.assertAlmostEqual(2.33, beta, delta=margin)
    #     self.assertEqual(2, len(alphas))

    #     self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
    #     self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

    #     self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
    #     self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

    #     self.assertEqual(len(project.variables), len(project.design_point.alphas))
    #     self.assertTrue(project.design_point.alphas[0].variable in project.variables)
    #     self.assertEqual('a', project.design_point.alphas[0].variable.name)


    def test_form_linear_run_twice(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;

        self.assertAlmostEqual(2.33, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.9, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.9, alphas[1].x, delta=margin)

        self.assertEqual(len(project.variables), len(project.design_point.alphas))
        self.assertTrue(project.design_point.alphas[0].variable in project.variables)

        project.correlation_matrix[(project.variables[0], project.variables[1])] = 0.5
        project.run();

        dp = project.design_point;

        self.assertAlmostEqual(1.90, dp.reliability_index, delta=margin)


    def test_form_linear_fully_correlated(self):
        project = project_builder.get_linear_fully_correlated_project()

        project.settings.reliability_method = ReliabilityMethod.form

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

        project.settings.reliability_method = ReliabilityMethod.form

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

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.58, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.70, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_crude_monte_carlo_plot(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.run();

        dp = project.design_point;

        test_file_name = 'design_point.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        dp.get_plot_alphas().savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)


    def test_directional_sampling_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling

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

        project.settings.reliability_method = ReliabilityMethod.importance_sampling

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.58, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.7, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

        self.assertAlmostEqual(0.93, alphas[0].x, delta=margin)
        self.assertAlmostEqual(0.94, alphas[1].x, delta=margin)

    def test_importance_sampling_linear_small(self):
        project = project_builder.get_linear_small_project()

        project.settings.reliability_method = ReliabilityMethod.importance_sampling
        project.settings.minimum_samples = 5000
        project.settings.maximum_samples = 10000

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.91, beta, delta=margin)

    def test_numerical_integration_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        self.assertEqual(2, len(project.settings.stochast_settings))

        for stochastSettings in project.settings.stochast_settings:
            stochastSettings.start_value = 0
            stochastSettings.variance_factor = 2

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

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        project.settings.stochast_settings['a'].min_value = -1
        project.settings.stochast_settings['b'].min_value = -1

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.57, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_numerical_integration_bligh(self):
        project = project_builder.get_bligh_project()

        project.settings.reliability_method = ReliabilityMethod.crude_monte_carlo

        project.settings.minimum_samples = 1000
        project.settings.maximum_samples = 50000
        project.settings.variation_coefficient = 0.02

        project.run()

        project.settings.reliability_method = ReliabilityMethod.numerical_integration

        project.settings.stochast_settings['m'].intervals = 20
        project.settings.stochast_settings['L'].intervals = 20
        project.settings.stochast_settings['delta_H'].intervals = 20

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertEqual(20*20*20+1, dp.total_model_runs)
        self.assertAlmostEqual(1.55, beta, delta=margin)
        self.assertEqual(4, len(alphas))

    def test_directional_sampling_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling

        project.settings.minimum_directions = 1000
        project.settings.maximum_directions = 2000
        project.settings.variation_coefficient = 0.02

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.88, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_directional_sampling_hunt_parallel(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling

        project.settings.minimum_directions = 1000
        project.settings.maximum_directions = 2000
        project.settings.variation_coefficient = 0.02
        project.settings.max_parallel_processes = 4

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.88, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_form_start_methods(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.sphere_search
        project.settings.all_quadrants = True

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 150)
        self.assertFalse(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 1)

        self.assertAlmostEqual(1.9877, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_start_methods_given_vector(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.fixed_value
        project.settings.stochast_settings["a"].start_value = 1.2
        project.settings.stochast_settings["b"].start_value = 2.4

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 150)
        self.assertFalse(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 0)

        self.assertAlmostEqual(1.9877, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_form_start_methods_max_steps(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form
        project.settings.start_method = StartMethod.sphere_search
        project.settings.all_quadrants = True
        project.settings.max_steps_sphere_search = 25

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;
        self.assertEqual(dp.total_model_runs, 6)
        self.assertTrue(dp.is_converged)
        self.assertEqual(len(dp.contributing_design_points), 1)
        self.assertEqual(176, dp.contributing_design_points[0].total_model_runs)

        self.assertAlmostEqual(2.3258, beta, delta=margin)
        self.assertEqual(2, len(alphas))

    def test_numerical_bisection_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.numerical_bisection

        project.settings.minimum_iterations = 5
        project.settings.maximum_iterations = 6
        project.settings.epsilon_beta = 0.1

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.890, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_latin_hypercube_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.latin_hypercube

        project.settings.minimum_samples = 25000

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.8987, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_cobyla_linear(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.cobyla_reliability

        project.run();

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.326, beta, delta=margin)
        self.assertEqual(2, len(alphas))

        self.assertAlmostEqual(-0.71, alphas[0].alpha, delta=margin)
        self.assertAlmostEqual(-0.71, alphas[1].alpha, delta=margin)

    def test_fdir_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.form_then_directional_sampling

        project.settings.maximum_iterations = 50
        project.settings.epsilon_beta = 0.05

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(2.05, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_dsfi_hunt(self):
        project = project_builder.get_hunt_project()

        project.settings.reliability_method = ReliabilityMethod.directional_sampling_then_form

        project.settings.maximum_iterations = 50
        project.settings.epsilon_beta = 0.05

        project.run()

        dp = project.design_point;
        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(1.87, beta, delta=margin)
        self.assertEqual(5, len(alphas))

    def test_fragility_curve(self):
        fragility_curve = FragilityCurve()
        fragility_curve.name = 'q'
        
        value1 = FragilityValue()
        value1.x = 0
        value1.reliability_index = 4.2
        fragility_curve.fragility_values.append(value1)

        value2 = FragilityValue()
        value2.x = 10
        value2.reliability_index = 2.6
        fragility_curve.fragility_values.append(value2)
        
        integrand = Stochast()
        integrand.name = 'h'
        integrand.distribution = DistributionType.normal
        integrand.mean = 5
        integrand.deviation = 1
        
        dp = fragility_curve.integrate(integrand)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.35, beta, delta=margin)
        self.assertEqual(2, len(alphas))
        self.assertEqual(integrand, alphas[0].variable)
        self.assertEqual(fragility_curve, alphas[1].variable)

    def test_fragility_curve_hunt(self):
        project = project_builder.get_hunt_project()
        project.settings.reliability_method = ReliabilityMethod.form
        
        h_stochast = Stochast()
        h_stochast.name = 'h'
        h_stochast.copy_from(project.variables['h'])

        fragility_curve = FragilityCurve()
        fragility_curve.name = 'q'
        
        for h_value in [1, 2, 3]:
            project.variables['h'].distribution = DistributionType.deterministic
            project.variables['h'].mean = h_value
            project.run()
            dp = project.design_point;
        
            value = FragilityValue()
            value.x = 10
            value.design_point = dp
            fragility_curve.fragility_values.append(value)

        dp = fragility_curve.integrate(h_stochast)

        beta = dp.reliability_index;
        alphas = dp.alphas;

        self.assertAlmostEqual(3.79, beta, delta=margin)
        self.assertEqual(6, len(alphas))
        self.assertEqual(h_stochast, alphas[0].variable)

    def test_design_point_print(self):

        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.run();

        dp = project.design_point;

        dp.print()

        # Replace default stdout (terminal) temporary with with our stream
        sys.stdout = StringIO()
        dp.print()
        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        expected = """Reliability (FORM)
 Reliability index = 2.327
 Probability of failure = 0.009979
 Convergence = 0.008632 (converged)
 Model runs = 18
Alpha values:
 a: alpha = -0.7071, x = 0.9001
 b: alpha = -0.7071, x = 0.9001

"""
        self.assertEqual(expected, printed)

    def test_form_copula_gumbel(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.correlation_type = CorrelationType.copulas
        project.copulas[(project.variables[0], project.variables[1])] = (5.0, CopulaType.gumbel)
        project.run()

        dp = project.design_point

        self.assertAlmostEqual(1.654, dp.reliability_index, delta=margin)

    def test_form_copula_clayton(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.correlation_type = CorrelationType.copulas
        project.copulas[(project.variables[0], project.variables[1])] = (4.0, CopulaType.clayton)
        project.run()

        dp = project.design_point

        self.assertAlmostEqual(1.86, dp.reliability_index, delta=margin)

    def test_form_copula_frank(self):
        project = project_builder.get_linear_project()

        project.settings.reliability_method = ReliabilityMethod.form

        project.correlation_type = CorrelationType.copulas
        project.copulas[(project.variables[0], project.variables[1])] = (4.0, CopulaType.frank)
        project.run()

        dp = project.design_point

        self.assertAlmostEqual(1.92, dp.reliability_index, delta=margin)

    def test_validation_gaussian_correlation(self):
        project = project_builder.get_hunt_project()
        project.settings.reliability_method = ReliabilityMethod.form

        project.correlation_type = CorrelationType.gaussian
        project.correlation_matrix[(project.variables[0], project.variables[1])] = -0.9
        project.correlation_matrix[(project.variables[0], project.variables[2])] = -0.9
        project.correlation_matrix[(project.variables[1], project.variables[2])] = -0.9

        sys.stdout = StringIO()
        project.validate()
        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__
        self.assertEqual(printed, "Error: Correlation Matrix => Cholesky decomposition fails.\n")

    def test_validation_fully_correlation(self):
        project = project_builder.get_hunt_project()
        project.settings.reliability_method = ReliabilityMethod.form

        project.correlation_type = CorrelationType.gaussian
        project.correlation_matrix[(project.variables[0], project.variables[1])] = 1.0
        project.correlation_matrix[(project.variables[0], project.variables[2])] = 1.0
        self.assertFalse(project.is_valid())

        project.correlation_matrix[(project.variables[1], project.variables[2])] = 1.0
        self.assertTrue(project.is_valid())


if __name__ == '__main__':
    unittest.main()
