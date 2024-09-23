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

from streams import *

import project_builder

margin = 0.02

class Test_Length_Effect(unittest.TestCase):
    def test_length_effect1(self):
        q = 0.01;

        project = LengthEffectProject()

        values = [100.0, 200.0]
        project.correlation_lengths = values
        project.length = 2000
        project.breach_length = 500

        beta = StandardNormal.get_u_from_q(q)
        dp1 = project_builder.get_design_point(beta, 2)
        project.design_point_cross_section.append(dp1)

        values = [0.45, 0.55]
        project.self_correlation = values

        project.run()

        design_point_upscaled = project.design_point
        self.assertAlmostEqual(design_point_upscaled.reliability_index, 1.282, delta=margin)
        for alpha in design_point_upscaled.alphas:
            self.assertAlmostEqual(alpha.alpha, -0.7071, delta=margin)

    def test_length_effect_no_breach_length(self):
        q = 0.01;

        project = LengthEffectProject()

        values = [100.0, 200.0]
        project.correlation_lengths = values
        project.length = 2000

        beta = StandardNormal.get_u_from_q(q)
        dp1 = project_builder.get_design_point(beta, 2)
        project.design_point_cross_section.append(dp1)

        values = [0.45, 0.55]
        project.self_correlation = values

        project.run()

        design_point_upscaled = project.design_point
        self.assertAlmostEqual(design_point_upscaled.reliability_index, 1.201, delta=margin)
        for alpha in design_point_upscaled.alphas:
            self.assertAlmostEqual(alpha.alpha, -0.7071, delta=margin)

    def test_length_effect_no_self_correlation(self):
        q = 0.01;

        project = LengthEffectProject()

        values = [100.0, 200.0]
        project.correlation_lengths = values
        project.length = 2000

        beta = StandardNormal.get_u_from_q(q)
        dp1 = project_builder.get_design_point(beta, 2)
        project.design_point_cross_section.append(dp1)

        values = [0.0, 0.0]
        project.self_correlation = values

        project.run()

        design_point_upscaled = project.design_point
        self.assertAlmostEqual(design_point_upscaled.reliability_index, 0.82196, delta=margin)

    def test_settings(self):
        q = 0.01;

        project = LengthEffectProject()

        values = [100.0, 200.0]
        project.correlation_lengths = values
        for i in range(2):
            self.assertEqual(values[i], project.correlation_lengths[i])
        project.length = 2000.0
        self.assertEqual(project.length, 2000.0)

        project.breach_length = 123.0
        self.assertEqual(project.breach_length, 123.0)

        values = [0.4, 0.6]
        project.self_correlation = values
        for i in range(2):
            self.assertEqual(values[i], project.self_correlation[i])

        beta = StandardNormal.get_u_from_q(q)
        dp1 = project_builder.get_design_point(beta, 2)
        project.design_point_cross_section.append(dp1)
        self.assertEqual(project.design_point_cross_section[0].reliability_index, beta)

if __name__ == '__main__':
    unittest.main()

