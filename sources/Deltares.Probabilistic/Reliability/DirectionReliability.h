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
#include "DirectionCalculation.h"
#include "DirectionReliabilitySettings.h"
#include "DirectionSection.h"
#include "PrecomputeValues.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
    namespace Reliability
    {
        class BetaValueTask
        {
        public:
            std::shared_ptr<Sample> UValues;
            std::shared_ptr<DirectionReliabilitySettings> Settings;
            double z0 = 0.0;
        };

        class DirectionReliability : public ReliabilityMethod
        {
        public:
            std::shared_ptr<DirectionReliabilitySettings> Settings = std::make_shared<DirectionReliabilitySettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
            double getBeta(Models::ModelRunner& modelRunner, std::shared_ptr<Sample> directionSample, double z0,
                const PrecomputeValues& zValues);
            double getBeta(Models::ModelRunner& modelRunner, std::shared_ptr<Sample> directionSample, double z0);
            static double GetZTolerance(const DirectionReliabilitySettings& settings, double uLow, double uHigh, double zLow, double zHigh);
        protected:
            double findBetaBetweenBoundariesAllowNaN(const DirectionCalculation& directionCalculation,
                double uLow, double uHigh, double zLow, double zHigh, double& z);
            virtual double findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
                const DirectionCalculation& directionCalculation,
                double uLow, double uHigh, double zLow, double zHigh, double& z);
        private:
            double getDirectionBeta(Models::ModelRunner& modelRunner, const BetaValueTask& directionTask,
                const PrecomputeValues& zValues);
            std::vector<DirectionSection> getDirectionSections(Models::ModelRunner& modelRunner,
                std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, const PrecomputeValues& zValues);
            double getBetaFromSections(const std::vector<DirectionSection>& sections) const;
        };

        class DirectionReliabilityForDirectionalSampling : public DirectionReliability
        {
        public:
            double Threshold = 0;
        protected:
            double findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
                const DirectionCalculation& directionCalculation,
                double uLow, double uHigh, double zLow, double zHigh, double& z) override;
        private:
            static bool isProxyAllowed(double ThresholdOffset, double u, double threshold);
        };
    }
}

