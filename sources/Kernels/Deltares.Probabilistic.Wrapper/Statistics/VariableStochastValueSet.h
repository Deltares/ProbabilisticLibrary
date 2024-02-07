#pragma once
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
				Statistics::VariableStochastValuesSet* m_value;
				SharedPointerProvider<Statistics::VariableStochastValuesSet>* sharedPointer = new SharedPointerProvider<Statistics::VariableStochastValuesSet>();

				System::Collections::Generic::List<VariableStochastValue^>^ stochastValues = gcnew System::Collections::Generic::List<VariableStochastValue^>();

			public:
				VariableStochastValueSet()
				{
					m_value = new Statistics::VariableStochastValuesSet();
				}
				~VariableStochastValueSet() { this->!VariableStochastValueSet(); }
				!VariableStochastValueSet() { delete sharedPointer; }

				property System::Collections::Generic::List<VariableStochastValue^>^ StochastValues
				{
					System::Collections::Generic::List<VariableStochastValue^>^ get() { return stochastValues; }
				}

				std::shared_ptr<Statistics::VariableStochastValuesSet> GetValue()
				{
					m_value->StochastValues.clear();

					for (int i = 0; i < stochastValues->Count; i++)
					{
						m_value->StochastValues.push_back(this->stochastValues[i]->GetValue());
					}

					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}








