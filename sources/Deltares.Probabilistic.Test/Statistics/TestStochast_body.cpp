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
#include "TestStochast.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestStochast::testSortFragilityCurves()
    {
        auto stochast = Statistics::Stochast();
        auto prop = stochast.getProperties();
        auto val1 = std::make_shared<Statistics::FragilityValue>();
        val1->X = -1.0;
        val1->Reliability = 0.1;
        auto val2 = std::make_shared<Statistics::FragilityValue>();
        val2->X = 0.0;
        val2->Reliability = 0.5;
        auto val3 = std::make_shared<Statistics::FragilityValue>();
        val3->X = 1.0;
        val3->Reliability = 0.25;
        prop->FragilityValues.push_back(val1);
        prop->FragilityValues.push_back(val2);
        prop->FragilityValues.push_back(val3);

        // sort on X:
        prop->sortFragilityValuesOnX();
        // check that also properties of stochast are sorted:
        EXPECT_EQ(stochast.getProperties()->FragilityValues[2]->X, 1.0);

        // sort on Reliability:
        prop->sortFragilityValuesOnReliability();
        // check that also properties of stochast are sorted:
        EXPECT_EQ(stochast.getProperties()->FragilityValues[2]->X, 0.0);
    }

}

