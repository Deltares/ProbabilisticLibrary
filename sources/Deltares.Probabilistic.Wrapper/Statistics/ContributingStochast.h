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







