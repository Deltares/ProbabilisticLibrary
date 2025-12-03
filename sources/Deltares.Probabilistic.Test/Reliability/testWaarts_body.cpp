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
#include "testWaarts.h"
#include "../projectBuilder.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Reliability/FORM.h"
#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../../Deltares.Probabilistic/Reliability/NumericalIntegration.h"
#include "../../Deltares.Probabilistic/Reliability/ImportanceSampling.h"
#include "../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSampling.h"
#include "../../Deltares.Probabilistic/Reliability/FORMThenDirectionalSampling.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaarts::WaartsLinearResistance()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            v->Z = v->Values[0] - v->Values[1];
            return v->Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(7.0, 1.0));
        stochast.push_back(projectBuilder::getNormalStochast(2.0, 1.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaarts::ExpectedValuesLinearResistance()
    {
        auto expected = WaartsResult();
        expected.beta = 3.54;
        expected.alpha = { sqrt(0.5), -sqrt(0.5) };
        expected.x = { 4.5, 4.5 };
        return expected;
    }

    void TestWaarts::WaartsLinearResistanceFORM()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORM>();
        auto expected = ExpectedValuesLinearResistance();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceCrudeMonteCarlo()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::CrudeMonteCarlo>();
        auto expected = ExpectedValuesLinearResistance();
        expected.alpha_margin = 0.15;
        expected.x_margin = 0.5;
        expected.success = false;
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceDirectionalSampling()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::DirectionalSampling>();
        auto expected = ExpectedValuesLinearResistance();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceNumericalIntegration()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::NumericalIntegration>();
        auto expected = ExpectedValuesLinearResistance();
        expected.success = false;
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceImportanceSampling()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::ImportanceSampling>();
        auto expected = ExpectedValuesLinearResistance();
        expected.x_margin = 0.1;
        expected.success = false;
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceAdaptiveImportanceSampling()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::AdaptiveImportanceSampling>();
        auto expected = ExpectedValuesLinearResistance();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceFDIR()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORMThenDirectionalSampling>();
        auto expected = ExpectedValuesLinearResistance();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::WaartsLinearResistanceDSFI()
    {
        auto modelRunner = WaartsLinearResistance();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::DirectionalSamplingThenFORM>();
        auto expected = ExpectedValuesLinearResistance();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaarts::RunSingleWaartsTest(const std::shared_ptr<Models::ModelRunner>& modelRunner,
        const std::unique_ptr<Reliability::ReliabilityMethod>& calculator, const WaartsResult& expected)
    {
        auto designPoint = calculator->getDesignPoint(modelRunner);
        ASSERT_FALSE(designPoint == nullptr);
        EXPECT_NEAR(expected.beta, designPoint->Beta, expected.beta_margin);
        if (!expected.alpha.empty())
        {
            ASSERT_EQ(expected.alpha.size(), designPoint->Alphas.size());
            for(size_t i = 0; i < expected.alpha.size(); i++)
            {
                EXPECT_NEAR(expected.alpha[i], designPoint->Alphas[i]->Alpha, expected.alpha_margin);
            }
        }
        if (!expected.x.empty())
        {
            ASSERT_EQ(expected.x.size(), designPoint->Alphas.size());
            for (size_t i = 0; i < expected.x.size(); i++)
            {
                EXPECT_NEAR(expected.x[i], designPoint->Alphas[i]->X, expected.x_margin);
            }
        }
        EXPECT_EQ(expected.success, designPoint->convergenceReport->IsConverged);
    }
}
