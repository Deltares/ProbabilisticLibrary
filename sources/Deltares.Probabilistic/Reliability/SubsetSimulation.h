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

namespace Deltares::Reliability
{
    /**
     * \brief Subset simulation algorithm
     */
    class SubsetSimulation : public ReliabilityMethod
    {
    public:
        SubsetSimulationSettings Settings = SubsetSimulationSettings();
        std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
        bool isValid() override
        {
            return Settings.isValid();
        }

    private:
        std::vector<std::shared_ptr<Models::Sample>> getInitialSamples(const Models::ModelRunner& modelRunner, bool initial);
        std::vector<std::shared_ptr<Models::Sample>> getMarkovChainSamples(Models::ModelRunner& modelRunner, std::vector<std::shared_ptr<Models::Sample>>& selectedSamples, double z0Fac);
        std::shared_ptr<Models::Sample> getMarkovChainSample(std::shared_ptr<Models::Sample> oldSample, Models::ModelRunner& modelRunner, double maxZ, double z0Fac);
        std::vector<std::shared_ptr<Models::Sample>> getAdaptiveConditionalSamples(Models::ModelRunner& modelRunner, std::vector<std::shared_ptr<Models::Sample>>& selectedSamples);

        std::vector<std::shared_ptr<Models::Sample>> getNewSamples(Models::ModelRunner& modelRunner, bool initial, double z0Fac, std::vector<std::shared_ptr<Models::Sample>> selectedSamples);
        std::vector<std::shared_ptr<Models::Sample>> selectSamples(double z0Fac, std::vector<std::shared_ptr<Models::Sample>> performedSamples) const;
        static double getConvergence(double pf, int samples);
        bool isConverged(int sampleIndex, double convergence) const;

        static double getStandardNormalPDF(double u);

        int rejectedSamples = 0;
        int acceptedSamples = 0;
        double acceptanceRate = 0.0;
        Models::RandomSampleGenerator randomSampleGenerator = Models::RandomSampleGenerator();
    };
}

