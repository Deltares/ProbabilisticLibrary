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

#include "TestWaartsResistanceTenQuadraticTerms.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsResistanceTenQuadraticTerms::WaartsModel()
    {
        constexpr int numberOfQuadraticTerms = 10;
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {

            double Z = v->Values[0];
            for(int i = 1; i <= numberOfQuadraticTerms; i++)
            {
                Z -= pow(v->Values[i], 2) / static_cast<double>(i);
            }
            v->Z = Z;
            return Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(0.5, 0.1));
        for(int i = 0; i < numberOfQuadraticTerms; i++)
        {
            stochast.push_back(projectBuilder::getNormalStochast(0.2, 0.1));
        }
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 3.20;
        expected.alpha = { 0.757538, -0.586958, -0.20787, -0.125312, -0.0896262, -0.0697503, -0.0570874, -0.0483151, -0.0418794, -0.0369565 , -0.0330692 };
        return expected;
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.beta = 3.04;
        expected.alpha = { 0.679947, -0.622061, -0.165165, -0.0979174, -0.201122, 0.00966454, -0.0959883, -0.08764, -0.0737432, -0.0224048, -0.224647 };
        expected.alpha_margin = 0.1;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValuesDirectionalSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.98;
        expected.alpha_margin = 0.1;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValuesImportanceSampling()
    {
        return expectedValuesDirectionalSampling();
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValuesAdaptiveImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.95;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistanceTenQuadraticTerms::expectedValuesDSFI()
    {
        auto expected = expectedValues();
        expected.beta = 2.98;
        expected.success = false;
        return expected;
    }
}
