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

#include "TestWaartsConvexFailureDomain.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

#include <numbers>

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsConvexFailureDomain::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            const double u1 = v->Values[0];
            const double u2 = v->Values[1];
            v->Z = 0.1 * pow(u1 - u2, 2) - (u1 + u2) / std::numbers::sqrt2 + 2.5;
            return v->Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        stochast.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 2.6;
        expected.alpha = { -sqrt(0.5), -sqrt(0.5) };
        return expected;
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValuesFORM()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.50;
        return expected;
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValuesFDIR()
    {
        return ExpectedValuesFORM();
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValuesCrudeMonteCarlo()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.69;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValuesAdaptiveImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.beta = 2.67;
        return expected;
    }

    WaartsResult TestWaartsConvexFailureDomain::ExpectedValuesImportanceSampling()
    {
        auto expected = ExpectedValues();
        expected.success = false;
        return expected;
    }

}
