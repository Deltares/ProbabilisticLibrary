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

#include "TestWaartsResistance25QuadraticTerms.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsResistance25QuadraticTerms::WaartsModel()
    {
        constexpr int numberOfQuadraticTerms = 25;
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

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochasts.push_back(projectBuilder::getNormalStochast(0.5, 0.1));
        auto st = projectBuilder::getNormalStochast(0.2, 0.1);
        st->modelParameter->isArray = true;
        st->modelParameter->arraySize = numberOfQuadraticTerms;
        stochasts.push_back(st);
        return getModelRunner(z, stochasts);
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 2.92;
        expected.alpha = { 0.771, -0.564, -0.207, -0.126, -0.090, -0.070, -0.058, -0.049, -0.042, -0.037, -0.0335, -0.030, -0.028, -0.026 };
        while (expected.alpha.size() < 26)
        {
            expected.alpha.push_back(-0.02);
        }
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.beta = 2.67;
        expected.alpha.clear();
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValuesDirectionalSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.64;
        expected.alpha_margin = 0.1;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.61;
        expected.alpha.clear();
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValuesAdaptiveImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.56;
        expected.alpha_margin = 0.2;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsResistance25QuadraticTerms::expectedValuesDSFI()
    {
        auto expected = expectedValues();
        expected.beta = 2.64;
        expected.converged = false;
        return expected;
    }
}
