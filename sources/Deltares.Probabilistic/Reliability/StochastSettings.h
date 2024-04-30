#pragma once
#include <memory>

#include "../Statistics/StandardNormal.h"
#include "../Statistics/Stochast.h"

namespace Deltares
{
	namespace Reliability
	{
		class StochastSettings
		{
		public:
			double MinValue = -Statistics::StandardNormal::UMax;
			double MaxValue = Statistics::StandardNormal::UMax;
			double StartValue = 0;
			double UncorrelatedStartValue = 0;

			double VarianceFactor = 1.5;
			bool IsVarianceAllowed = true;

			int Intervals = 200;
			bool IsInitializationAllowed = true;

			int StochastIndex = 0;

			bool isMinMaxDefault();
			void initializeForRun();

			// Converted min and max values, is updated by initializeForRun
			double XMinValue = 0;
			double XMaxValue = 0;

			bool IsQualitative = false;

			std::shared_ptr<Statistics::Stochast> stochast = nullptr;

			double getRepresentativeU(double u);

			std::shared_ptr<StochastSettings> clone();
		};
	}
}

