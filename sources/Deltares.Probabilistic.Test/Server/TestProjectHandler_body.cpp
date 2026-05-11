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
#include "TestProjectHandler.h"
#include "../../Deltares.Probabilistic/Server/ProjectHandler.h"
#include "../../Deltares.Probabilistic/Math/RandomValueGenerator.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"

#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestProjectHandler::TestSetAndGetMessage()
    {
        auto handler = Server::ProjectHandler();
        const auto id = handler.Create("message");

        handler.SetStringValue(id, "text", "hello world");
        handler.SetStringValue(id, "type", "debug");
        handler.SetStringValue(id, "subject", "unit test");
        const auto result1 = handler.GetStringValue(id, "text");
        EXPECT_EQ(result1, "hello world");
        const auto result2 = handler.GetStringValue(id, "type");
        EXPECT_EQ(result2, "debug");
        const auto result3 = handler.GetStringValue(id, "subject");
        EXPECT_EQ(result3, "unit test");

        handler.Destroy(id);
    }

    void TestProjectHandler::TestStandardNormal()
    {
        auto handler = Server::ProjectHandler();
        const auto id = handler.Create("standard_normal");

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);

        const double q = random.next();
        const double u_expected = Statistics::StandardNormal::getUFromQ(q);
        const auto u_through_server = handler.GetArgValue(id, "u_from_q", q);
        EXPECT_EQ(u_through_server, u_expected);

        const double u = random.next();
        const double q_expected = Statistics::StandardNormal::getQFromU(u);
        const auto q_through_server = handler.GetArgValue(id, "q_from_u", u);
        EXPECT_EQ(q_through_server, q_expected);

        handler.Destroy(id);
    }

}

