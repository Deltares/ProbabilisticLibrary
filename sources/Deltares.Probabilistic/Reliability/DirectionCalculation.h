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

#include "ZGetter.h"

namespace Deltares::Reliability
{
    class DirectionCalculation
    {
        std::shared_ptr<Models::ModelRunner> modelRunner;
        std::shared_ptr<Sample> uDirection;
        bool inverted;
        ZGetter model;

    public:
        DirectionCalculation(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> uDirection, bool inverted)
            : model(ZGetter(modelRunner))
        {
            this->modelRunner = modelRunner;
            this->uDirection = uDirection;
            this->inverted = inverted;
        }

        double GetZProxy(double u, bool allowProxy)
        {
            return model.GetZ(uDirection, u, inverted, allowProxy);
        }

        double GetZ(double u)
        {
            return model.GetZ(uDirection, u, inverted, true);
        }

        double GetZNoProxy(double u)
        {
            return model.GetZ(uDirection, u, inverted, false);
        }
    };
}

