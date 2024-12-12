// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include "DirectionalSamplingSettingsS.h"
#include "SensitivityMethod.h"

namespace Deltares
{
    namespace Sensitivity
    {
        /**
         * \brief Calculates the sensitivity using the Directional Sampling algorithm
         */
        class DirectionalSamplingS : public SensitivityMethod
        {
        public:
            /**
             * \brief Settings for this algorithm
             */
            std::shared_ptr<DirectionalSamplingSettingsS> Settings = std::make_shared<DirectionalSamplingSettingsS>();

            /**
             * \brief Gets the sensitivity
             * \param modelRunner The model for which the sensitivity is calculated
             * \return The sensitivity in the form of a stochastic variable
             */
            std::shared_ptr<Statistics::Stochast> getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            class Result
            {
            public:
                double Distance;
                double ZValue;
            };

            class Direction
            {
            public:
                Direction(std::shared_ptr<Models::Sample> sample) { this->sample = sample; }
                void AddResult(double distance, double z);
                double GetDistanceAtZ(double z) const;
                std::shared_ptr<Models::Sample> CreateNewSampleAt(double z, double maxBeta);
                bool Valid = true;
            private:
                std::shared_ptr<Models::Sample> sample;
                std::vector<double> distances = std::vector<double>();
                std::vector<double> zValues = std::vector<double>();
                std::shared_ptr<Models::Sample> newSample = nullptr;
            };

            double getZForRequiredQ(Models::ModelRunner& modelRunner, double requestedBeta, int nStochasts, double Z0) const;
            static double predict(double predZi, const std::vector<std::shared_ptr<Direction>>& directions, double probability0, int nStochasts);
            static double calculateProbabilityOfFailure(const std::vector<double>& dValues, double nstochasts);
            static double getBetaDistance(double betaRequired, int nStochasts);
            static double calculateError(std::vector<double>& zValues, std::vector<double>& newZValues);
        };
    }
}

