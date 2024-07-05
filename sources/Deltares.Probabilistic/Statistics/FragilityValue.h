#pragma once

#include "StandardNormal.h"
#include "../Utils/DirtySupport.h"

namespace Deltares
{
    namespace Statistics
    {
        class FragilityValue
        {
        public:
            double X;
            double Reliability;

            double getProbabilityOfFailure()
            {
                return StandardNormal::getQFromU(this->Reliability);
            }

            void setProbabilityOfFailure(double q)
            {
                this->Reliability = StandardNormal::getUFromQ(q);
            }

            double getProbabilityOfNonFailure()
            {
                return StandardNormal::getPFromU(this->Reliability);
            }

            void setProbabilityOfNonFailure(double p)
            {
                this->Reliability = StandardNormal::getUFromP(p);
            }

            double getReturnPeriod()
            {
                return StandardNormal::getRFromU(this->Reliability);
            }

            void setReturnPeriod(double r)
            {
                this->Reliability = StandardNormal::getUFromR(r);
            }

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

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

