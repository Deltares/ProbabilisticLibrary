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
//#include <cmath>
#include "TestNumIntShared.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalIntegrationShared.h"

namespace Deltares::Probabilistic::Test
{
    void TestNumIntShared::testDefaults()
    {
        auto settings = Reliability::StochastSettings();
        auto u_values = Reliability::NumericalIntegrationShared::buildUpList(settings);
        ASSERT_EQ(u_values.size(), settings.Intervals + 1);
        EXPECT_EQ(u_values.front(), settings.MinValue);
        EXPECT_EQ(u_values.back(), settings.MaxValue);
    }

    void TestNumIntShared::testGivenURange()
    {
        auto settings = Reliability::StochastSettings();
        settings.MinValue = -5.0;
        settings.MaxValue = 5.0;
        auto u_values = Reliability::NumericalIntegrationShared::buildUpList(settings);
        EXPECT_EQ(u_values.size(), settings.Intervals + 3);
        EXPECT_EQ(u_values.front(), -8.0);
        EXPECT_EQ(u_values.back(), 8.0);
    }

    void TestNumIntShared::testGivenURange2()
    {
        auto settings = Reliability::StochastSettings();
        settings.MinValue = -15.0;
        settings.MaxValue = 15.0;
        auto u_values = Reliability::NumericalIntegrationShared::buildUpList(settings);
        EXPECT_EQ(u_values.size(), 109);
        EXPECT_EQ(u_values.front(), -8.0);
        EXPECT_EQ(u_values.back(), 8.0);
    }

}
