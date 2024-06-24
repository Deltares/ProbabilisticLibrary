#pragma once
#include <vector>

#include "CorrelationMatrix.h"
#include "Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        class SparseCorrelationMatrix
        {
        public:
            double getCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2);
            void setCorrelation(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2, double value);

            std::shared_ptr<CorrelationMatrix> getCorrelationMatrix(std::vector<std::shared_ptr<Stochast>> stochasts);
        private:
            class CorrelationValue
            {
            public:
                CorrelationValue(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2)
                {
                    this->stochast1 = stochast1;
                    this->stochast2 = stochast2;
                }

                std::shared_ptr<Stochast> stochast1 = nullptr;
                std::shared_ptr<Stochast> stochast2 = nullptr;
                double value = 0;

                bool matches(std::shared_ptr<Stochast> stochast1, std::shared_ptr<Stochast> stochast2) const
                {
                    return this->stochast1 == stochast1 && this->stochast2 == stochast2 || this->stochast1 == stochast2 && this->stochast2 == stochast1;
                }
            };

            std::vector<std::shared_ptr<CorrelationValue>> correlations;
        };
    }
}

