// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Optimization;

namespace Deltares.Probabilistic.Test
{
    public static class ProjectBuilder
    {
        public static OptimizationProject GetPolynomeOptimizationProject(double x = 0, double y = 0, double c = 0)
        {
            var project = new OptimizationProject();

            project.InputParameters.Add(new ModelParameter { Name = "X" });
            project.InputParameters.Add(new ModelParameter { Name = "Y" });

            Polynome polynome = new Polynome { C = c, X = x, Y = y };
            project.ZFunction = polynome.CalcSample;

            return project;
        }

        class Polynome
        {
            public double X = 0;
            public double Y = 0;
            public double C = 0;

            public void CalcSample(ModelSample sample)
            {
                double x = sample.Values[0] - X;
                double y = sample.Values[1] - Y;
                sample.Z = C + 10 * x * x + y * y;
            }
        }
    }
}

