#pragma once

#include "../Model/Sample.h"
#include "../Model/ModelRunner.h"

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
            std::shared_ptr<Sample> defaultSample = nullptr;
            std::shared_ptr<Sample> meanSample = nullptr;
            std::shared_ptr<Sample> sinSample = nullptr;
            std::shared_ptr<Sample> cosSample = nullptr;
            bool sampleAdded = false;

            std::vector<int> qualitativeIndices;
            int qualitativeCount = 0;
            std::vector<std::shared_ptr<ModeFinder>> modeFinders;

        public:
            DesignPointBuilder(int count, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet = nullptr);
            void initialize(double beta);
            void addSample(std::shared_ptr<Sample> sample);
            std::shared_ptr<Sample> getSample();

            static std::string getDesignPointMethodString(DesignPointMethod method);
            static DesignPointMethod getDesignPointMethod(std::string method);
        };
    }
}





