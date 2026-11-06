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

#include <algorithm>
#include "TestShuffleItems.h"
#include "../../Deltares.Probabilistic/Math/ShuffleItems.h"

namespace Deltares::Numeric::Test
{
    void TestShuffleItems::BasicShuffleInPlaceTest()
    {
        constexpr auto n_items = 10;
        auto rng = RandomValueGenerator();
        auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto copy = list;
        ShuffleItems<int>::ShuffleInPlace(list, rng);
        EXPECT_EQ(list.size(), n_items);

        // check that the list is shuffled (not in the same order as the original; note that this is a probabilistic test and may occasionally fail)
        EXPECT_TRUE(list[0] != 1 || list[1] != 2 || list[2] != 3 || list[3] != 4 || list[4] != 5);

        // check that all items are present in the shuffled list
        for (const auto& item: list)
        {
            const auto pos = std::ranges::find(copy, item);
            EXPECT_NE(pos, copy.end());
        }
    }

    void TestShuffleItems::BasicGetNItemsTest()
    {
        constexpr auto n_items = 2;
        auto rng = RandomValueGenerator();
        auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
        auto result = ShuffleItems<int>::GetNShuffledItems(list, rng, n_items);
        EXPECT_EQ(result.size(), n_items);

        // check that the list is shuffled (not in the same order as the original; note that this is a probabilistic test and may occasionally fail)
        EXPECT_TRUE(result[0] != 1 || result[1] != 2);

        // check that the result is a subset of the original list
        EXPECT_NE(std::ranges::find(list, result[0]), list.end());
        EXPECT_NE(std::ranges::find(list, result[1]), list.end());

        // check that the result contains unique items
        EXPECT_NE(result[0], result[1]);
    }

    void TestShuffleItems::RepeatedShuffleInPlaceTest()
    {
        constexpr auto num_iterations = 1000;
        constexpr auto n_items = 10;
        auto rng = RandomValueGenerator();
        auto sum_items = std::vector<int>(n_items, 0);
        for (int i = 0; i < num_iterations; ++i)
        {
            auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            ShuffleItems<int>::ShuffleInPlace(list, rng);
            for (int j = 0; j < n_items; ++j)
            {
                sum_items[j] += list[j];
            }
        }
        for (int j = 0; j < n_items; ++j)
        {
            // sum_items[j] has an expected value of 5500, so we check if it is within 200 of that value
            EXPECT_TRUE((sum_items[j] >= 5300) && (sum_items[j] <= 5700));
        }
    }

    void TestShuffleItems::RepeatedGetNItemsTest()
    {
        constexpr auto num_iterations = 1000;
        constexpr auto n_items = 2;
        auto rng = RandomValueGenerator();
        auto sum_items = std::vector<int>(n_items, 0);
        for (int i = 0; i < num_iterations; ++i)
        {

            auto list = std::vector<int>{ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            auto result = ShuffleItems<int>::GetNShuffledItems(list, rng, n_items);
            for (int j = 0; j < n_items; ++j)
            {
                sum_items[j] += result[j];
            }
        }
        for (int j = 0; j < n_items; ++j)
        {
            // sum_items[j] has an expected value of 5500, so we check if it is within 200 of that value
            EXPECT_TRUE((sum_items[j] >= 5300) && (sum_items[j] <= 5700));
        }
    }
}
