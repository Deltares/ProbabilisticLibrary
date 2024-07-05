#pragma once

#include "../../Deltares.Probabilistic/Statistics/HistogramValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class HistogramValue
            {
            private:
                SharedPointerProvider<Statistics::HistogramValue>* shared = nullptr;
            public:
                HistogramValue()
                {
                    shared = new SharedPointerProvider(new Statistics::HistogramValue());
                }

                HistogramValue(std::shared_ptr<Statistics::HistogramValue> histogramValue)
                {
                    shared = new SharedPointerProvider(histogramValue);
                }

                HistogramValue(double lowerBound, double upperBound, double amount)
                {
                    shared = new SharedPointerProvider(new Statistics::HistogramValue());
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

                bool IsValid()
                {
                    return shared->object->isValid();
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







