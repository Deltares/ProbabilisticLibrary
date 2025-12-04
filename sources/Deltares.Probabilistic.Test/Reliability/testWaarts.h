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

#pragma once

#include <memory>
#include <vector>

#include "../../Deltares.Probabilistic/Model/ModelRunner.h"
#include "../../Deltares.Probabilistic/Reliability/ReliabilityMethod.h"

namespace Deltares::Probabilistic::Test
{
    struct WaartsResult
    {
        double beta = 40.0;
        double beta_margin = 0.05;
        std::vector<double> alpha;
        double alpha_margin = 0.05;
        std::vector<double> x;
        double x_margin = 0.05;
        bool success = true;
    };

    class TestWaarts
    {
    public:
        virtual void WaartsFORM();
        virtual void WaartsCrudeMonteCarlo();
        virtual void WaartsDirectionalSampling();
        virtual void WaartsNumericalIntegration();
        virtual void WaartsImportanceSampling();
        virtual void WaartsAdaptiveImportanceSampling();
        virtual void WaartsFDIR();
        virtual void WaartsDSFI();
    protected:
        static void RunSingleWaartsTest(const std::shared_ptr<Models::ModelRunner>& modelRunner,
            const std::unique_ptr<Reliability::ReliabilityMethod>& calculator, const WaartsResult& expected);
        virtual std::shared_ptr<Models::ModelRunner> WaartsModel() = 0;
        virtual WaartsResult ExpectedValues() = 0;
        virtual WaartsResult ExpectedValuesFORM() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesCrudeMonteCarlo() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesDirectionalSampling() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesNumericalIntegration() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesImportanceSampling() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesAdaptiveImportanceSampling() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesFDIR() { return ExpectedValues(); }
        virtual WaartsResult ExpectedValuesDSFI() { return ExpectedValues(); }
    };
}
