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

#include "PrecomputeValues.h"
#include "ZGetter.h"

namespace Deltares::Reliability
{
    class DirectionCalculationSettings
    {
    public:
        DirectionCalculationSettings(bool inverted) : inverted(inverted) {}
        DirectionCalculationSettings(bool inverted, double Dsdu, double maxLengthU) :
            inverted(inverted), Dsdu(Dsdu), MaximumLengthU(maxLengthU) {}
        const bool inverted;
        const double Dsdu = std::nan("");
        const double MaximumLengthU = std::nan("");
    };

    class DirectionCalculation
    {
    public:
        Sample& uDirection;
        const ZGetter& model;
        const DirectionCalculationSettings& settings;

        DirectionCalculation(ZGetter& model, Sample& uDirection, const DirectionCalculationSettings& settings)
            : uDirection(uDirection), model(model), settings(settings)
        {
        }

        double GetZProxy(double u, bool allowProxy) const
        {
            return model.GetZ(uDirection, u, settings.inverted, allowProxy);
        }

        double GetZ(double u) const
        {
            return model.GetZ(uDirection, u, settings.inverted, true);
        }

        double GetZ(size_t index, const PrecomputeValues& zValues)  const
        {
            const auto [foundZ0, z0] = zValues.findZ(index);
            if (foundZ0) return z0;

            double factor = std::min(static_cast<double>(index) * settings.Dsdu, settings.MaximumLengthU);
            return model.GetZ(uDirection, factor, settings.inverted, true);
        }

        double GetZNoProxy(double u) const
        {
            return model.GetZ(uDirection, u, settings.inverted, false);
        }
    };
}

