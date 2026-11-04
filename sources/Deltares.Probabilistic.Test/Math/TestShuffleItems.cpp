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
#include "TestShuffleItems.h"
#include "../../Deltares.Probabilistic/Math/ShuffleItems.h"

namespace Deltares::Numeric::Test
{
    void TestShuffleItems::RunTest1()
    {
        auto rng = RandomValueGenerator();
        auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        ShuffleItems<int>::ShuffleInPlace(list, rng);
        EXPECT_EQ(list.size(), 10);
        EXPECT_TRUE(list[0] != 1 || list[1] != 2 || list[2] != 3 || list[3] != 4 || list[4] != 5);
    }

    void TestShuffleItems::RunTest2()
    {
        auto rng = RandomValueGenerator();
        auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto result = ShuffleItems<int>::GetNShuffledItems(list, rng, 2);
        EXPECT_EQ(result.size(), 2);
        EXPECT_TRUE(result[0] != 1 || result[1] != 2);
    }
}
