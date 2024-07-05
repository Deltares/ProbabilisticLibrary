#pragma once
#include "StartPointCalculatorSettings.h"
#include "../Model/Sample.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Reliability {

        class StartPointCalculator
        {
        public:
            std::shared_ptr<StartPointCalculatorSettings> Settings = std::make_shared<StartPointCalculatorSettings>();
            std::shared_ptr<Sample> getStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
        private:
            std::shared_ptr<Sample> getOneStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getRayStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getSensitivityStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getSphereStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);
            std::shared_ptr<Sample> getGivenVectorStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner);

            std::shared_ptr<Sample> getDirectionStartPoint(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> startPoint);
            void correctDefaultValues(std::shared_ptr<Sample> startPoint);
            void setBestSample(std::shared_ptr<Sample> & bestSample, const std::shared_ptr<Sample> sample);
            std::shared_ptr<Sample> refineSpherePoint(const std::shared_ptr<Sample> u, const std::shared_ptr<Sample> previous);
            std::vector<double> getGradient(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> sample);
        };
    }
}

