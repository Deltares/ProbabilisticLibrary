#include "FragilityCurve.h"
#include "../Statistics/FragilityValue.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            Deltares::Models::Wrappers::StochastPoint^ FragilityCurve::GetRealization(double x)
            {
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
                                stochasts->Add(designPoint->Alphas[j]->Parameter);
                            }
                        }
                    }

                    return gcnew Reliability::Wrappers::StochastPoint(stochastPoint, stochasts);
                }
                else
                {
                    return nullptr;
                }
            };

            void FragilityCurve::SynchronizeContributingFragilityCurves(ListOperationType listOperationType, FragilityCurve^ fragilityCurve)
            {
                switch (listOperationType)
                {
                case ListOperationType::Add: shared->object->contributingFragilityCurves.push_back(fragilityCurve->GetNativeValue()); break;
                case ListOperationType::Remove: std::erase(shared->object->contributingFragilityCurves, fragilityCurve->GetNativeValue()); break;
                case ListOperationType::Clear: shared->object->contributingFragilityCurves.clear(); break;
                default: throw gcnew System::NotImplementedException("List operation type");
                }
            }

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
                }

                return true;
            }
        }
    }
}


