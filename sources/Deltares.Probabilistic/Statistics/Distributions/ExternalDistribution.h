#pragma once

#include "Distribution.h"
#include <functional>

namespace Deltares
{
    namespace Statistics
    {
        class ExternalDistribution : public Distribution
        {
        public:
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override { return true; }

            void setExternalFunction(UXLambda uxFunction)
            {
                this->uxFunction = uxFunction;
            }
        private:
            UXLambda uxFunction = nullptr;
        };
    }
}

