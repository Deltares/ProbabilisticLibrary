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

#include "TestWaartsResistanceOneQuadraticTerm.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsResistanceOneQuadraticTerm::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            v->Z = v->Values[0] - pow(v->Values[1], 2);
            return v->Z;
        });

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochasts.push_back(projectBuilder::getNormalStochast(11.0, 1.0));
        stochasts.push_back(projectBuilder::getNormalStochast(1.5, 0.5));
        return getModelRunner(z, stochasts);
    }

    WaartsResult TestWaartsResistanceOneQuadraticTerm::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 3.45;
        expected.alpha = { 0.358979, -0.933345 };
        expected.alpha_margin = 0.1;
        expected.x = { 9.95, 3.15 };
        return expected;
    }

    WaartsResult TestWaartsResistanceOneQuadraticTerm::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.alpha.clear();
        expected.x.clear();
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsResistanceOneQuadraticTerm::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.converged = false;
        return expected;
    }
}
