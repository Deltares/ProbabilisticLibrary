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
            std::vector<std::shared_ptr<Sample>> getInitialSamples(std::shared_ptr<ModelRunner> modelRunner, std::shared_ptr<RandomSampleGenerator> randomSampleGenerator, int nstochasts, bool initial);
            std::vector<std::shared_ptr<Sample>> getMarkovChainSamples(std::shared_ptr<ModelRunner> modelRunner, int nstochasts, std::vector<std::shared_ptr<Sample>>& selectedSamples, double z0Fac);
            std::shared_ptr<Sample> getMarkovChainSample(std::shared_ptr<Sample> oldSample, std::shared_ptr<ModelRunner> modelRunner, double maxZ, double z0Fac);
            std::vector<std::shared_ptr<Sample>> getAdaptiveConditionalSamples(std::shared_ptr<ModelRunner> modelRunner, int nSamples, std::vector<std::shared_ptr<Sample>>& selectedSamples);

            double getConvergence(double pf, int samples);
            bool isConverged(std::shared_ptr<SubsetSimulationSettings> settings, int sampleIndex, double convergence);

            double getStandardNormalPDF(double u);

            int rejectedSamples = 0;
            int acceptedSamples = 0;
            double new_lambda = 0.0;
            double acceptanceRate = 0.0;
        };
    }
}





