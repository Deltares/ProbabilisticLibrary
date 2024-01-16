#pragma once

#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "DesignPointBuilder.h"
#include "StochastSettingsSet.h"

namespace Deltares
{
    namespace Reliability
	{
        class CrudeMonteCarloSettings
        {
        public:
            int MinimumSamples = 1000;
            int MaximumSamples = 10000;
            double VariationCoefficient = 0.05;
            DesignPointMethod DesignPointMethod = DesignPointMethod::CenterOfGravity;
            RandomSettings* RandomSettings = new ::RandomSettings();
            RunSettings* RunSettings = new ::RunSettings();
            StochastSettingsSet* StochastSet = new StochastSettingsSet();
        };
    }
}

