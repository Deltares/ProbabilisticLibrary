#pragma once

#include "../../Deltares.Probabilistic/Statistics/DiscreteValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public ref class DiscreteValue
            {
            private:
                SharedPointerProvider<Statistics::DiscreteValue>* shared = nullptr;

            public:
                DiscreteValue()
                {
                    shared = new SharedPointerProvider(new Statistics::DiscreteValue());
                }

                DiscreteValue(std::shared_ptr<Statistics::DiscreteValue> discreteValue)
                {
                    shared = new SharedPointerProvider(discreteValue);
                }

                DiscreteValue(double x, double amount)
                {
                    shared = new SharedPointerProvider(new Statistics::DiscreteValue());
                    shared->object->X = x;
                    shared->object->Amount = amount;
                }

                ~DiscreteValue() { this->!DiscreteValue(); }
                !DiscreteValue() { delete shared; }

                property double X
                {
                    double get() { return shared->object->X; }
                    void set(double value)
                    {
                        shared->object->X = value;
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

                property double NormalizedAmount
                {
                    double get() { return shared->object->NormalizedAmount; }
                }

                property double CumulativeNormalizedAmount
                {
                    double get() { return shared->object->CumulativeNormalizedAmount; }
                }

                std::shared_ptr<Statistics::DiscreteValue> GetValue()
                {
                    return shared->object;
                }
            };
        }
    }
}




