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

#include "TestWaartsSeriesSystem.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../../Deltares.Probabilistic/Math/NumericSupport.h"
#include <numbers>

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsSeriesSystem::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            const double u1 = v->Values[0];
            const double u2 = v->Values[1];

            double g1 = 0.1 * pow(u1 - u2,2) - (u1 + u2) / std::numbers::sqrt2 + 3.0;
            double g2 = 0.1 * pow(u1 - u2,2) + (u1 + u2) / std::numbers::sqrt2 + 3.0;
            double g3 =  u1 - u2 + 3.5 * std::numbers::sqrt2;
            double g4 = -u1 + u2 + 3.5 * std::numbers::sqrt2;
            double seriesSystem = Numeric::NumericSupport::getMinimum({ g1, g2, g3, g4 });

            v->Z = seriesSystem;
            return seriesSystem;
        });

        auto stochasts = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochasts.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        stochasts.push_back(projectBuilder::getNormalStochast(0.0, 1.0));
        return getModelRunner(z, stochasts);
    }

    WaartsResult TestWaartsSeriesSystem::expectedValues()
    {
        // note that the alpha and x values of the design point of this case strongly depends on the method used.
        auto expected = WaartsResult();
        expected.beta = 2.84; // value found with numerical integration
        expected.beta_margin = 0.01;
        expected.alpha_margin = 0.01;
        expected.x_margin = 0.01;
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesFORM()
    {
        auto expected = expectedValues();
        expected.beta = 3.00;
        expected.alpha = { -sqrt(0.5), -sqrt(0.5) };
        expected.x = {2.12146, 2.12146};
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesFDIR()
    {
        return expectedValuesFORM();
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.beta = 2.88;
        expected.alpha = {0.670837, 0.741605};
        expected.x = {-1.93078, -2.13446};
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesDirectionalSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.89;
        expected.alpha = {0.742148, 0.670236};
        expected.x = {-2.14284, -1.93521};
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesDSFI()
    {
        auto expected = expectedValuesDirectionalSampling();
        expected.alpha ={0.717144, 0.696925};
        expected.x = {-2.15094, -2.0903};
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesNumericalIntegration()
    {
        auto expected = expectedValues();
        expected.alpha = {-0.259111, -0.965847};
        expected.x = {0.736722, 2.74616};
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.85;
        expected.alpha = {0.579354, 0.815076};
        expected.x = { -1.65229, -2.32456 };
        expected.converged = false;
        return expected;
    }

    WaartsResult TestWaartsSeriesSystem::expectedValuesAdaptiveImportanceSampling()
    {
        auto expected = expectedValues();
        expected.beta = 2.33;
        expected.alpha = { -0.602854, -0.797851};
        expected.x = {1.40778, 1.86313};
        expected.converged = false;
        return expected;
    }

}
