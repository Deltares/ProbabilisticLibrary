#pragma once
#include "DesignPoint.h"
#include "../Model/ModelRunner.h"

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
            virtual void setStopped();

        public:
            virtual std::shared_ptr<DesignPoint> getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner) { return nullptr; }
            virtual ~ReliabilityMethod() = default;

            bool isStopped();
            void Stop();
        };
    }
}

