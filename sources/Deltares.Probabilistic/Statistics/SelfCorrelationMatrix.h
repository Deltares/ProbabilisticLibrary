// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once
#include "Stochast.h"
#include "../Reliability/DesignPoint.h"
#include <memory>

namespace Deltares
{
    namespace Statistics
    {
        /**
         * \brief Registers the correlation for a stochast between different design points
         */
        class SelfCorrelationMatrix
        {

        public:
            /**
             * \brief Sets the correlation for a stochast between two design points
             * \param stochast Stochast
             * \param designPoint1 First design point
             * \param designPoint2 Second design point
             * \param rho Correlation value
             */
            void setSelfCorrelation(std::shared_ptr<Stochast> stochast, std::shared_ptr<Reliability::DesignPoint> designPoint1, std::shared_ptr<Reliability::DesignPoint> designPoint2, double rho)
            {
                std::shared_ptr<SelfCorrelationValue> value = this->getSelfCorrelationValue(stochast, designPoint1, designPoint2, true);
                value->rho = rho;
            }

            /**
             * \brief Sets the default correlation value for a stochast for all design point combinations
             * \param stochast Stochast
             * \param rho Default correlation value
             */
            void setSelfCorrelation(std::shared_ptr<Stochast> stochast, double rho)
            {
                this->getSelfCorrelationStochast(stochast)->defaultRho = rho;
            }

            /**
             * \brief Gets the correlation value between two design points
             * \param stochast Stochast
             * \param designPoint1 First design point
             * \param designPoint2 Second design point
             * \return Correlation value
             * \remark If the correlation value was not registered for these design points, the default correlation value is returned
             */
            double getSelfCorrelation(std::shared_ptr<Stochast> stochast, std::shared_ptr<Reliability::DesignPoint> designPoint1, std::shared_ptr<Reliability::DesignPoint> designPoint2)
            {
                std::shared_ptr<SelfCorrelationStochast> stochastValue = this->getSelfCorrelationStochast(stochast);

                if (stochastValue->hasSelfCorrelationValue(designPoint1, designPoint2))
                {
                    return stochastValue->getSelfCorrelationValue(designPoint1, designPoint2)->rho;
                }
                else
                {
                    return stochastValue->defaultRho;
                }
            }

            /**
             * \brief Gets the default correlation value of a stochast for all design point combinations
             * \param stochast Stochast
             * \return Default correlation value
             */
            double getSelfCorrelation(std::shared_ptr<Stochast> stochast)
            {
                return this->getSelfCorrelationStochast(stochast)->defaultRho;
            }

            /**
             * \brief Indicates whether a stochast has a self correlation unequal to 1
             * \param stochast Stochast
             * \return Indication
             */
            bool isSelfCorrelated(std::shared_ptr<Stochast> stochast)
            {
                return this->getSelfCorrelationStochast(stochast)->isSelfCorrelated();
            }

        private:
            class SelfCorrelationValue
            {
            public:
                SelfCorrelationValue() {}

                std::shared_ptr<Reliability::DesignPoint> designPoint1 = nullptr;
                std::shared_ptr<Reliability::DesignPoint> designPoint2 = nullptr;
                double rho = 1.0;

                bool isSelfCorrelated()
                {
                    return rho != 1.0;
                }
            };

            class SelfCorrelationStochast
            {
            public:
                std::shared_ptr<Statistics::Stochast> stochast = nullptr;
                double defaultRho = 1;

                bool hasSelfCorrelationValue(std::shared_ptr<Reliability::DesignPoint> designPoint1, std::shared_ptr<Reliability::DesignPoint> designPoint2)
                {
                    for (std::shared_ptr<SelfCorrelationValue> value : rhoValues)
                    {
                        if ((value->designPoint1 == designPoint1 && value->designPoint2 == designPoint2) || (value->designPoint1 == designPoint2 && value->designPoint2 == designPoint1))
                        {
                            return true;
                        }
                    }

                    return false;
                }

                std::shared_ptr<SelfCorrelationValue> getSelfCorrelationValue(std::shared_ptr<Reliability::DesignPoint> designPoint1, std::shared_ptr<Reliability::DesignPoint> designPoint2)
                {
                    for (std::shared_ptr<SelfCorrelationValue> value : rhoValues)
                    {
                        if ((value->designPoint1 == designPoint1 && value->designPoint2 == designPoint2) || (value->designPoint1 == designPoint2 && value->designPoint2 == designPoint1))
                        {
                            return value;
                        }
                    }

                    std::shared_ptr<SelfCorrelationValue> newValue = std::make_shared<SelfCorrelationValue>();
                    newValue->designPoint1 = designPoint1;
                    newValue->designPoint2 = designPoint2;

                    rhoValues.push_back(newValue);

                    return newValue;
                }

                bool isSelfCorrelated()
                {
                    for (size_t i = 0; i < this->rhoValues.size(); i++)
                    {
                        if (rhoValues[i]->isSelfCorrelated())
                        {
                            return true;
                        }
                    }

                    return defaultRho != 1.0;
                }

            private:
                std::vector<std::shared_ptr<SelfCorrelationValue>> rhoValues;
            };

            std::vector<std::shared_ptr<SelfCorrelationStochast>> stochastValues;

            std::shared_ptr<SelfCorrelationStochast> getSelfCorrelationStochast(std::shared_ptr<Stochast> stochast)
            {
                for (std::shared_ptr<SelfCorrelationStochast> value : stochastValues)
                {
                    if (value->stochast == stochast)
                    {
                        return value;
                    }
                }

                std::shared_ptr<SelfCorrelationStochast> newValue = std::make_shared<SelfCorrelationStochast>();
                newValue->stochast = stochast;
                stochastValues.push_back(newValue);

                return newValue;
            }

            std::shared_ptr<SelfCorrelationValue> getSelfCorrelationValue(std::shared_ptr<Stochast> stochast, std::shared_ptr<Reliability::DesignPoint> designPoint1, std::shared_ptr<Reliability::DesignPoint> designPoint2, bool create)
            {
                std::shared_ptr<SelfCorrelationStochast> stochastValue = this->getSelfCorrelationStochast(stochast);
                return stochastValue->getSelfCorrelationValue(designPoint1, designPoint2);
            }
        };
    }
}

