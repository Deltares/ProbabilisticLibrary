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

#include "SubsetSimulationSettings.h"
#include "ReliabilityMethod.h"
#include "../Model/RandomSampleGenerator.h"

namespace Deltares
{
    namespace Reliability
    {
        /**
         * \brief Subset simulation algorithm
         */
        class SubsetSimulation : public ReliabilityMethod
        {
        public:
            std::shared_ptr<SubsetSimulationSettings> Settings = std::make_shared<SubsetSimulationSettings>();
            std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        private:
            std::vector<std::shared_ptr<Sample>> getInitialSamples(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<RandomSampleGenerator> randomSampleGenerator, bool initial);
            std::vector<std::shared_ptr<Sample>> getMarkovChainSamples(std::shared_ptr<ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>>& selectedSamples, double z0Fac);
            std::shared_ptr<Sample> getMarkovChainSample(std::shared_ptr<Sample> oldSample, std::shared_ptr<ModelRunner> modelRunner, double maxZ, double z0Fac);
            std::vector<std::shared_ptr<Sample>> getAdaptiveConditionalSamples(std::shared_ptr<ModelRunner> modelRunner, std::vector<std::shared_ptr<Sample>>& selectedSamples);

            std::vector<std::shared_ptr<Sample>> getNewSamples(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<RandomSampleGenerator> randomSampleGenerator, bool initial, double z0Fac, std::vector<std::shared_ptr<Sample>> selectedSamples);
            std::vector<std::shared_ptr<Sample>> selectSamples(double z0Fac, std::vector<std::shared_ptr<Sample>> performedSamples);
            static double getConvergence(double pf, int samples);
            bool isConverged(int sampleIndex, double convergence) const;

            double getStandardNormalPDF(double u);

            int rejectedSamples = 0;
            int acceptedSamples = 0;
            double acceptanceRate = 0.0;
            std::shared_ptr<RandomSampleGenerator> randomSampleGenerator;
        };
    }
}





