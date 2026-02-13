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

#include "../../Deltares.Probabilistic/Statistics/HistogramValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            public ref class HistogramValue
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Statistics::HistogramValue>* shared = nullptr;
            public:
                HistogramValue()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::HistogramValue());
                }

                HistogramValue(std::shared_ptr<Statistics::HistogramValue> histogramValue)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(histogramValue);
                }

                HistogramValue(double lowerBound, double upperBound, double amount)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Statistics::HistogramValue());
                    this->LowerBound = lowerBound;
                    this->UpperBound = upperBound;
                    this->Amount = amount;
                }

                ~HistogramValue() { this->!HistogramValue(); }
                !HistogramValue() { delete shared; }

                property double LowerBound
                {
                    double get() { return shared->object->LowerBound; }
                    void set(double value)
                    {
                        shared->object->LowerBound = value;
                        shared->object->setDirty();
                    }
                }

                property double UpperBound
                {
                    double get() { return shared->object->UpperBound; }
                    void set(double value)
                    {
                        shared->object->UpperBound = value;
                        shared->object->setDirty();
                    }
                }

                property double Amount
                {
                    double get() { return shared->object->Amount; }
                    void set(double value)
                    {
                        shared->object->Amount = value;
                        shared->object->setDirty();
                    }
                }

                property double Size
                {
                    double get() { return shared->object->getSize(); }
                }

                property double Center
                {
                    double get() { return shared->object->getCenter(); }
                }

                std::shared_ptr<Statistics::HistogramValue> GetValue()
                {
                    return shared->object;
                }
            };
        }
    }
}







