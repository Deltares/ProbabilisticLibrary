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

#include "TestWaartsSaddleSurface.h"
#include "../../projectBuilder.h"
#include "../../../Deltares.Probabilistic/Model/ModelSample.h"
#include "../../../Deltares.Probabilistic/Model/ZModel.h"
#include "../../../Deltares.Probabilistic/Statistics/Stochast.h"
#include "../../../Deltares.Probabilistic/Utils/probLibException.h"
#include <numbers>

namespace Deltares::Probabilistic::Test
{
    std::shared_ptr<Models::ModelRunner> TestWaartsSaddleSurface::WaartsModel()
    {
        auto z = std::make_shared<Models::ZModel>([](std::shared_ptr<Models::ModelSample> v)
        {
            v->Z = 3.0 - v->Values[0] * v->Values[1];
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

    void TestWaartsSaddleSurface::WaartsFORM()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORM>();
        auto form = dynamic_cast<Reliability::FORM*>(calculator.get());
        form->Settings->StartPointSettings->StartMethod = Reliability::StartMethodType::FixedValue;
        form->Settings->StartPointSettings->startVector = { 1.0, 1.0 };
        auto expected = ExpectedValuesFORM();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    void TestWaartsSaddleSurface::WaartsFDIR()
    {
        auto modelRunner = WaartsModel();
        std::unique_ptr<Reliability::ReliabilityMethod> calculator = std::make_unique<Reliability::FORMThenDirectionalSampling>();
        auto form = dynamic_cast<Reliability::FORMThenDirectionalSampling*>(calculator.get());
        form->formSettings->StartPointSettings->StartMethod = Reliability::StartMethodType::FixedValue;
        form->formSettings->StartPointSettings->startVector = { 1.0, 1.0 };
        auto expected = ExpectedValuesFORM();
        RunSingleWaartsTest(modelRunner, calculator, expected);
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValues()
    {
        throw Reliability::probLibException("Must be implemented for all reliability methods (too much differences between methods)");
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesFORM()
    {
        auto expected = WaartsResult();
        expected.beta = 2.45;
        expected.alpha = { -sqrt(0.5), -sqrt(0.5) };
        expected.x = { std::numbers::sqrt3,  std::numbers::sqrt3 };
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesFDIR()
    {
        return ExpectedValuesFORM();
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesNumericalIntegration()
    {
        auto expected = WaartsResult();
        expected.beta = 2.35;
        expected.alpha = { -sqrt(0.5), -sqrt(0.5) };
        expected.x = { 1.65, 1.65 };
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesCrudeMonteCarlo()
    {
        auto expected = WaartsResult();
        expected.beta = 2.34;
        expected.alpha = { 0.823269, 0.567651 };
        expected.x = {-1.92779, - 1.32923};
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesImportanceSampling()
    {
        auto expected = WaartsResult();
        expected.beta = 2.35;
        expected.alpha = { -0.754939, -0.655795 };
        expected.x = {1.77368, 1.54074};
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesAdaptiveImportanceSampling()
    {
        auto expected = WaartsResult();
        expected.beta = 2.38;
        expected.alpha = {-0.804692,-0.593693};
        expected.x = {1.91793, 1.41503};
        expected.success = false;
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesDSFI()
    {
        auto expected = WaartsResult();
        expected.beta = 2.34;
        expected.alpha = {0.71567, 0.698439};
        expected.x = {-1.75277, -1.71057};
        return expected;
    }

    WaartsResult TestWaartsSaddleSurface::ExpectedValuesDirectionalSampling()
    {
        auto expected = WaartsResult();
        expected.beta = 2.34;
        expected.alpha = {0.110093, 0.993921};
        expected.x = {-0.257192, -2.32193};
        return expected;
    }

}
