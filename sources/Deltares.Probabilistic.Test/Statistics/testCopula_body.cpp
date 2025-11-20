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
#include "../../Deltares.Probabilistic/Statistics/CorrelationMatrix.h"
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
        EXPECT_NEAR(b, 1.4845704558, margin);
    }

    void testCopula::testGaussian()
    {
        constexpr double margin = 1e-12;

        auto correlation = CorrelationMatrix();
        correlation.init(2);
        correlation.SetCorrelation(0, 1, 0.5, correlationType::Gaussian);
        auto correlationWithType = correlation.GetCorrelation(0, 1);
        EXPECT_NEAR(0.5, correlationWithType.value, margin);
    }

    void testCopula::testGumbel()
    {
        constexpr double margin = 1e-9;

        auto gumbelCopula = GumbelCopula(0.7);
        double a = 1.0;
        double b = 1.0;
        gumbelCopula.update_uspace(a, b);
        EXPECT_NEAR(b, 0.5057196397, margin);
    }

}

