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

#include "TestWaartsResistance25QuadraticTermsSparse.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsResistance25QuadraticTermsSparse::WaartsModel()
    {
        constexpr int numberOfQuadraticTerms = 25;
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            double Z = v->Values[29];
            for(int i = 1; i <= numberOfQuadraticTerms; i++)
            {
                Z -= pow(v->Values[i+1], 2) / static_cast<double>(i);
            }
            v->Z = Z;
            return Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        for (int i = 0; i < 2; i++)
        {
            stochast.push_back(projectBuilder::getDeterministicStochast(0.0)); // not used stochast
        }
        for (int i = 0; i < numberOfQuadraticTerms; i++)
        {
            stochast.push_back(projectBuilder::getNormalStochast(0.2, 0.1));
        }
        for (int i = 0; i < 2; i++)
        {
            stochast.push_back(projectBuilder::getDeterministicStochast(0.0)); // not used stochast
        }
        stochast.push_back(projectBuilder::getNormalStochast(0.5, 0.1));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        corr->init(30);
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 2.92;
        expected.alpha = { 0.0, 0.0, -0.564, -0.207, -0.126, -0.090, -0.070, -0.058, -0.049, -0.042, -0.037, -0.0335, -0.030, -0.028, -0.026 };
        while (expected.alpha.size() < 27)
        {
            expected.alpha.push_back(-0.02);
        }
        expected.alpha.push_back(0.0);
        expected.alpha.push_back(0.0);
        expected.alpha.push_back(0.771);
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValuesCrudeMonteCarlo()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.56;
        expected.alpha.clear();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValuesDirectionalSampling()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.64;
        expected.alpha_margin = 0.1;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValuesImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.69;
        expected.alpha.clear();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValuesAdaptiveImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.56;
        expected.alpha_margin = 0.2;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTermsSparse::ExpectedValuesDSFI()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.64;
        expected.success = false;
        return expected;
    }
}
