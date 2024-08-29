#pragma once

#include "BaseStochastPoint.h"
#include "StandardNormal.h"
#include "../Utils/DirtySupport.h"

#include <memory>

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Point in a fragility curve or empirical distribution
         */
        class FragilityValue
        {
        public:
            /**
             * \brief Physical value for which the reliability is defined
             */
            double X;

            /**
             * \brief Reliability of the fragility point
             */
            double Reliability;

            /**
             * \brief Design point corresponding with the reliability
             * \remark This property is always of the type designPoint. The only reason it is declared as BaseStochastPoint is that C++ does not allow circular references
             */
            std::shared_ptr<BaseStochastPoint> designPoint = nullptr;

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

            std::shared_ptr<FragilityValue> clone()
            {
                std::shared_ptr<FragilityValue> clone = std::make_shared<FragilityValue>();

                clone->X = this->X;
                clone->Reliability = this->Reliability;
                clone->designPoint = this->designPoint;

                return clone;
            }

        private:
            Utils::SetDirtyLambda setDirtyLambda = nullptr;
        };
    }
}

