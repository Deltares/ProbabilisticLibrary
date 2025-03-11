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
#include <limits>
#include <cmath>
#include "../../Deltares.Probabilistic/Utils/probLibString.h"
#include "testToString.h"

using namespace Deltares::Reliability;

namespace Deltares::Probabilistic::Test
{
    void to_string_tests::all_to_string_tests()
    {
        double x = 1.23456789;
        auto s = probLibString::double2str(x);
        EXPECT_EQ(s, "       1.234568");

        x = 123456789;
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "123456789.00000");

        x = -123456789;
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "-1.23457e+08");

        x = 1.23456789e20;
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "1.23457e+20");

        x = -1.23456789e20;
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "-1.23457e+20");

        x = std::numeric_limits<double>::infinity();
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "            inf");

        x = -std::numeric_limits<double>::infinity();
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "           -inf");

        x = std::nan("");
        s = probLibString::double2str(x);
        EXPECT_EQ(s, "            nan");
    }

}

