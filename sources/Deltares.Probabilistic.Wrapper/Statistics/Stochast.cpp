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

            void Stochast::SynchronizeFragilityValues(ListOperationType listOperationType, FragilityValue^ fragilityValue)
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
                            this->FragilityValues->Add(gcnew FragilityValue(properties->FragilityValues[i]));
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
