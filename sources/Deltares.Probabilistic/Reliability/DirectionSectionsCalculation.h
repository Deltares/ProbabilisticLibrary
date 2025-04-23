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
#include "DirectionCalculation.h"
#include "DirectionReliabilitySettings.h"
#include "DirectionSection.h"
#include "PrecomputeValues.h"
#include "ReliabilityMethod.h"

namespace Deltares::Reliability
{
    class DirectionSectionsCalculation
    {
    public:
        DirectionSectionsCalculation(const DirectionReliabilitySettings& settings, const double z0) :
            z0(z0), Settings(settings) {}
        static double GetZTolerance(const DirectionReliabilitySettings& settings, double uLow, double uHigh, double zLow, double zHigh);
        std::vector<DirectionSection> getDirectionSections(Models::ModelRunner& modelRunner, const BetaValueTask& directionTask);
        static double getBetaFromSections(const std::vector<DirectionSection>& sections, const bool FindMinimalValue);
        bool CanPrecomputeSample() const;
        double GetPrecomputeUvalue() const;
        void ProvidePrecomputeValue(const PrecomputeValue& zValue);
    private:
        PrecomputeValues zValues;
        double z0;
    protected:
        double findBetaBetweenBoundariesAllowNaN(const DirectionCalculation& directionCalculation,
            double uLow, double uHigh, double zLow, double zHigh, double& z) const;
        virtual double findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
            const DirectionCalculation& directionCalculation,
            double uLow, double uHigh, double zLow, double zHigh, double& z) const;
        bool isStopped() const { return false; }
        const DirectionReliabilitySettings& Settings;
    };

}


