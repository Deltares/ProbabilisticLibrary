#include "FragilityCurve.h"
#include "../Statistics/FragilityValue.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            void FragilityCurve::updateNativeObject()
            {
                if (!HasMatchingFragilityValues())
                {
                    shared->object->getProperties()->FragilityValues.clear();

                    for (int i = 0; i < this->FragilityValues->Count; i++)
                    {
                        shared->object->getProperties()->FragilityValues.push_back(this->FragilityValues[i]->GetValue());
                    }
                }
            }

            Deltares::Models::Wrappers::StochastPoint^ FragilityCurve::GetRealization(double x)
            {
                this->updateNativeObject();

                std::shared_ptr<Models::StochastPoint> stochastPoint = shared->object->getDesignPoint(x);

                if (stochastPoint != nullptr)
                {
                    System::Collections::Generic::List<Stochast^>^ stochasts = gcnew System::Collections::Generic::List<Stochast^>();
                    for (size_t i = 0; i < this->FragilityValues->Count; i++)
                    {
                        FragilityValue^ fragilityValue = this->FragilityValues[i];
                        if (fragilityValue->DesignPoint != nullptr)
                        {
                            Models::Wrappers::StochastPoint^ designPoint = static_cast<Models::Wrappers::StochastPoint^>(fragilityValue->DesignPoint);
                            for (size_t j = 0; j < designPoint->Alphas->Count; j++)
                            {
                                if (!stochasts->Contains(designPoint->Alphas[j]->Parameter))
                                {
                                    stochasts->Add(designPoint->Alphas[j]->Parameter);
                                }
                            }
                        }
                    }

                    stochastPoint->updateInfluenceFactors();

                    return gcnew Reliability::Wrappers::StochastPoint(stochastPoint, stochasts);
                }
                else
                {
                    return nullptr;
                }
            };

            bool FragilityCurve::HasMatchingFragilityValues()
            {
                if (this->FragilityValues->Count != shared->object->getProperties()->FragilityValues.size())
                {
                    return false;
                }

                for (int i = 0; i < this->FragilityValues->Count; i++)
                {
                    if (this->FragilityValues[i]->GetValue() != shared->object->getProperties()->FragilityValues[i])
                    {
                        return false;
                    }

                    if (this->FragilityValues[i]->DesignPoint != nullptr && shared->object->getProperties()->FragilityValues[i]->designPoint == nullptr)
                    {
                        return false;
                    }
                }

                return true;
            }
        }
    }
}


