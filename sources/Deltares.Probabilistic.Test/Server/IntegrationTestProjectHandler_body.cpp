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

#include "IntegrationTestProjectHandler.h"
#include "../../Deltares.Probabilistic/Server/ProjectHandler.h"
#include "../../Deltares.Probabilistic/Math/RandomValueGenerator.h"

#include <numbers>
#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{
    void IntegrationTestProjectHandler::LinearZ(Models::ModelSampleStruct* sample)
    {
        sample->Z = sample->Values[0] - sample->Values[1];
    }

    void IntegrationTestProjectHandler::LinearZmulti(Models::ModelSampleStruct* samples, int sampleCount)
    {
        for (int i = 0; i < sampleCount; i++)
        {
            LinearZ(&samples[i]);
        }
    }

    void IntegrationTestProjectHandler::Zvalues(double* data, int size, double* outputValues)
    {
        outputValues[0] = data[0];
        outputValues[1] = data[1];
        outputValues[2] = data[0] - data[1];
    }

    void IntegrationTestProjectHandler::ZvaluesMulti(int arraySize, double** data, int inputSize, double** outputValues)
    {
        for (int i = 0; i < arraySize; i++)
        {
            Zvalues(data[i], inputSize, outputValues[i]);
        }
    }

    /// <summary>
    /// Test run project
    /// </summary>
    void IntegrationTestProjectHandler::TestRunProject()
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
    void IntegrationTestProjectHandler::TestReliabilityProject()
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
    void IntegrationTestProjectHandler::TestReliabilityProjectCM()
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
        handler.SetStringValue(id5, "reliability_method", "crude_monte_carlo");
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

    /// <summary>
    /// Test sensitivity project with default settings
    /// </summary>
    void IntegrationTestProjectHandler::TestSensitivityProject()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("sensitivity_project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        ASSERT_TRUE(handler.CanHandle("settings"));
        const auto id1 = handler.Create("sensitivity_project");
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

        const auto id5 = handler.Create("sensitivity_settings");
        handler.SetStringValue(id5, "sensitivity_method", "single_variation");
        handler.SetValue(id5, "low_value", 0.02);
        handler.SetValue(id5, "high_value", 0.98);
        handler.SetIntValue(id1, "settings", id5);

        handler.Execute(id1, "run");

        const int id6 = handler.GetIdValue(id1, "result");
        const int id7 = handler.GetIndexedIdValue(id6, "values", 0);
        const int id8 = handler.GetIndexedIdValue(id6, "values", 1);

        const double low1_through_server = handler.GetValue(id7, "low");
        const double medium2_through_server = handler.GetValue(id8, "medium");
        const double high2_through_server = handler.GetValue(id8, "high");
        EXPECT_NEAR(low1_through_server, -0.0537, 1e-4);
        EXPECT_NEAR(medium2_through_server, 2.0, 1e-2);
        EXPECT_NEAR(high2_through_server, -0.0537, 1e-4);

        handler.Destroy(id8);
        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test uncertainty project with FORM
    /// </summary>
    void IntegrationTestProjectHandler::TestUncertaintyProject()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("uncertainty_project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        ASSERT_TRUE(handler.CanHandle("uncertainty_settings"));
        const auto id1 = handler.Create("uncertainty_project");
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

        const auto id5 = handler.Create("uncertainty_settings");
        handler.SetStringValue(id5, "uncertainty_method", "form");
        handler.SetIntValue(id1, "settings", id5);

        handler.Execute(id1, "run");

        const int id6 = handler.GetIndexedIdValue(id1, "uncertainty_stochasts", 0);
        const auto distribution_name = handler.GetStringValue(id6, "distribution");
        EXPECT_EQ("cdf_curve", distribution_name);

        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test run project with multiple output
    /// </summary>
    void IntegrationTestProjectHandler::TestRunProjectMultipleOutput()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("run_project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("model_parameter"));
        const auto id1 = handler.Create("run_project");
        handler.SetCallBack(id1, "model", Zvalues);

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

        const auto id5 = handler.Create("model_parameter");
        const auto id6 = handler.Create("model_parameter");
        const auto id7 = handler.Create("model_parameter");
        std::vector params = { id5, id6, id7 };
        handler.SetArrayIntValue(id1, "output_parameters", params.data(), static_cast<int>(params.size()));

        handler.Execute(id1, "run");

        const int id8 = handler.GetIdValue(id1, "realization");

        const double out0 = handler.GetIndexedValue(id8, "output_values", 0);
        const double out1 = handler.GetIndexedValue(id8, "output_values", 1);
        const double out2 = handler.GetIndexedValue(id8, "output_values", 2);

        const double out2_expected = determinist1 - determinist2;
        EXPECT_NEAR(out2_expected, out2, 1e-12);
        EXPECT_EQ(out0, determinist1);
        EXPECT_EQ(out1, determinist2);

        handler.Destroy(id8);
        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test combine project with default settings
    /// design points are a result of two reliability projects
    /// </summary>
    void IntegrationTestProjectHandler::TestCombineProject()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("combine_project"));
        ASSERT_TRUE(handler.CanHandle("project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));

        const auto id1 = handler.Create("combine_project");
        const auto id2 = handler.Create("project");
        const auto id3 = handler.Create("project");
        handler.SetModelSampleCallBack(id2, "model", LinearZ);
        handler.SetMultipleModelSampleCallBack(id2, "model", LinearZmulti);
        handler.SetModelSampleCallBack(id3, "model", LinearZ);
        handler.SetMultipleModelSampleCallBack(id3, "model", LinearZmulti);

        const auto id4 = handler.Create("stochast"); // for reliability project ids 2 and 3
        const auto id5 = handler.Create("stochast"); // for reliability project id 2
        const auto id6 = handler.Create("stochast"); // for reliability project id 3

        handler.SetStringValue(id4, "distribution", "normal");
        handler.SetValue(id4, "mean", 3.0);
        handler.SetValue(id4, "deviation", 1.0);
        handler.SetStringValue(id5, "distribution", "normal");
        handler.SetValue(id5, "mean", 1.0);
        handler.SetValue(id5, "deviation", 1.0);
        handler.SetStringValue(id6, "distribution", "normal");
        handler.SetValue(id6, "mean", 1.0);
        handler.SetValue(id6, "deviation", 1.0);

        const auto id7 = handler.Create("copula_correlation");
        const auto id8 = handler.Create("copula_correlation");

        std::vector values2 = { id4, id5 };
        handler.SetArrayIntValue(id2, "variables", values2.data(), static_cast<int>(values2.size()));
        handler.SetIntValue(id2, "copula_correlation", id7);

        std::vector values3 = { id4, id6 };
        handler.SetArrayIntValue(id3, "variables", values3.data(), static_cast<int>(values3.size()));
        handler.SetIntValue(id3, "copula_correlation", id8);

        handler.Execute(id2, "run");
        handler.Execute(id3, "run");

        const int id9 = handler.GetIdValue(id2, "design_point");
        const int id10 = handler.GetIdValue(id3, "design_point");

        std::vector values1 = { id9, id10 };
        handler.SetArrayIntValue(id1, "design_points", values1.data(), static_cast<int>(values1.size()));
        handler.Execute(id1, "run");
        const int id11 = handler.GetIdValue(id1, "design_point");
        const double beta_through_server = handler.GetValue(id11, "beta");
        EXPECT_NEAR(beta_through_server, 1.1066, 1e-4);
        std::vector ref_alpha = { 0.815495, -0.40925, -0.40925 };
        for (int i = 0; i < 3; i++)
        {
            const int id = handler.GetIndexedIdValue(id11, "alphas", i);
            const double alpha_through_server = handler.GetValue(id, "alpha");
            EXPECT_NEAR(alpha_through_server, ref_alpha[i], 1e-4);
            handler.Destroy(id);
        }

        handler.Destroy(id11);
        handler.Destroy(id10);
        handler.Destroy(id9);
        handler.Destroy(id8);
        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }

    /// <summary>
    /// Test reliability project with limit state function with multiple output values
    /// </summary>
    void IntegrationTestProjectHandler::TestReliabilityProjectZValues()
    {
        auto handler = Server::ProjectHandler();
        ASSERT_TRUE(handler.CanHandle("project"));
        ASSERT_TRUE(handler.CanHandle("stochast"));
        ASSERT_TRUE(handler.CanHandle("copula_correlation"));
        const auto id1 = handler.Create("project");
        handler.SetCallBack(id1, "model", Zvalues);
        handler.SetMultipleCallBack(id1, "model", ZvaluesMulti);

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

        const auto id5 = handler.Create("model_parameter");
        const auto id6 = handler.Create("model_parameter");
        const auto id7 = handler.Create("model_parameter");
        handler.SetStringValue(id7, "name", "z-value");
        std::vector params = { id5, id6, id7 };

        handler.SetArrayIntValue(id1, "output_parameters", params.data(), static_cast<int>(params.size()));

        const auto id11 = handler.Create("limit_state_function");
        handler.SetStringValue(id11, "parameter", "z-value");
        handler.SetValue(id11, "critical_value", 0.123);
        handler.SetIntValue(id1, "limit_state_function", id11);

        handler.Execute(id1, "run");

        const int id8 = handler.GetIdValue(id1, "design_point");
        const int id9 = handler.GetIndexedIdValue(id8, "alphas", 0);
        const int id10 = handler.GetIndexedIdValue(id8, "alphas", 1);

        const double beta_through_server = handler.GetValue(id8, "beta");
        constexpr double beta_expected = 1.327239428;
        EXPECT_NEAR(beta_expected, beta_through_server, 1e-4);

        const double x1_through_server = handler.GetValue(id9, "x");
        const double x2_through_server = handler.GetValue(id10, "x");
        EXPECT_NEAR(x1_through_server, 2.0615, 1e-4);
        EXPECT_NEAR(x2_through_server, 1.9385, 1e-4);

        handler.Destroy(id11);
        handler.Destroy(id10);
        handler.Destroy(id9);
        handler.Destroy(id8);
        handler.Destroy(id7);
        handler.Destroy(id6);
        handler.Destroy(id5);
        handler.Destroy(id4);
        handler.Destroy(id3);
        handler.Destroy(id2);
        handler.Destroy(id1);
    }


}

