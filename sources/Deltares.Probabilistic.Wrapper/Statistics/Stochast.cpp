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
#include "Stochast.h"

namespace Deltares
{
    namespace Statistics
    {
        namespace Wrappers
        {
            typedef double(__stdcall* UXDelegate) (double);

            Statistics::ConstantParameterType Stochast::getNativeConstantParameterType(Wrappers::ConstantParameterType constantParameterType)
            {
                switch (constantParameterType)
                {
                case Wrappers::ConstantParameterType::Deviation: return Statistics::ConstantParameterType::Deviation;
                case Wrappers::ConstantParameterType::VariationCoefficient: return Statistics::ConstantParameterType::VariationCoefficient;
                default: throw gcnew System::NotSupportedException(constantParameterType.ToString());
                }
            }

            void Stochast::updateStochast()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                shared->object->ValueSet = this->ValueSet->GetValue();
                shared->object->ValueSet->StochastValues.clear();
                if (this->IsVariableStochast)
                {
                    for (size_t i = 0; i < this->ValueSet->StochastValues->Count; i++)
                    {
                        shared->object->ValueSet->StochastValues.push_back(this->ValueSet->StochastValues[i]->GetValue());
                    }
                }
            }

            void Stochast::SynchronizeHistogramValues(ListOperationType listOperationType, HistogramValue^ histogramValue)
            {
                if (!synchronizing)
                {
                    std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                    properties->setDirty();

                    switch (listOperationType)
                    {
                    case ListOperationType::Add: properties->HistogramValues.push_back(histogramValue->GetValue()); break;
                    case ListOperationType::Remove: std::erase(properties->HistogramValues, histogramValue->GetValue()); break;
                    case ListOperationType::Clear: properties->HistogramValues.clear(); break;
                    default: throw gcnew System::NotImplementedException("List operation type");
                    }
                }
            }

            void Stochast::SynchronizeDiscreteValues(ListOperationType listOperationType, DiscreteValue^ discreteValue)
            {
                if (!synchronizing)
                {
                    std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                    properties->setDirty();

                    switch (listOperationType)
                    {
                    case ListOperationType::Add: properties->DiscreteValues.push_back(discreteValue->GetValue()); break;
                    case ListOperationType::Remove: std::erase(properties->DiscreteValues, discreteValue->GetValue()); break;
                    case ListOperationType::Clear: properties->DiscreteValues.clear(); break;
                    default: throw gcnew System::NotImplementedException("List operation type");
                    }
                }
            }

            void Stochast::SynchronizeFragilityValues(ListOperationType listOperationType, Wrappers::FragilityValue^ fragilityValue)
            {
                if (!synchronizing)
                {
                    std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                    properties->setDirty();

                    switch (listOperationType)
                    {
                    case ListOperationType::Add: properties->FragilityValues.push_back(fragilityValue->GetValue()); break;
                    case ListOperationType::Remove: std::erase(properties->FragilityValues, fragilityValue->GetValue()); break;
                    case ListOperationType::Clear: properties->FragilityValues.clear(); break;
                    default: throw gcnew System::NotImplementedException("List operation type");
                    }
                }
            }

            void Stochast::SynchronizeContributingStochasts(ListOperationType listOperationType, ContributingStochast^ contributingStochast)
            {
                if (!synchronizing)
                {
                    std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                    properties->setDirty();

                    switch (listOperationType)
                    {
                    case ListOperationType::Add: properties->ContributingStochasts.push_back(contributingStochast->GetValue()); break;
                    case ListOperationType::Remove: std::erase(properties->ContributingStochasts, contributingStochast->GetValue()); break;
                    case ListOperationType::Clear: properties->ContributingStochasts.clear(); break;
                    default: throw gcnew System::NotImplementedException("List operation type");
                    }
                }
            }

            void Stochast::updateLists()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                this->synchronizing = true;

                try
                {
                    if (!AreHistogramValuesMatching())
                    {
                        this->HistogramValues->Clear();
                        for (int i = 0; i < properties->HistogramValues.size(); i++)
                        {
                            this->HistogramValues->Add(gcnew HistogramValue(properties->HistogramValues[i]));
                        }
                    }

                    if (!AreFragilityValuesMatching())
                    {
                        this->FragilityValues->Clear();
                        for (int i = 0; i < properties->FragilityValues.size(); i++)
                        {
                            this->FragilityValues->Add(gcnew Wrappers::FragilityValue(properties->FragilityValues[i]));
                        }
                    }

                    if (!AreDiscreteValuesMatching())
                    {
                        this->DiscreteValues->Clear();
                        for (int i = 0; i < properties->DiscreteValues.size(); i++)
                        {
                            this->DiscreteValues->Add(gcnew DiscreteValue(properties->DiscreteValues[i]));
                        }
                    }

                    if (!AreContributingStochastsMatching())
                    {
                        this->ContributingStochasts->Clear();
                        for (int i = 0; i < properties->ContributingStochasts.size(); i++)
                        {
                            this->ContributingStochasts->Add(gcnew ContributingStochast(properties->ContributingStochasts[i]));
                        }
                    }
                }
                finally
                {
                    this->synchronizing = false;
                }
            }

            bool Stochast::AreHistogramValuesMatching()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                if (this->HistogramValues->Count != properties->HistogramValues.size())
                {
                    return false;
                }

                for (int i = 0; i < this->HistogramValues->Count; i++)
                {
                    if (this->HistogramValues[i]->GetValue() != properties->HistogramValues[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            bool Stochast::AreFragilityValuesMatching()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                if (this->FragilityValues->Count != properties->FragilityValues.size())
                {
                    return false;
                }

                for (int i = 0; i < this->FragilityValues->Count; i++)
                {
                    if (this->FragilityValues[i]->GetValue() != properties->FragilityValues[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            bool Stochast::AreDiscreteValuesMatching()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                if (this->DiscreteValues->Count != properties->DiscreteValues.size())
                {
                    return false;
                }

                for (int i = 0; i < this->DiscreteValues->Count; i++)
                {
                    if (this->DiscreteValues[i]->GetValue() != properties->DiscreteValues[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            bool Stochast::AreContributingStochastsMatching()
            {
                std::shared_ptr<StochastProperties> properties = shared->object->getProperties();

                if (this->ContributingStochasts->Count != properties->ContributingStochasts.size())
                {
                    return false;
                }

                for (int i = 0; i < this->ContributingStochasts->Count; i++)
                {
                    if (this->ContributingStochasts[i]->GetValue() != properties->ContributingStochasts[i])
                    {
                        return false;
                    }
                }

                return true;
            }

            void Stochast::SetExternalDistribution(ManagedUXDelegate^ uxDelegate)
            {
                System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(uxDelegate);
                UXLambda functionPointer = static_cast<UXDelegate>(callbackPtr.ToPointer());

                handles->Add(uxDelegate);

                shared->object->setExternalDistribution(functionPointer);
            }
        }
    }
}
