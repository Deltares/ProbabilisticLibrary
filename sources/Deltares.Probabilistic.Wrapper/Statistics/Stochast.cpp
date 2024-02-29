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

				properties->DiscreteValues.clear();
				for (size_t i = 0; i < this->discreteValues->Count; i++)
				{
					properties->DiscreteValues.push_back(discreteValues[i]->GetValue());
				}

				properties->HistogramValues.clear();
				for (size_t i = 0; i < this->histogramValues->Count; i++)
				{
					properties->HistogramValues.push_back(histogramValues[i]->GetValue());
				}

				properties->FragilityValues.clear();
				for (size_t i = 0; i < this->fragilityValues->Count; i++)
				{
					properties->FragilityValues.push_back(fragilityValues[i]->GetValue());
				}

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

			void Stochast::SetExternalDistribution(ManagedUXDelegate^ uxDelegate)
			{
				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(uxDelegate);
				UXLambda functionPointer = static_cast<UXDelegate>(callbackPtr.ToPointer());

				shared->object->setExternalDistribution(functionPointer);
			}
		}
	}
}
