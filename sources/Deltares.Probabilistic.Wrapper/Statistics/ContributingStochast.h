// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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

#include "BaseStochast.h"
#include "../../Deltares.Probabilistic/Statistics/ContributingStochast.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class ContributingStochast
            {
            private:
                SharedPointerProvider<Statistics::ContributingStochast>* shared = nullptr;
                BaseStochast^ stochast = nullptr;

            public:
                ContributingStochast()
                {
                    shared = new SharedPointerProvider(new Statistics::ContributingStochast());
                }

                ContributingStochast(std::shared_ptr<Statistics::ContributingStochast> contributingStochast)
                {
                    shared = new SharedPointerProvider(contributingStochast);
                }

                ~ContributingStochast() { this->!ContributingStochast(); }
                !ContributingStochast() { delete shared; }

                property double Probability
                {
                    double get() { return shared->object->Probability; }
                    void set(double value) { shared->object->Probability = value; }
                }

                property BaseStochast^ Stochast
                {
                    BaseStochast^ get() { return this->stochast; }
                    void set(BaseStochast^ value) { this->stochast = value; }
                }

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Statistics::ContributingStochast> GetValue()
                {
                    if (this->stochast != nullptr)
                    {
                        shared->object->Stochast = this->stochast->GetStochast();
                    }

                    return shared->object;
                }
            };
        }
    }
}







