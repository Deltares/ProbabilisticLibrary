#pragma once

#include "FORMSettings.h"
#include "DirectionalSamplingSettings.h"
#include "ReliabilityMethod.h"

namespace Deltares
{
	namespace Reliability
	{
		class DSFI : public ReliabilityMethod
		{
		public:
			std::shared_ptr<FORMSettings> formSettings = std::make_shared<FORMSettings>();
			std::shared_ptr<DirectionalSamplingSettings> DsSettings = std::make_shared<DirectionalSamplingSettings>();
			std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) override;
		};
	}
}
