#pragma once

#include "../Reliability/DesignPoint.h"
#include "../Model/ModelSample.h"

namespace Deltares
{
    namespace Reliability
    {
        class DesignPointModel
        {
        public:
            bool inverted = false;
            bool negated = false;
            std::shared_ptr<DesignPoint> designPoint;

            void calculate(std::shared_ptr<ModelSample> sample);
            double getDirectionBeta(std::shared_ptr<ModelSample> sample);

            void setParameters(std::vector<std::shared_ptr<Statistics::Stochast>>& parameters);
        private:
            std::vector<double> alphas;
        };
    }
}

