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

#include "TestWaartsConcaveFailureDomain.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"

#include <numbers>

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsConcaveFailureDomain::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            const double u1 = v->Values[0];
            const double u2 = v->Values[1];
            v->Z = -0.5 * (u1 * u1 + u2 * u2 - 2.0 * u1 * u2) - (u1 + u2) / std::numbers::sqrt2 + 3.0;
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

    void TestWaartsConcaveFailureDomain::WaartsFORM()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORM>();
        auto form = dynamic_cast<Reliability::FORM*>(calculator.get());
        form->Settings->StartPointSettings->StartMethod = Reliability::RaySearch;
        form->Settings->StartPointSettings->startVector = { 1.0, 0.0 };
        auto expected = ExpectedValuesFORM();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaartsConcaveFailureDomain::WaartsFDIR()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORMThenDirectionalSampling>();
        auto form = dynamic_cast<Reliability::FORMThenDirectionalSampling*>(calculator.get());
        form->formSettings->StartPointSettings->StartMethod = Reliability::RaySearch;
        form->formSettings->StartPointSettings->startVector = { 1.0, 0.0 };
        auto expected = ExpectedValuesFORM();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    void TestWaartsConcaveFailureDomain::WaartsNumericalIntegration()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::NumericalIntegration>();
        auto numint = dynamic_cast<Reliability::NumericalIntegration*>(calculator.get());
        numint->Settings.designPointMethod = Reliability::NearestToMean;
        auto expected = ExpectedValuesNumericalIntegration();
        RunSingleWaartsTest(modelRunner, *calculator, expected);
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValues()
    {
        auto expected = WaartsResult();
        expected.beta = 1.26;
        expected.alpha = {-sqrt(0.5), -sqrt(0.5)};
        expected.alpha_margin = 0.1;
        return expected;
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValuesNumericalIntegration()
    {
        auto expected = ExpectedValues();
        expected.beta = 1.25;
        expected.alpha = { 0.46, -0.79 };
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValuesFORM()
    {
        auto expected = ExpectedValues();
        expected.beta = 1.66;
        expected.alpha.clear();
        return expected;
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValuesFDIR()
    {
        return ExpectedValuesFORM();
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValuesDSFI()
    {
        auto expected = ExpectedValues();
        expected.alpha = { 0.431047, -0.90233};
        expected.x = {-0.715352, 1.49748};
        expected.alpha_margin = 0.01;
        expected.x_margin = 0.01;
        return expected;
    }

    WaartsResult TestWaartsConcaveFailureDomain::ExpectedValuesDirectionalSampling()
    {
        auto expected = ExpectedValues();
        expected.alpha = {-0.496362, -0.868116};
        expected.x = {0.630058, 1.10195};
        expected.alpha_margin = 0.01;
        expected.x_margin = 0.01;
        return expected;
    }

}
