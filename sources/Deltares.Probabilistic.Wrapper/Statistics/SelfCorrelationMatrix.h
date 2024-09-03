// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "Stochast.h"
#include "../Reliability/DesignPoint.h"
#include "../../Deltares.Probabilistic/Statistics/SelfCorrelationMatrix.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            /**
             * \brief Registers the correlation for a stochast in different design points
             */
            public ref class SelfCorrelationMatrix
            {
            private:
                SharedPointerProvider<Statistics::SelfCorrelationMatrix>* shared = new SharedPointerProvider(new Statistics::SelfCorrelationMatrix());
            public:
                SelfCorrelationMatrix() { }
                ~SelfCorrelationMatrix() { this->!SelfCorrelationMatrix(); }
                !SelfCorrelationMatrix() { delete shared; }

                /**
                 * \brief Sets the correlation for a stochast between two design points
                 * \param stochast Stochast
                 * \param designPoint1 First design point
                 * \param designPoint2 Second design point
                 * \param rho Correlation value
                 */
                void SetSelfCorrelation(Wrappers::Stochast^ stochast, Reliability::Wrappers::DesignPoint^ designPoint1, Reliability::Wrappers::DesignPoint^ designPoint2, double rho)
                {
                    shared->object->setSelfCorrelation(stochast->GetStochast(), designPoint1->getDesignPoint(), designPoint2->getDesignPoint(), rho);
                }

                /**
                 * \brief Sets the default correlation value for a stochast for all design point combinations
                 * \param stochast Stochast
                 * \param rho Correlation value
                 */
                void SetSelfCorrelation(Wrappers::Stochast^ stochast, double rho)
                {
                    shared->object->setSelfCorrelation(stochast->GetStochast(), rho);
                }

                /**
                 * \brief Gets the correlation value between two design points
                 * \param stochast Stochast
                 * \param designPoint1 First design point
                 * \param designPoint2 Second design point
                 * \return Correlation value
                 * \remark If the correlation value was not registered for these design points, the default correlation value is returned
                 */
                double GetSelfCorrelation(Wrappers::Stochast^ stochast, Reliability::Wrappers::DesignPoint^ designPoint1, Reliability::Wrappers::DesignPoint^ designPoint2)
                {
                    return shared->object->getSelfCorrelation(stochast->GetStochast(), designPoint1->getDesignPoint(), designPoint2->getDesignPoint());
                }

                /**
                 * \brief Gets the default correlation value of a stochast for all design point combinations
                 * \param stochast Stochast
                 * \return Default correlation value
                 */
                double GetSelfCorrelation(Wrappers::Stochast^ stochast)
                {
                    return shared->object->getSelfCorrelation(stochast->GetStochast());
                }


                std::shared_ptr<Statistics::SelfCorrelationMatrix> GetNativeSelfCorrelationMatrix()
                {
                    return shared->object;
                }
            };
        }
    }
}

