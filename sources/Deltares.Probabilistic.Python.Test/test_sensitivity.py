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
from probabilistic_library.sensitivity import SensitivityResult, SensitivityValue, SensitivityMethod
from probabilistic_library.project import SensitivityProject
from probabilistic_library.statistic import Stochast, DistributionType

import project_builder

margin = 0.02

class Test_sensitivity(unittest.TestCase):

    def test_single_variation_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation

        project.run()

        sens = project.result

        self.assertEqual('L - (a+b)' , sens.identifier)
        self.assertEqual(2, len(sens.values))
        self.assertAlmostEqual(2.7, sens.values[0].low, delta=margin)
        self.assertAlmostEqual(1.8, sens.values[0].medium, delta=margin)
        self.assertAlmostEqual(0.9, sens.values[0].high, delta=margin)

    def test_single_variation_settings_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation
        project.settings.low_value = 0.3
        project.settings.high_value = 0.99

        project.run()

        sens = project.result

        self.assertEqual('L - (a+b)' , sens.identifier)
        self.assertEqual(2, len(sens.values))
        self.assertAlmostEqual(2.2, sens.values[0].low, delta=margin)
        self.assertAlmostEqual(1.8, sens.values[0].medium, delta=margin)
        self.assertAlmostEqual(0.82, sens.values[0].high, delta=margin)

    def test_single_variation_multiple_unbalanced_linear(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation

        project.run();

        self.assertEqual(3, len(project.results))

        sens1 = project.results[0]

        self.assertEqual('x' , sens1.identifier)

        self.assertEqual(sens1 , project.results['x'])

        self.assertEqual(2, len(sens1.values))
        self.assertAlmostEqual(2.07, sens1.values[0].low, delta=margin)
        self.assertAlmostEqual(1.8, sens1.values[0].medium, delta=margin)
        self.assertAlmostEqual(1.53, sens1.values[0].high, delta=margin)
        self.assertEqual(project.variables[0] , sens1.values[0].variable) 

        self.assertAlmostEqual(3.51, sens1.values[1].low, delta=margin)
        self.assertAlmostEqual(1.8, sens1.values[1].medium, delta=margin)
        self.assertAlmostEqual(0.09, sens1.values[1].high, delta=margin)

        sens2= project.results[1]

        self.assertEqual('y' , sens2.identifier)
        self.assertEqual(2, len(sens2.values))
        self.assertAlmostEqual(2.34, sens2.values[0].low, delta=margin)
        self.assertAlmostEqual(1.8, sens2.values[0].medium, delta=margin)
        self.assertAlmostEqual(1.26, sens2.values[0].high, delta=margin)

    def test_single_variation_realizations(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation
        project.settings.save_realizations = True

        project.run()

        sens = project.result

        self.assertEqual(5, project.total_model_runs)
        self.assertEqual(5, len(sens.realizations))

    def test_sobol_linear(self):
        project = project_builder.get_sensitivity_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.sobol

        project.run()

        sens = project.result

        self.assertEqual('L - (a+b)' , sens.identifier)
        self.assertEqual(2, len(sens.values))
        self.assertAlmostEqual(0.501, sens.values[0].first_order_index, delta=margin)
        self.assertAlmostEqual(0.504, sens.values[0].total_index, delta=margin)
        self.assertAlmostEqual(0.499, sens.values[1].first_order_index, delta=margin)
        self.assertAlmostEqual(0.496, sens.values[1].total_index, delta=margin)

    def test_sobol_multiple_unbalanced_linear(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.sobol

        project.run();

        self.assertEqual(3, len(project.results))

        sens1 = project.results[0]

        self.assertEqual('x' , sens1.identifier)
        self.assertEqual(2, len(sens1.values))
        self.assertAlmostEqual(0.02, sens1.values[0].first_order_index, delta=margin)
        self.assertAlmostEqual(0.02, sens1.values[0].total_index, delta=margin)

        self.assertAlmostEqual(0.98, sens1.values[1].first_order_index, delta=margin)
        self.assertAlmostEqual(0.98, sens1.values[1].total_index, delta=margin)

        sens2= project.results[1]

        self.assertEqual('y' , sens2.identifier)
        self.assertEqual(2, len(sens2.values))
        self.assertAlmostEqual(0.175, sens2.values[0].first_order_index, delta=margin)
        self.assertAlmostEqual(0.175, sens2.values[0].total_index, delta=margin)

    def test_single_variation_plot(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation

        project.run();

        self.assertEqual(3, len(project.results))

        sens1 = project.results[0]

        test_file_name = 'sens.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        # uncomment next line to show plot
        #sens1.get_plot().show()

        sens1.get_plot().savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)

    def test_sobol_plot(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.sobol

        project.run();

        self.assertEqual(3, len(project.results))

        sens1 = project.results[0]

        test_file_name = 'sens.png'
        if os.path.exists(test_file_name):
            os.remove(test_file_name)

        # uncomment next line to show plot
        #sens1.get_plot().show()

        sens1.get_plot().savefig(test_file_name)

        self.assertTrue(os.path.exists(test_file_name))
        os.remove(test_file_name)

    def test_sobol_print(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.sobol

        project.run();

        self.assertEqual(3, len(project.results))

        sens1 = project.results[0]

        sys.stdout = StringIO()

        sens1.print()

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("""Parameter: x
Values:
  a: first order index = 0.02403, total index = 0.02446
  b: first order index = 0.9812, total index = 0.9793
""", printed)

    def test_validation_settings(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation
        project.settings.low_value = -0.1
        project.settings.high_value = 1.0

        self.assertFalse(project.settings.is_valid())

        sys.stdout = StringIO()

        project.settings.validate();

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("""Error: low value value -0.1 is not greater than 0.
Error: high value value 1 is not less than 1.
""", printed)

    def test_validation(self):
        project = project_builder.get_sensitivity_multiple_unbalanced_linear_project()

        project.settings.sensitivity_method = SensitivityMethod.single_variation
        project.settings.low_value = -0.1
        project.settings.high_value = 1.0

        self.assertFalse(project.is_valid())

        sys.stdout = StringIO()

        project.validate();

        printed = sys.stdout.getvalue()
        sys.stdout = sys.__stdout__

        self.assertEqual("""Error: low value value -0.1 is not greater than 0.
Error: high value value 1 is not less than 1.
""", printed)


if __name__ == '__main__':
    unittest.main()
