import unittest
import sys

from reliability import DesignPoint
from statistic import StandardNormal, Stochast, DiscreteValue, CorrelationMatrix
from project import CombineProject

import project_builder

margin = 0.02

class Test_combine(unittest.TestCase):
    def test_directional_sampling_series(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = 'directional_sampling'
        project.settings.combine_type = 'series'

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(2 * q - q * q), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])
        
    def test_importance_sampling_series(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = 'importance_sampling'
        project.settings.combine_type = 'series'

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(2 * q - q * q), project.design_point.reliability_index, delta=margin)
        
    def test_directional_sampling_parallel(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = 'directional_sampling'
        project.settings.combine_type = 'parallel'

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(q * q), project.design_point.reliability_index, delta=margin)
        
    def test_importance_sampling_parallel(self):
        q = 0.01;

        project = CombineProject()

        beta = StandardNormal.get_u_from_q(q)

        dp1 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp1)

        dp2 = project_builder.get_design_point(beta, 2)
        project.design_points.append(dp2)

        project.settings.combiner_method = 'importance_sampling'
        project.settings.combine_type = 'parallel'

        project.run()

        self.assertAlmostEqual(StandardNormal.get_u_from_q(q * q), project.design_point.reliability_index, delta=margin)

        self.assertEqual(len(project.design_points), len(project.design_point.contributing_design_points))
        self.assertEqual(project.design_points[0], project.design_point.contributing_design_points[0])
        
        
if __name__ == '__main__':
    unittest.main()
