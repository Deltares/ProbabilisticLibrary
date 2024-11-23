// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
//
#include <gtest/gtest.h>
#include <cmath>
#include "testRootfinder.h"
#include "../../Deltares.Probabilistic/Math/RootFinders/BisectionRootFinder.h"
#include "../../Deltares.Probabilistic/Math/RootFinders/LinearRootFinder.h"
#include "../../Deltares.Probabilistic/Utils/probLibException.h"

namespace Deltares::Probabilistic::Test
{

    void rootfinder_tests::all_rootfinder_tests()
    {
        //bisection_test();
        //bisection_test2();
        bisection_const_test();
        bisection_nod_test();
        linear_root_finder_test();
        linear_root_finder_test2();
        linear_root_finder_const_test();
        linear_root_finder_nod_test();
    }

    double rootfinder_tests::testLinearFunc(double x) const
    {
        return a + b * x;
    }

    double rootfinder_tests::testConstFunc([[maybe_unused]] double x)
    {
        cnt++;
        if (cnt > 1000) throw Reliability::probLibException("too many function calls");
        return 1.0;
    }

    double rootfinder_tests::testNodFunc(double x)
    {
        cnt++;
        if (cnt > 1000) throw Reliability::probLibException("too many function calls");
        return (x < 0 ? a : a + b * x);
    }

    void rootfinder_tests::bisection_test()
    {
        a = 3.0;
        b = 0.5;
        auto bisectionRF = Numeric::BisectionRootFinder();
        auto result = bisectionRF.CalculateValue(1.0, 2.0, 0.0,
            [this](double x) {return testLinearFunc(x); });
        ASSERT_NEAR(result, -6.0, 1e-12);
    }

    void rootfinder_tests::bisection_test2()
    {
        a = 3.0;
        b = 0.5;
        auto bisectionRF = Numeric::BisectionRootFinder();
        auto result = bisectionRF.CalculateValue(-1.0, -2.0, 0.0,
            [this](double x) {return testLinearFunc(x); });
        ASSERT_NEAR(result, -6.0, 1e-12);
    }

    void rootfinder_tests::bisection_const_test()
    {
        cnt = 0;
        auto bisectionRF = Numeric::BisectionRootFinder();
        EXPECT_NO_THROW(
            {
                auto result = bisectionRF.CalculateValue(-1.0, -2.0, 0.0,
                    [this](double x) {return testConstFunc(x); });
                ASSERT_TRUE(std::abs(result) > 1e50);
            });
    }

    void rootfinder_tests::bisection_nod_test()
    {
        cnt = 0;
        a = -1.0;
        b = 1.0;
        auto bisectionRF = Numeric::BisectionRootFinder();
        EXPECT_NO_THROW(
            {
                auto result = bisectionRF.CalculateValue(-1.0, -2.0, 0.0,
                    [this](double x) {return testNodFunc(x); });
                ASSERT_NEAR(result, 1.0, 1e-3);
                ASSERT_EQ(cnt, 17);
            });
    }

    void rootfinder_tests::linear_root_finder_test()
    {
        a = 3.0;
        b = 0.5;
        auto linear_root_finder = Numeric::LinearRootFinder(1e-6, 25);
        auto result = linear_root_finder.CalculateValue(1.0, 2.0, 0.0,
            [this](double x) {return testLinearFunc(x); });
        ASSERT_NEAR(result, -6.0, 1e-12);
    }

    void rootfinder_tests::linear_root_finder_test2()
    {
        a = 3.0;
        b = 0.5;
        auto linear_root_finder = Numeric::LinearRootFinder(1e-6, 25);
        auto result = linear_root_finder.CalculateValue(-1.0, -2.0, 0.0,
            [this](double x) {return testLinearFunc(x); });
        ASSERT_NEAR(result, -6.0, 1e-12);
    }

    void rootfinder_tests::linear_root_finder_const_test()
    {
        cnt = 0;
        auto linear_root_finder = Numeric::LinearRootFinder(1e-6, 25);
        EXPECT_NO_THROW(
            {
                auto result = linear_root_finder.CalculateValue(-1.0, -2.0, 0.0,
                    [this](double x) {return testConstFunc(x); });
                ASSERT_TRUE(std::isnan(result));
            });
    }

    void rootfinder_tests::linear_root_finder_nod_test()
    {
        cnt = 0;
        a = -1.0;
        b = 1.0;
        auto linear_root_finder = Numeric::LinearRootFinder(1e-6, 50);
        EXPECT_NO_THROW(
            {
                auto result = linear_root_finder.CalculateValue(-1.0, 0.01, 0.0,
                    [this](double x) {return testNodFunc(x); });
                ASSERT_NEAR(result, 1.0, 1e-6);
                ASSERT_EQ(cnt, 4);
            });
    }


}

