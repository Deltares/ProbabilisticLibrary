#pragma once

#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "StochastListSettings.h"
#include "StochastSettings.h"

namespace Deltares
{
    namespace Reliability
	{
        class CrudeMonteCarloSettings : public StochastListSettings
        {
        public:
            int MinimumSamples = 1000;
            int MaximumSamples = 10000;
            double VariationCoefficient = 0.05;
            DesignPointMethod DesignPointMethod = DesignPointMethod::CenterOfGravity;
            RandomSettings* RandomSettings = new ::RandomSettings();
            RunSettings* RunSettings = new ::RunSettings();
        };
    }
}

