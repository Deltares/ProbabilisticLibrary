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
			std::shared_ptr<Sample> examineSurfaceForFailure(std::shared_ptr<Models::ModelRunner> modelRunner, int index, double radiusFactor, std::shared_ptr<Sample> uRay, double z0Fac, double& zMin);
			std::shared_ptr<Sample> getBestSample(std::shared_ptr<Sample> bestSample, std::shared_ptr<Sample> sample, double z0Fac);
			std::shared_ptr<Sample> refineSpherePoint(std::shared_ptr<Models::ModelRunner> modelRunner, double radiusFactor, std::shared_ptr<Sample> u);
		};
	}
}

