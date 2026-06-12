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

#include "UnitTestProjectHandler.h"
#include "../../Deltares.Probabilistic/Server/ProjectHandler.h"
#include "../../Deltares.Probabilistic/Math/RandomValueGenerator.h"
#include "../../Deltares.Probabilistic/Statistics/StandardNormal.h"
#include "../../Deltares.Probabilistic/Statistics/CorrelationValueAndType.h"

#include <numbers>
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void UnitTestProjectHandler::TestSetAndGetMessage()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("message"));
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

    void UnitTestProjectHandler::TestStandardNormal()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("standard_normal"));
        const auto id = handler.Create("standard_normal");

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);

        const double q = random.next();
        const double u_expected = Statistics::StandardNormal::getUFromQ(q);
        const double u_through_server = handler.GetArgValue(id, "u_from_q", q);
        EXPECT_EQ(u_through_server, u_expected);

        const double u = random.next();
        const double q_expected = Statistics::StandardNormal::getQFromU(u);
        const double q_through_server = handler.GetArgValue(id, "q_from_u", u);
        EXPECT_EQ(q_through_server, q_expected);

        handler.Destroy(id);
    }

    void UnitTestProjectHandler::TestProbabilityValue()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("probability_value"));
        const auto id = handler.Create("probability_value");

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);

        const double t = 5000.0 * (1.0 + random.next());
        handler.SetValue(id, "return_period", t);
        const double beta_through_server = handler.GetValue(id, "reliability_index");
        const double beta_expected = Statistics::StandardNormal::getUFromT(t);

        EXPECT_EQ(beta_through_server, beta_expected);

        handler.Destroy(id);
    }

    void UnitTestProjectHandler::TestStochast()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("stochast"));
        const auto id = handler.Create("stochast");

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);

        const double mean = random.next();
        const double deviation = random.next();
        const double u = random.next();

        handler.SetStringValue(id, "distribution", "normal");
        handler.SetValue(id, "mean", mean);
        handler.SetValue(id, "deviation", deviation);
        const double x_through_server = handler.GetArgValue(id, "x_from_u", u);
        const double x_expected = mean + deviation * u;
        EXPECT_NEAR(x_through_server, x_expected, 1e-12);

        handler.Destroy(id);
    }

    void UnitTestProjectHandler::TestCopula()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        const auto id1 = handler.Create("stochast");
        const auto id2 = handler.Create("stochast");
        const auto id3 = handler.Create("copula_correlation");

        std::vector values = { id1, id2 };
        handler.SetArrayIntValue(id3, "variables", values.data(), static_cast<int>(values.size()));

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);
        // get a random correlation value ; for Frank correlation every number is valid except 0.0
        const double correlation_value = 10.0 + 10.0 * random.next();
        constexpr int copula_type = static_cast<int>(CorrelationType::Frank);
        handler.SetIndexedIndexedIntValue(id3, "correlation", id1, id2, copula_type);
        handler.SetIndexedIndexedValue(id3, "correlation", id1, id2, correlation_value);

        const double correlation_through_server = handler.GetIndexedIndexedValue(id3, "correlation", id1, id2);
        EXPECT_EQ(correlation_value, correlation_through_server);

        const int dimension = handler.GetIntValue(id3, "variables_count");
        EXPECT_EQ(dimension, 2);

        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    void UnitTestProjectHandler::TestSettings()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("settings"));
        const auto id1 = handler.Create("settings");
        std::string default_method = handler.GetStringValue(id1, "reliability_method");
        EXPECT_EQ(default_method, "form");
        handler.SetStringValue(id1, "reliability_method", "crude_monte_carlo");
        std::string current_method = handler.GetStringValue(id1, "reliability_method");
        EXPECT_EQ(current_method, "crude_monte_carlo");
        handler.Destroy(id1);
    }

    void UnitTestProjectHandler::TestProjectEntries()
    {
        using namespace Server;
        using enum ObjectType;

        bool found_entry = ProjectEntries::CanHandle("message");
        EXPECT_TRUE(found_entry);
        bool missing_entry = ProjectEntries::CanHandle("MESSAGE");
        EXPECT_FALSE(missing_entry);

        ObjectType type = ProjectEntries::GetType("stochast");
        EXPECT_EQ(type, Stochast);

        std::string error_message;
        try
        {
            ProjectEntries::GetType("STOCHAST");
        }
        catch (const Reliability::probLibException& e)
        {
            error_message = e.what();
        }
        EXPECT_EQ(error_message, "type not supported: STOCHAST");
    }

    void UnitTestProjectHandler::TestCreateDestroyAllEntries()
    {
        auto handler = Server::ProjectHandler();
        auto ids = std::vector<int>();
        for (const auto& [name, type] : Server::ProjectEntries::all_entries)
        {
            if (type != Server::ObjectType::ValidationReport)
            {
                ids.push_back(handler.Create(name));
            }
        }
        for (const auto& id : ids)
        {
            handler.Destroy(id);
        }
    }

    void UnitTestProjectHandler::TestProjectEntriesIsModelProject()
    {
        using namespace Server;
        using enum ObjectType;

        EXPECT_FALSE(ProjectEntries::IsModelProjectType(Alpha));
        EXPECT_FALSE(ProjectEntries::IsModelProjectType(CombineProject));
        EXPECT_TRUE(ProjectEntries::IsModelProjectType(Project));
        EXPECT_TRUE(ProjectEntries::IsModelProjectType(UncertaintyProject));
        EXPECT_TRUE(ProjectEntries::IsModelProjectType(RunProject));
        EXPECT_TRUE(ProjectEntries::IsModelProjectType(SensitivityProject));
    }

    void UnitTestProjectHandler::TestProjectEntriesIsModelSettingsType()
    {
        using namespace Server;
        using enum ObjectType;

        EXPECT_FALSE(ProjectEntries::IsModelSettingsType(Alpha));
        EXPECT_FALSE(ProjectEntries::IsModelSettingsType(CombineSettings));
        EXPECT_TRUE(ProjectEntries::IsModelSettingsType(Settings));
        EXPECT_TRUE(ProjectEntries::IsModelSettingsType(UncertaintySettings));
        EXPECT_TRUE(ProjectEntries::IsModelSettingsType(RunProjectSettings));
        EXPECT_TRUE(ProjectEntries::IsModelSettingsType(SensitivitySettings));
    }

    void UnitTestProjectHandler::TestProjectEntriesIsStochast()
    {
        using namespace Server;
        using enum ObjectType;

        EXPECT_FALSE(ProjectEntries::IsStochast(Alpha));
        EXPECT_FALSE(ProjectEntries::IsStochast(CombineSettings));
        EXPECT_TRUE(ProjectEntries::IsStochast(Stochast));
        EXPECT_TRUE(ProjectEntries::IsStochast(FragilityCurve));
    }
}

