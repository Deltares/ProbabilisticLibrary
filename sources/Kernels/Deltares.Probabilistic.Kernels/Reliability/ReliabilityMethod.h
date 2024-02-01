#pragma once
#include "../Model/DesignPoint.h"
#include "../Model/ZModelRunner.h"
#include "../Model/ConvergenceReport.h"

namespace Deltares
{
	namespace Reliability
	{
		class ReliabilityMethod
		{
		private:
			bool stopped = false;

		protected:

			static int getZFactor(double z);

		public:
			virtual std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ZModelRunner> modelRunner) { return nullptr; }
			virtual ~ReliabilityMethod() = default;

			bool isStopped();
			void Stop();
		};
	}
}

