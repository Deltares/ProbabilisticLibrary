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
#include <gtest/gtest.h>
#include "testRandom.h"
#include "../../Deltares.Probabilistic/Math/RandomValueGenerator.h"

namespace Deltares::Probabilistic::Test
{
    void testRandom::allRandomTests()
    {
        mersenneTwisterTest1();
        initializationTest();
        repetitiveTest();
        twoInstances();
    }

    void testRandom::mersenneTwisterTest1()
    {
        constexpr double margin = 1e-12;
        auto mt = Numeric::RandomValueGenerator();
        mt.initialize(true, 1);

        double sum = 0.0;
        for (size_t i = 0; i < 1000; i++)
        {
            sum += mt.next();
        }
        ASSERT_NEAR(sum, 510.36972065202127, margin);
    }

    void testRandom::initializationTest()
    {
        auto mt = Numeric::RandomValueGenerator();
        mt.initialize(false, 0);

        double val2 = mt.next();
        ASSERT_TRUE(val2 >= 0.0 && val2 <= 1.0);
    }

    void testRandom::repetitiveTest()
    {
        auto mt = Numeric::RandomValueGenerator();

        size_t size1 = 1000;
        size_t size2 = 1500;

        std::vector<double> values1;
        std::vector<double> values2;
        std::vector<double> values3;
        std::vector<double> values4;

        mt.initialize(true, 0);
        for (size_t i = 0; i < size1; i++)
        {
            values1.push_back(mt.next());
        }

        mt.initialize(true, 1);
        for (size_t i = 0; i < size2; i++)
        {
            values2.push_back(mt.next());
        }

        mt.initialize(true, 0);
        for (size_t i = 0; i < size1; i++)
        {
            values3.push_back(mt.next());
        }

        mt.initialize(true, 1);
        for (size_t i = 0; i < size2; i++)
        {
            values4.push_back(mt.next());
        }

        for (size_t i = 0; i < size1; i++)
        {
            ASSERT_EQ(values1[i], values3[i]);
        }

        for (size_t i = 0; i < size2; i++)
        {
            ASSERT_EQ(values2[i], values4[i]);
        }
    }

    void testRandom::twoInstances()
    {
        auto mt1 = Numeric::RandomValueGenerator();
        mt1.initialize(true, 0);
        auto mt2 = Numeric::RandomValueGenerator();
        mt2.initialize(true, 0);

        double val1 = mt1.next();
        double val2 = mt2.next();
        ASSERT_EQ(val1, val2);
    }

}

