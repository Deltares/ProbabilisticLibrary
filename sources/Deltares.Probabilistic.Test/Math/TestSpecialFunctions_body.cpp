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
#include "TestSpecialFunctions.h"
#include "../../Deltares.Probabilistic/Math/SpecialFunctions.h"
#include "../../Deltares.Probabilistic/Utils/probLibException.h"
#include <gtest/gtest.h>
#include <cmath>
#include <limits>

namespace Deltares::Probabilistic::Test
{
    void TestSpecialFunctions::IncompleteGammaTest()
    {
        using namespace Deltares::Numeric;
        auto references = std::vector<std::vector<double>>();
        references.push_back({ 0.632, 0.264, 0.080, 0.019, 0.004, 0.001, 0.000, 0.000, 0.000, 0.000 });
        references.push_back({ 0.865, 0.594, 0.323, 0.143, 0.053, 0.017, 0.005, 0.001, 0.000, 0.000 });
        references.push_back({ 0.950, 0.801, 0.577, 0.353, 0.185, 0.084, 0.034, 0.012, 0.004, 0.001 });
        references.push_back({ 0.982, 0.908, 0.762, 0.567, 0.371, 0.215, 0.111, 0.051, 0.021, 0.008 });
        references.push_back({ 0.993, 0.960, 0.875, 0.735, 0.560, 0.384, 0.238, 0.133, 0.068, 0.032 });
        references.push_back({ 0.998, 0.983, 0.938, 0.849, 0.715, 0.554, 0.394, 0.256, 0.153, 0.084 });
        references.push_back({ 0.999, 0.993, 0.970, 0.918, 0.827, 0.699, 0.550, 0.401, 0.271, 0.170 });
        references.push_back({ 1.000, 0.997, 0.986, 0.958, 0.900, 0.809, 0.687, 0.547, 0.407, 0.283 });
        references.push_back({ 1.000, 0.999, 0.994, 0.979, 0.945, 0.884, 0.793, 0.676, 0.544, 0.413 });
        references.push_back({ 1.000, 1.000, 0.997, 0.990, 0.971, 0.933, 0.870, 0.780, 0.667, 0.542 });
        references.push_back({ 1.000, 1.000, 0.999, 0.995, 0.985, 0.962, 0.921, 0.857, 0.768, 0.659 });
        references.push_back({ 1.000, 1.000, 1.000, 0.998, 0.992, 0.980, 0.954, 0.911, 0.845, 0.758 });
        references.push_back({ 1.000, 1.000, 1.000, 0.999, 0.996, 0.989, 0.974, 0.946, 0.900, 0.834 });
        references.push_back({ 1.000, 1.000, 1.000, 1.000, 0.998, 0.994, 0.986, 0.968, 0.938, 0.891 });
        references.push_back({ 1.000, 1.000, 1.000, 1.000, 0.999, 0.997, 0.992, 0.982, 0.963, 0.930 });

        for (int i = 1; i <= 10; i++)
        {
            for (int j = 1; j <= 15; j++)
            {
                constexpr double tol = 6e-4;
                double gamma_complement = SpecialFunctions::getGammaUpperRegularized(i, j);
                double gamma = 1.0 - gamma_complement;
                EXPECT_NEAR(gamma, references[j - 1][i - 1], tol);
            }
        }
    }

    void TestSpecialFunctions::IncompleteGammaExceptionalCasesTest()
    {
        using namespace Deltares::Numeric;
        std::string message;
        try
        {
            SpecialFunctions::getGammaUpperRegularized(-1, 1);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "Error in Dgammq: a <= 0 violated; a =  -1.0000");

        try
        {
            SpecialFunctions::getGammaUpperRegularized(1, -1);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "Error in Dgammq: x < 0 violated; x =  -1.0000");

        try
        {
            constexpr double a = std::numeric_limits<double>::infinity();
            SpecialFunctions::getGammaUpperRegularized(a, 1);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "Error in DGser: no convergence");

        try
        {
            constexpr double x = std::numeric_limits<double>::infinity();
            SpecialFunctions::getGammaUpperRegularized(1, x);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "Error in DGcf: no convergence");
    }

}

