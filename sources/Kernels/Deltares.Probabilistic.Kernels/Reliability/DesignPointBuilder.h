#pragma once
#include <unordered_map>

#include "../Model/Sample.h"
#include "../Model/ZModelRunner.h"

namespace Deltares
{
    namespace Reliability
    {
	    class ModeFinder;

	    enum DesignPointMethod
        {
            CenterOfGravity,
            CenterOfAngles,
            NearestToMean
        };

        class DesignPointBuilder
        {
        private:
            int count = 0;
            DesignPointMethod method = DesignPointMethod::NearestToMean;
            double rmin = std::numeric_limits<double>::infinity();
            double sumWeights = 0;
            Sample* defaultSample = nullptr;
            Sample* meanSample = nullptr;
            Sample* sinSample = nullptr;
            Sample* cosSample = nullptr;
            bool sampleAdded = false;

            int* qualitativeIndices;
            int qualitativeCount = 0;
            ModeFinder** modeFinders;

        public:
            DesignPointBuilder(int count, DesignPointMethod method, StochastSettingsSet* stochastSet);
            void initialize(double beta);
            void addSample(Sample* sample);
            Sample* getSample();
        };
    }
}





