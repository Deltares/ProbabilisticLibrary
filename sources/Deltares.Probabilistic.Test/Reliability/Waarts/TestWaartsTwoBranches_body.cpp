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

#include "TestWaartsTwoBranches.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsTwoBranches::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            const double x1 = v->Values[0];
            const double x2 = v->Values[1];
            const double x3 = v->Values[2];
            double Z;
            if (x3 <= 5.0)
            {
                Z = x1 - x2 - x3;
            }
            else
            {
                Z = x3 - x2;
            }
            v->Z = Z;
            return Z;
        });

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochasts.push_back(projectBuilder::getNormalStochast(10.0, 0.5));
        stochasts.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        stochasts.push_back(projectBuilder::getNormalStochast(4.0, 1.0));
        return getModelRunner(z, stochasts);
    }

    void TestWaartsTwoBranches::WaartsFORM()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORM>();
        auto form = dynamic_cast<Reliability::FORM*>(calculator.get());
        form->Settings->FilterAtNonConvergence = true;
        auto expected = expectedValuesFORM();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaartsTwoBranches::WaartsCrudeMonteCarlo()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::CrudeMonteCarlo>();
        auto cm = dynamic_cast<Reliability::CrudeMonteCarlo*>(calculator.get());
        cm->Settings->MaximumSamples = 10000000;
        cm->Settings->randomSettings->Seed = 1; // default seed (0), gives beta = 40
        auto expected = expectedValuesCrudeMonteCarlo();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    WaartsResult TestWaartsTwoBranches::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 5.03;
        return expected;
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesFORM()
    {
        auto expected = expectedValues();
        expected.beta = 2.45;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.beta = 5.07;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesDirectionalSampling()
    {
        auto expected = expectedValues();
        expected.beta = 5.04;
        expected.alpha = { 0.380261 , -0.913978 ,-0.141584 };
        expected.x = { 9.041, 4.61003, 4.71414 };
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesFDIR()
    {
        return expectedValuesDirectionalSampling();
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesDSFI()
    {
        auto expected = WaartsResult();
        expected.beta = 5.04;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsTwoBranches::expectedValuesAdaptiveImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 5.01;
        expected.alpha = {0.465031, -0.871085, -0.157981};
        expected.x = {8.83589, 4.36116, 4.79095};
        expected.converged = false;
        return expected;
    }

}
