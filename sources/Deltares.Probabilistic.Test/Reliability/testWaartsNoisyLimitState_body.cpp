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

#include "testWaartsNoisyLimitState.h"
#include "../projectBuilder.h"
#include "../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsNoisyLimitState::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            double summation = 0.0;
            for (const auto value : v->Values)
            {
                summation += sin(100.0 * value);
            }

            v->Z = v->Values[0] + 2.0 * v->Values[1] + 2.0 * v->Values[2] + v->Values[3] - 5.0 * v->Values[4]
                - 5.0 * v->Values[5] + 0.001 * summation;
            return v->Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getLogNormalStochast(120.0, 12.0));
        stochast.push_back(projectBuilder::getLogNormalStochast(120.0, 12.0));
        stochast.push_back(projectBuilder::getLogNormalStochast(120.0, 12.0));
        stochast.push_back(projectBuilder::getLogNormalStochast(120.0, 12.0));
        stochast.push_back(projectBuilder::getLogNormalStochast(50.0, 15.0));
        stochast.push_back(projectBuilder::getLogNormalStochast(40.0, 12.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaartsNoisyLimitState::ExpectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 2.3;
        expected.beta_margin = 0.1;
        return expected;
    }

    WaartsResult TestWaartsNoisyLimitState::ExpectedValuesCrudeMonteCarlo()
    {
        auto expected = ExpectedValues();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsNoisyLimitState::ExpectedValuesImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsNoisyLimitState::ExpectedValuesAdaptiveImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.success = false;
        return expected;
    }

}
