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
#include "../../Deltares.Probabilistic/Statistics/CorrelationValueAndType.h"

#include <numbers>
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void TestProjectHandler::TestSetAndGetMessage()
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

    void TestProjectHandler::TestStandardNormal()
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

    void TestProjectHandler::TestProbabilityValue()
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

    void TestProjectHandler::TestStochast()
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

    void TestProjectHandler::TestCopula()
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

    void TestProjectHandler::LinearZ(Models::ModelSampleStruct* sample)
    {
        sample->Z = sample->Values[0] - sample->Values[1];
    }

    void TestProjectHandler::LinearZmulti(Models::ModelSampleStruct* samples, int sampleCount)
    {
        for (int i = 0; i < sampleCount; i++)
        {
            LinearZ(&samples[i]);
        }
    }

    void TestProjectHandler::TestRunProject()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("run_project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        const auto id1 = handler.Create("run_project");
        handler.SetModelSampleCallBack(id1, "model", LinearZ);
        handler.SetMultipleModelSampleCallBack(id1, "model", LinearZmulti);

        const auto id2 = handler.Create("stochast");
        const auto id3 = handler.Create("stochast");

        auto random = Numeric::RandomValueGenerator();
        random.initialize(true, 1234);
        const double determinist1 = random.next();
        const double determinist2 = random.next();

        handler.SetStringValue(id2, "distribution", "deterministic");
        handler.SetValue(id2, "mean", determinist1);
        handler.SetStringValue(id3, "distribution", "deterministic");
        handler.SetValue(id3, "mean", determinist2);

        const auto id4 = handler.Create("copula_correlation");

        std::vector values = { id2, id3 };
        handler.SetArrayIntValue(id1, "variables", values.data(), static_cast<int>(values.size()));
        handler.SetIntValue(id1, "copula_correlation", id4);

        handler.Execute(id1, "run");

        const int id5 = handler.GetIdValue(id1, "realization");

        const double Z_through_server = handler.GetValue(id5, "z");
        const double Z_expected = determinist1 - determinist2;
        EXPECT_NEAR(Z_expected, Z_through_server, 1e-12);

        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test reliability project with default settings
    /// </summary>
    void TestProjectHandler::TestReliabilityProject()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        const auto id1 = handler.Create("project");
        handler.SetModelSampleCallBack(id1, "model", LinearZ);
        handler.SetMultipleModelSampleCallBack(id1, "model", LinearZmulti);

        const auto id2 = handler.Create("stochast");
        const auto id3 = handler.Create("stochast");

        handler.SetStringValue(id2, "distribution", "normal");
        handler.SetValue(id2, "mean", 3.0);
        handler.SetValue(id2, "deviation", 1.0);
        handler.SetStringValue(id3, "distribution", "normal");
        handler.SetValue(id3, "mean", 1.0);
        handler.SetValue(id3, "deviation", 1.0);

        const auto id4 = handler.Create("copula_correlation");

        std::vector values = { id2, id3 };
        handler.SetArrayIntValue(id1, "variables", values.data(), static_cast<int>(values.size()));
        handler.SetIntValue(id1, "copula_correlation", id4);

        handler.Execute(id1, "run");

        const int id5 = handler.GetIdValue(id1, "design_point");
        const int id6 = handler.GetIndexedIdValue(id5, "alphas", 0);
        const int id7 = handler.GetIndexedIdValue(id5, "alphas", 1);

        const double beta_through_server = handler.GetValue(id5, "beta");
        constexpr double beta_expected = std::numbers::sqrt2;
        EXPECT_NEAR(beta_expected, beta_through_server, 1e-4);

        const double x1_through_server = handler.GetValue(id6, "x");
        const double x2_through_server = handler.GetValue(id7, "x");
        EXPECT_NEAR(x1_through_server, 2.0, 1e-4);
        EXPECT_NEAR(x2_through_server, 2.0, 1e-4);

        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test reliability project with default settings, except for the reliability method: now Crude Monte Carlo
    /// </summary>
    void TestProjectHandler::TestReliabilityProjectCM()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        ASSERT_TRUE(handler.CanHandle("settings"));
        const auto id1 = handler.Create("project");
        handler.SetModelSampleCallBack(id1, "model", LinearZ);
        handler.SetMultipleModelSampleCallBack(id1, "model", LinearZmulti);

        const auto id2 = handler.Create("stochast");
        const auto id3 = handler.Create("stochast");

        handler.SetStringValue(id2, "distribution", "normal");
        handler.SetValue(id2, "mean", 3.0);
        handler.SetValue(id2, "deviation", 1.0);
        handler.SetStringValue(id3, "distribution", "normal");
        handler.SetValue(id3, "mean", 1.0);
        handler.SetValue(id3, "deviation", 1.0);

        const auto id4 = handler.Create("copula_correlation");

        std::vector values = { id2, id3 };
        handler.SetArrayIntValue(id1, "variables", values.data(), static_cast<int>(values.size()));
        handler.SetIntValue(id1, "copula_correlation", id4);

        const auto id5 = handler.Create("settings");
        std::string default_method = handler.GetStringValue(id5, "reliability_method");
        EXPECT_EQ(default_method, "form");
        handler.SetStringValue(id5, "reliability_method", "crude_monte_carlo");
        std::string current_method = handler.GetStringValue(id5, "reliability_method");
        EXPECT_EQ(current_method, "crude_monte_carlo");
        handler.SetIntValue(id1, "settings", id5);

        handler.Execute(id1, "run");

        const int id6 = handler.GetIdValue(id1, "design_point");
        const int id7 = handler.GetIndexedIdValue(id6, "alphas", 0);
        const int id8 = handler.GetIndexedIdValue(id6, "alphas", 1);

        const double beta_through_server = handler.GetValue(id6, "beta");
        constexpr double beta_expected = 1.456;
        EXPECT_NEAR(beta_expected, beta_through_server, 1e-3);

        const double x1_through_server = handler.GetValue(id7, "x");
        const double x2_through_server = handler.GetValue(id8, "x");
        EXPECT_NEAR(x1_through_server, 2.00, 1e-2);
        EXPECT_NEAR(x2_through_server, 2.06, 1e-2);

        handler.Destroy(id8);
        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    void TestProjectHandler::TestProjectEntries()
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

}

