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

#include "TestWaartsLinearResistance.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsLinearResistance::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            v->Z = v->Values[0] - v->Values[1];
            return v->Z;
        });

        auto stochast = std::vector<std::shared_ptr<Statistics::Stochast>>();
        stochast.push_back(projectBuilder::getNormalStochast(7.0, 1.0));
        stochast.push_back(projectBuilder::getNormalStochast(2.0, 1.0));
        auto corr = std::make_shared<Statistics::CorrelationMatrix>();
        auto uConverter = std::make_shared <Models::UConverter>(stochast, corr);
        uConverter->initializeForRun();
        auto modelRunner = std::make_shared<Models::ModelRunner>(z, uConverter);
        return modelRunner;
    }

    void TestWaartsLinearResistance::runNumInt(const Reliability::DesignPointMethod method)
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::NumericalIntegration>();
        auto form = dynamic_cast<Reliability::NumericalIntegration*>(calculator.get());
        form->Settings.designPointMethod = method;
        auto expected = expectedValuesNumericalIntegration();
        expected.success = false;
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    WaartsResult TestWaartsLinearResistance::expectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 3.54;
        expected.alpha = { sqrt(0.5), -sqrt(0.5) };
        expected.x = { 4.5, 4.5 };
        return expected;
    }

    WaartsResult TestWaartsLinearResistance::expectedValuesCrudeMonteCarlo()
    {
        auto expected = expectedValues();
        expected.alpha_margin = 0.15;
        expected.x_margin = 0.5;
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsLinearResistance::expectedValuesImportanceSampling()
    {
        auto expected = expectedValues();
        expected.x_margin = 0.1;
        expected.success = false;
        return expected;
    }

}
