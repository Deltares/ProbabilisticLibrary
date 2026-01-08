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
#include <iostream>
#include "TestWaarts.h"
#include "../../../Deltares.Probabilistic/Reliability/FORM.h"
#include "../../../Deltares.Probabilistic/Reliability/CrudeMonteCarlo.h"
#include "../../../Deltares.Probabilistic/Reliability/DirectionalSampling.h"
#include "../../../Deltares.Probabilistic/Reliability/NumericalIntegration.h"
#include "../../../Deltares.Probabilistic/Reliability/ImportanceSampling.h"
#include "../../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSampling.h"
#include "../../../Deltares.Probabilistic/Reliability/FORMThenDirectionalSampling.h"
#include "../../../Deltares.Probabilistic/Reliability/DirectionalSamplingThenFORM.h"

namespace Deltares::Probabilistic::Test
{
    void TestWaarts::WaartsFORM()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORM>();
        auto expected = expectedValuesFORM();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsCrudeMonteCarlo()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::CrudeMonteCarlo>();
        auto expected = expectedValuesCrudeMonteCarlo();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsDirectionalSampling()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::DirectionalSampling>();
        auto expected = expectedValuesDirectionalSampling();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsNumericalIntegration()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::NumericalIntegration>();
        auto expected = expectedValuesNumericalIntegration();
        expected.converged = false;
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsImportanceSampling()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::ImportanceSampling>();
        auto expected = expectedValuesImportanceSampling();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsAdaptiveImportanceSampling()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::AdaptiveImportanceSampling>();
        auto expected = expectedValuesAdaptiveImportanceSampling();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsFDIR()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORMThenDirectionalSampling>();
        auto expected = expectedValuesFDIR();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaarts::WaartsDSFI()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::DirectionalSamplingThenFORM>();
        auto expected = expectedValuesDSFI();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    std::shared_ptr<Models::ModelRunner> TestWaarts::getModelRunner(const std::shared_ptr<Models::ZModel>& model,
        const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts, const int nStochasts)
    {
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        if (nStochasts > 0)
        {
            corr->Init(nStochasts);
        }
        auto uConverter = std::make_shared <Models::UConverter>(stochasts, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(model, uConverter);
        return modelRunner;
    }

    void TestWaarts::RunSingleWaartsTest(const std::shared_ptr<Models::ModelRunner>& modelRunner,
        Reliability::ReliabilityMethod& calculator, const WaartsResult& expected)
    {
        auto designPoint = calculator.getDesignPoint(modelRunner);

        ASSERT_FALSE(designPoint == nullptr);

        if (expected.printResults)
        {
            std::cout << "DesignPoint:" << "\n";
            std::cout << "Beta = " << designPoint->Beta << "\n";
            for (size_t i = 0; i < designPoint->Alphas.size(); i++)
            {
                std::cout << "Alpha: " << i << " , " << designPoint->Alphas[i]->Alpha << " , " << designPoint->Alphas[i]->X << "\n";
            }
            std::cout << "Converged = " << designPoint->convergenceReport->IsConverged << "\n";
            std::cout.flush();
        }

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

        EXPECT_EQ(expected.converged, designPoint->convergenceReport->IsConverged);
    }
}
