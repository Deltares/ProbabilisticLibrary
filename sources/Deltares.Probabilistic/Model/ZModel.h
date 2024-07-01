#pragma once
#include <limits>
#include <vector>
#include <functional>

#include "ModelSample.h"
#include "RunSettings.h"

namespace Deltares
{
	namespace Models
	{
		typedef std::function<void(std::shared_ptr<ModelSample>)> ZLambda;
		typedef std::function<void(std::vector<std::shared_ptr<ModelSample>>)> ZMultipleLambda;
		typedef std::function<double(std::shared_ptr<ModelSample>, double beta)> ZBetaLambda;

		class ZModel
		{
		private:
			ZLambda zLambda = nullptr;
			ZMultipleLambda zMultipleLambda = nullptr;
			ZBetaLambda zBetaLambda = nullptr;
			int maxProcesses = 1;

		public:
			ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr)
			{
				this->zLambda = zLambda;
				this->zMultipleLambda = zMultipleLambda;
			}

			void setBetaLambda(ZBetaLambda zBetaLambda)
			{
				this->zBetaLambda = zBetaLambda;
			}

			void setMaxProcesses(int maxProcesses);

			void invoke(std::shared_ptr<ModelSample> sample);

			void invoke(std::vector<std::shared_ptr<ModelSample>> samples);

            void invoke(std::shared_ptr<ModelSample> sample, const designPointOptions dpOption, const int loggingCounter);

			double getBeta(std::shared_ptr<ModelSample> sample, double beta);

			bool canCalculateBeta()
			{
				return this->zBetaLambda != nullptr;
			}
        };
	}
}

