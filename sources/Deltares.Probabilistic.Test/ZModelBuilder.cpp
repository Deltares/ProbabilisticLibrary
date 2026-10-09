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
#include "ZModelBuilder.h"

using namespace Deltares::Probabilistic::Test;

namespace Deltares::Probabilistic::Test
{
    Models::ZModel ZModelBuilder::getPolynomeModel(double offset1, double offset2)
    {
        Models::ZLambda function = [offset1, offset2](Models::ModelSample& sample)
        {
            sample.Z = 10.0 * std::pow(sample.Values[0] - offset1, 2) + std::pow(sample.Values[1] - offset2, 2);
        };

        Models::ZModel model = Models::ZModel(function);

        return model;
    }

    Models::ZModel ZModelBuilder::getConstrainedPolynomeModel()
    {
        Models::ZLambda function = [](Models::ModelSample& sample)
        {
            sample.Z = sample.Values[0] * sample.Values[1];
        };

        Models::ZModel model = Models::ZModel(function);

        Models::ZBetaLambda constraint = [](Models::ModelSample& sample)
        {
            double C = 1.0 - hypot(sample.Values[0], sample.Values[1]);
            return std::abs(C);
        };

        model.setConstraint(constraint);

        return model;
    }

    void ZmodelWithCenter::invoke(Models::ModelSample& sample)
    {
        sample.Z = 0.0;
        evaluations++;
        for (size_t i = 0; i < sample.Values.size(); i++)
        {
            sample.Z += pow(sample.Values[i] - center[i], 2);
        }
    }

}

