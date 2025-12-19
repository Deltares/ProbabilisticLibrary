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

#include "TestWaartsOblateSpheroid.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsOblateSpheroid::WaartsModel()
    {
        constexpr int numberOfQuadraticTerms = 10;
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            double Z = v->Values[0];
            for (int i = 1; i <= numberOfQuadraticTerms; i++)
            {
                Z -= pow(v->Values[i], 2) / (1.0 + static_cast<double>(i) / 10.0);
            }
            v->Z = Z;
            return Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(10.0, 0.5));
        for (int i = 1; i <= numberOfQuadraticTerms; i++)
        {
            stochast.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        }
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaartsOblateSpheroid::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 1.1;
        return expected;
    }

    WaartsResult TestWaartsOblateSpheroid::expectedValuesFORM()
    {
        auto expected = expectedValues();
        expected.beta = 3.38;
        return expected;
    }

    WaartsResult TestWaartsOblateSpheroid::expectedValuesFDIR()
    {
        return expectedValuesFORM();
    }

    WaartsResult TestWaartsOblateSpheroid::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsOblateSpheroid::expectedValuesAdaptiveImportanceSampling()
    {
        return expectedValuesImportanceSampling();
    }

}
