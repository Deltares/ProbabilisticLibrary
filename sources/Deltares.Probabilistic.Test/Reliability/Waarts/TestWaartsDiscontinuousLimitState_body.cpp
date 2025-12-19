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

#include "TestWaartsDiscontinuousLimitState.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsDiscontinuousLimitState::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            double Z;
            const double r = v->Values[0];
            const double s = v->Values[1];
            if (r >= s)
            {
                Z = -0.5 + sqrt(r - s);
            }
            else
            {
                Z = -0.5;

            }
            v->Z = Z;
            return Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(15.0, 2.5));
        stochast.push_back(projectBuilder::getNormalStochast(5.0, 0.5));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    void TestWaartsDiscontinuousLimitState::WaartsCrudeMonteCarlo()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::CrudeMonteCarlo>();
        auto cm = dynamic_cast<Reliability::CrudeMonteCarlo*>(calculator.get());
        cm->Settings->MaximumSamples = 250000;
        auto expected = expectedValuesCrudeMonteCarlo();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    WaartsResult TestWaartsDiscontinuousLimitState::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 3.83;
        expected.alpha = { 0.98, -0.20};
        return expected;
    }

    WaartsResult TestWaartsDiscontinuousLimitState::expectedValuesFORM()
    {
        auto expected = expectedValues();
        expected.beta = -40.0;
        expected.alpha.clear();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsDiscontinuousLimitState::expectedValuesDSFI()
    {
        auto expected = expectedValues();
        expected.alpha = { 0.96, -0.27 };
        return expected;
    }

    WaartsResult TestWaartsDiscontinuousLimitState::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.alpha.clear();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsDiscontinuousLimitState::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.success = false;
        return expected;
    }
    /*

    */
}
