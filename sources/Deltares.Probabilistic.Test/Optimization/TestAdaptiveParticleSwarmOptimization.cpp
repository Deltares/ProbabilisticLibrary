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

#include "TestAdaptiveParticleSwarmOptimization.h"

#include <gtest/gtest.h>

#include "../../Deltares.Probabilistic/Optimization/AdaptiveParticleSwarmOptimization.h"
#include "../ZModelBuilder.h"

namespace Deltares::Optimization::Test
{
    void TestAdaptiveParticleSwarmOptimization::TestBeeswarm(bool move, double tolerance, int evaluations, const std::vector<double>& center)
    {
        auto apso = AdaptiveParticleSwarmOptimization();
        auto search_param_settings = SearchParameterSettings();
        search_param_settings.MinValue = 1;
        search_param_settings.MaxValue = 10;
        search_param_settings.Move = move;
        for (size_t i = 0; i < center.size(); i++)
        {
            apso.Options.SearchArea.Dimensions.push_back(std::make_shared<SearchParameterSettings>(search_param_settings));
        }
        apso.Options.CrossOver = 0.8;
        apso.Options.StopAfterNonImprovingGenerations = 5;
        auto model = Probabilistic::Test::ZmodelWithCenter(center);
        auto result = apso.getOptimizedSampleNew(model);

        for (size_t i = 0; i < center.size(); i++)
        {
            EXPECT_NEAR(result.values[i], center[i], tolerance);
        }
        EXPECT_LE(model.getEvaluations(), evaluations);
    }
}

