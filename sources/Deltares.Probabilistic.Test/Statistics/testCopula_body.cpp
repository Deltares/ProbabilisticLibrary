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
#include "testCopula.h"
#include "../../Deltares.Probabilistic/Statistics/ClaytonCopula.h"
#include "../../Deltares.Probabilistic/Statistics/FrankCopula.h"
#include "../../Deltares.Probabilistic/Statistics/GumbelCopula.h"
#include "../../Deltares.Probabilistic/Statistics/GaussianCopula.h"
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    using namespace Deltares::Statistics;

    void testCopula::testClayton()
    {
        constexpr double margin = 1e-9;

        auto claytonCopula = ClaytonCopula(0.7);
        double a = 1.0;
        double b = 1.0;
        claytonCopula.update_uspace(a, b);
        EXPECT_NEAR(b, 1.23779153247, margin);
    }

    void testCopula::testFrank()
    {
        constexpr double margin = 1e-9;

        auto frankCopula = FrankCopula(0.7);
        double a = 1.0;
        double b = 1.0;
        frankCopula.update_uspace(a, b);
        EXPECT_NEAR(b, 1.1216920258, margin);
    }

    void testCopula::testGaussian()
    {
        constexpr double margin = 1e-12;

        auto correlation = GaussianCopula(0.5);
        auto correlationWithType = correlation.getCorrelation();
        EXPECT_NEAR(0.5, correlationWithType.value, margin);
        constexpr double u1 = 0.9;
        double u2 = 0.8;
        correlation.update_uspace(u1, u2);
        const double expected = u1 * 0.5 + 0.8 * sqrt(0.75);
        EXPECT_NEAR(expected, u2, margin);
    }

    void testCopula::testGumbel()
    {
        constexpr double margin = 1e-4;

        auto gumbelCopula = GumbelCopula(5.0);
        double a = 0.2;
        double b = 0.2;
        gumbelCopula.update(a, b);
        EXPECT_NEAR(b, 0.1355, margin) << "comparison with Matlab fails";
    }

}

