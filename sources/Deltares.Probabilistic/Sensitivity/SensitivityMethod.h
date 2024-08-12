#pragma once
#include "../Statistics/Stochast.h"
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Sensitivity
    {
        class SensitivityMethod
        {
        private:
            bool stopped = false;

        protected:

            virtual void setStopped();
            std::shared_ptr<Statistics::Stochast> getStochastFromSamples(std::vector<double>& samples, std::vector<double>& weights);

        public:
            virtual std::shared_ptr<Statistics::Stochast> getStochast(std::shared_ptr<Models::ModelRunner> modelRunner) { return nullptr; }
            virtual ~SensitivityMethod() = default;

            bool isStopped();
            void Stop();
        };
    }
}

