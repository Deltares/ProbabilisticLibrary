#pragma once
#include <memory>
#include <vector>

#include "Distribution.h"
#include "../../Model/ModelSample.h"

namespace Deltares
{
    namespace Statistics
    {
        class DistributionFitter
        {
        public:
            std::vector<double> fitByLogLikelihood(std::vector<double>& values, Distribution* distribution, std::shared_ptr<StochastProperties> stochast, std::vector<double>& minimum, std::vector<double>& maximum, std::vector<double>& initial, std::vector<DistributionPropertyType>& properties);
        private:
            std::shared_ptr<StochastProperties> stochast = nullptr;
            std::vector<double> values;
            std::vector<DistributionPropertyType> properties;
            Distribution* distribution = nullptr;
            void getLogLikelihood(std::shared_ptr<Models::ModelSample> sample);
            double getSumLogLikelihood();
        };
    }
}

