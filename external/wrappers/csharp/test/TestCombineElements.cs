// Copyright (C) Stichting Deltares and State of the Netherlands 2023. All rights reserved.
//
// This file is part of the Hydra Ring Application.
//
// The Hydra Ring Application is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.

using Deltares.ProbabilisticCPP.Wrapper;

namespace Probabilistic.Tests
{
    public class TestCombineElements
    {
        public static void runAllCombinTests()
        {
            TestCombineTwoElementsPartialCorrelation();
            TestCombineMultipleElements();
        }

        private static void TestCombineTwoElementsPartialCorrelation()
        {
            var beta1 = 3.0;
            var beta2 = 4.0;
            double beta3;
            var alpha1 = new[] {0.7, 0.7};
            var alpha2 = new[] {0.7, -0.7};
            var alpha3 = new double[2];
            var rhoP = new double[] {1, 1};
            PLCombin.CombineTwoElementsPartialCorrelation(beta1, alpha1, beta2, alpha2, rhoP, out beta3, alpha3,
                CombineAndOr.CombineOr);
            Comparer.AreEqual(2.99293, beta3, 1e-5, "diff beta combin1a");

            PLCombin.CombineTwoElementsPartialCorrelation(beta1, alpha1, beta2, alpha2, rhoP, out beta3, alpha3,
                CombineAndOr.CombineAnd);
            Comparer.AreEqual(5.35511, beta3, 1e-5, "diff beta combin1b");

            var alphaI = new double[2];
            var alphaIi = new double[2];
            PLCombin.CombineTwoElementsPartialCorrelation(beta1, alpha1, beta2, alpha2, rhoP, out beta3, alpha3,
                CombineAndOr.CombineAnd, alphaI, alphaIi);
            Comparer.AreEqual(5.35511, beta3, 1e-5, "diff beta combin1c");
        }

        private static void TestCombineMultipleElements()
        {
            var betaElement = new[] { 3.0, 4.0 };
            var alphaElement = new[,] { { 0.7, 0.7 }, { 0.7, -0.7 } };
            var rhoP = new double[] { 1, 1 };
            double beta; var alpha = new double[2];
            PLCombin.CombineMultipleElements(betaElement, alphaElement, rhoP, out beta, alpha, CombineAndOr.CombineOr);
            Comparer.AreEqual(2.99293, beta, 1e-5, "diff beta combin2");
        }
    }
}
