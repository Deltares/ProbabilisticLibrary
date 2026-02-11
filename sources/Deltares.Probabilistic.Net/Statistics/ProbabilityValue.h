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

#include "../../Deltares.Probabilistic/Statistics/ProbabilityValue.h"
#include "../Model/BaseStochastPoint.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class ProbabilityValue
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::ProbabilityValue>* shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::ProbabilityValue());
            public:
                ProbabilityValue() {}

                ~ProbabilityValue() { this->!ProbabilityValue(); }
                !ProbabilityValue() { delete shared; }

                virtual property double Reliability
                {
                    double get() { return shared->object->Reliability; }
                    void set(double value) { shared->object->Reliability = value; }
                }

                virtual property double ProbabilityFailure
                {
                    double get() { return shared->object->getProbabilityOfFailure(); }
                    void set(double value) { shared->object->setProbabilityOfFailure(value); }
                }

                virtual property double ProbabilityNonFailure
                {
                    double get() { return shared->object->getProbabilityOfNonFailure(); }
                    void set(double value) { shared->object->setProbabilityOfNonFailure(value); }
                }

                virtual property double ReturnPeriod
                {
                    double get() { return shared->object->getReturnPeriod(); }
                    void set(double value) { shared->object->setReturnPeriod(value); }
                }

                std::shared_ptr<Statistics::ProbabilityValue> GetValue()
                {
                    return shared->object;
                }
            };
        }
    }
}
