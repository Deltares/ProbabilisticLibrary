#pragma once

#include "../Utils/DirtySupport.h"
#include <memory>

namespace Deltares
{
    namespace Statistics
    {
        class DiscreteValue
        {
        public:
            DiscreteValue()    { }

            DiscreteValue(double x, double amount)
            {
                this->X = x;
                this->Amount = amount;
            }

            ~DiscreteValue()
            {
            }

            double X = 0;
            double Amount = 0;

            double NormalizedAmount = 0; // for internal use
            double CumulativeNormalizedAmount = 0; // for internal use

            void setDirtyFunction(Utils::SetDirtyLambda setDirtyLambda)
            {
                this->setDirtyLambda = setDirtyLambda;
            }

            void setDirty()
            {
                if (setDirtyLambda != nullptr)
                {
                    setDirtyLambda();
                }
            }

            std::shared_ptr<DiscreteValue> clone()
            {
                std::shared_ptr<DiscreteValue> clone = std::make_shared<DiscreteValue>();

                clone->X = this->X;
                clone->Amount = this->Amount;

                return clone;
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

