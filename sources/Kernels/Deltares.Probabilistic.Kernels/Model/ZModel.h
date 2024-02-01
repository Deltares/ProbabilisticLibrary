#pragma once
#include <limits>
#include <vector>
#include <functional>

#include "Sample.h"

namespace Deltares
{
	namespace Models
	{
		typedef std::function<void(std::shared_ptr<Sample>)> ZLambda;
		typedef std::function<void(std::vector<std::shared_ptr<Sample>>)> ZMultipleLambda;

		class ZModel
		{
		private:
			ZLambda zLambda = nullptr;
			ZMultipleLambda zMultipleLambda = nullptr;
			int maxProcesses = 1;

		public:
			ZModel(ZLambda zLambda, ZMultipleLambda zMultipleLambda = nullptr)
			{
				this->zLambda = zLambda;
				this->zMultipleLambda = zMultipleLambda;
			}

			void setMaxProcesses(int maxProcesses);

			void invoke(std::shared_ptr<Sample> sample);

			void invoke(std::vector<std::shared_ptr<Sample>> samples);
		};
	}
}

