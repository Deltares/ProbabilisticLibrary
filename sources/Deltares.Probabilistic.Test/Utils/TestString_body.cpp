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
#include "TestString.h"
#include "../../Deltares.Probabilistic/Utils/ProbabilisticLibraryString.h"

namespace Deltares::Probabilistic::Test
{
    void TestString::testDouble2Str()
    {
        double x = 1.23;
        std::string value = Reliability::ProbabilisticLibraryString::double2str(x);
        value = Reliability::ProbabilisticLibraryString::trim(value, " ");
        EXPECT_EQ(value, "1.23");

        x = 1e-12;
        value = Reliability::ProbabilisticLibraryString::double2str(x);
        value = Reliability::ProbabilisticLibraryString::trim(value, " ");
        EXPECT_EQ(value, "1e-12");
    }

    void TestString::testDoubles2Str()
    {
        const auto x = std::vector({ -1e-12, 1e-12 });
        const auto value = Reliability::ProbabilisticLibraryString::doubles2strTrimmed(x);
        EXPECT_EQ(value, "-1e-12_1e-12");
    }

}

