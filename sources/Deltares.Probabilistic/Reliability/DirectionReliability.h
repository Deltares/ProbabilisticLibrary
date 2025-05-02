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
#include "BetaValueTask.h"
#include "DirectionReliabilitySettings.h"
#include "DirectionSection.h"
#include "DirectionSectionsCalculation.h"
#include "ReliabilityMethod.h"
#include <memory>

namespace Deltares::Reliability
{
    class DirectionReliability : public ReliabilityMethod
    {
    public:
        DirectionReliability() = default;
        DirectionReliability(std::shared_ptr<DirectionSectionsCalculation> sectionsCalc) : sectionsCalc(std::move(sectionsCalc)) {}
        std::shared_ptr<DirectionReliabilitySettings> Settings = std::make_shared<DirectionReliabilitySettings>();
        std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        virtual double getBeta(Models::ModelRunner& modelRunner, Sample& directionSample, double z0);
        virtual double getBeta(Models::ModelRunner& modelRunner, double z0) { return 0.0; }
    protected:
        double getDirectionBeta(Models::ModelRunner& modelRunner, const BetaValueTask& directionTask) const;
        std::shared_ptr<DirectionSectionsCalculation> sectionsCalc;
    };

}


