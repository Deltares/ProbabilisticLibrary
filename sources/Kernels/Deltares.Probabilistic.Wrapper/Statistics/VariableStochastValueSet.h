#pragma once
#include "DistributionType.h"
#include "VariableStochastValue.h"

#include "../../Deltares.Probabilistic/Statistics/VariableStochastValueSet.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class VariableStochastValueSet
			{
			private:
				SharedPointerProvider<Statistics::VariableStochastValuesSet>* shared = new SharedPointerProvider(new Statistics::VariableStochastValuesSet());

				System::Collections::Generic::List<VariableStochastValue^>^ stochastValues = gcnew System::Collections::Generic::List<VariableStochastValue^>();

			public:
				VariableStochastValueSet() {}
				~VariableStochastValueSet() { this->!VariableStochastValueSet(); }
				!VariableStochastValueSet() { delete shared; }

				property System::Collections::Generic::List<VariableStochastValue^>^ StochastValues
				{
					System::Collections::Generic::List<VariableStochastValue^>^ get() { return stochastValues; }
				}

				bool IsVarying(Wrappers::DistributionType distributionType)
				{
					return shared->object->isVarying(DistributionTypeConverter::getNativeDistributionType(distributionType));
				}

				std::shared_ptr<Statistics::VariableStochastValuesSet> GetValue()
				{
					shared->object->StochastValues.clear();

					for (int i = 0; i < stochastValues->Count; i++)
					{
						shared->object->StochastValues.push_back(this->stochastValues[i]->GetValue());
					}

					return shared->object;
				}
			};
		}
	}
}








