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
#include "TestProxies.h"

#include "../projectBuilder.h"

#include "../../Deltares.Probabilistic/Proxies/ProxyModel.h"
#include "../../Deltares.Probabilistic/Proxies/LinearProxyMethod.h"

#include <gtest/gtest.h>

namespace Deltares::Probabilistic::Test
{

    void TestProxies::testLinearModel() const
    {
        std::shared_ptr<Models::ModelRunner> modelRunner = projectBuilder::BuildLinearProject();

        std::shared_ptr<Models::Sample> sample = std::make_shared<Models::Sample>(std::vector<double> {1.0, 0.5});

        Models::Evaluation eval1 = modelRunner->getEvaluation(sample);

        modelRunner->useProxy(true);
        modelRunner->ProxySettings->InitializationType = Proxies::ProxyInitializationType::Single;
        modelRunner->ProxySettings->MethodType = Proxies::ProxyMethodType::FirstOrder;

        modelRunner->initializeForRun();

        Models::Evaluation eval2 = modelRunner->getEvaluation(sample);

        for (size_t i = 0; i < eval1.OutputValues.size(); i++)
        {
            // EXPECT_NEAR(eval1.OutputValues[i], eval2.OutputValues[i], margin);
        }

        EXPECT_NEAR(eval1.Z, eval2.Z, margin);
    }

}

