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

#include "TestWaartsParallelSystem.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../../Deltares.Probabilistic/Math/NumericSupport.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsParallelSystem::WaartsModel()
    {
        constexpr int nrStochasts = 5;
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            const double u1 = v->Values[0];
            const double u2 = v->Values[1];
            const double u3 = v->Values[2];
            const double u4 = v->Values[3];
            const double u5 = v->Values[4];

            const double g1 = 2.677 - u1 - u2;
            const double g2 = 2.500 - u2 - u3;
            const double g3 = 2.323 - u3 - u4;
            const double g4 = 2.250 - u4 - u5;
            const double parallelSystem = Numeric::NumericSupport::getMaximum({ g1, g2, g3, g4 });

            v->Z = parallelSystem;
            return parallelSystem;
        });

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        for(int i = 0; i < nrStochasts; i++)
        {
            stochasts.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        }
        return getModelRunner(z, stochasts);
    }

    WaartsResult TestWaartsParallelSystem::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 3.5;
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsParallelSystem::expectedValuesFORM()
    {
        auto expected = expectedValues();
        expected.beta = 1.64;
        return expected;
    }

    WaartsResult TestWaartsParallelSystem::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.beta = 3.29;
        return expected;
    }

}
