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
        typedef std::function<void(std::shared_ptr<ModelSample>, const designPointOptions dpOption)> ZLambdaDesignPoint;

		class ZModel
		{
		private:
			ZLambda zLambda = nullptr;
			ZMultipleLambda zMultipleLambda = nullptr;
			ZBetaLambda zBetaLambda = nullptr;
            ZLambdaDesignPoint zLambdaDp = nullptr;
			int maxProcesses = 1;

		public:
			ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr, ZLambdaDesignPoint z_lambda_design_point = nullptr)
			{
				this->zLambda = zLambda;
				this->zMultipleLambda = zMultipleLambda;
                this->zLambdaDp = z_lambda_design_point;
			}

			void setBetaLambda(ZBetaLambda zBetaLambda)
			{
				this->zBetaLambda = zBetaLambda;
			}

			void setMaxProcesses(int maxProcesses);

			void invoke(std::shared_ptr<ModelSample> sample);

			void invoke(std::vector<std::shared_ptr<ModelSample>> samples);

            void invoke(std::shared_ptr<ModelSample> sample, const designPointOptions dpOption);

			double getBeta(std::shared_ptr<ModelSample> sample, double beta);

			bool canCalculateBeta()
			{
				return this->zBetaLambda != nullptr;
			}
        };
	}
}

