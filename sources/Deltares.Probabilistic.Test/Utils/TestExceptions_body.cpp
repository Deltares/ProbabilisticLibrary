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
#include "TestExceptions.h"
#include "../../Deltares.Probabilistic/Utils/probLibException.h"

namespace Deltares::Probabilistic::Test
{
    void TestExceptions::testProblibExceptions()
    {
        std::string message;
        try
        {
            throw Reliability::probLibException("message", 12.34);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "message 12.3400");

        try
        {
            throw Reliability::probLibException("message", 1e99);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "message 1.0000e+99");

        try
        {
            constexpr int a = -1234;
            throw Reliability::probLibException("message", a);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "message -1234");

        try
        {
            constexpr size_t a = 123456789;
            throw Reliability::probLibException("message", a);
        }
        catch (const Reliability::probLibException& e)
        {
            message = e.what();
        }
        EXPECT_EQ(message, "message 123456789");
    }


}

