#pragma once
#include "VariableStochastValueWrapper.h"

#include "../../Deltares.Probabilistic.Kernels/Statistics/VariableStochastValueSet.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class VariableStochastValueSetWrapper
			{
			private:
				Statistics::VariableStochastValuesSet* m_value;
				SharedPointerProvider<Statistics::VariableStochastValuesSet>* sharedPointer = new SharedPointerProvider<Statistics::VariableStochastValuesSet>();

				System::Collections::Generic::List<Deltares::Probabilistic::Kernels::VariableStochastValueWrapper^>^ stochastValues = gcnew System::Collections::Generic::List<Deltares::Probabilistic::Kernels::VariableStochastValueWrapper^>();

			public:
				VariableStochastValueSetWrapper()
				{
					m_value = new Statistics::VariableStochastValuesSet();
				}
				~VariableStochastValueSetWrapper() { this->!VariableStochastValueSetWrapper(); }
				!VariableStochastValueSetWrapper() { delete sharedPointer; }

				property System::Collections::Generic::List<Deltares::Probabilistic::Kernels::VariableStochastValueWrapper^>^ StochastValues
				{
					System::Collections::Generic::List<Deltares::Probabilistic::Kernels::VariableStochastValueWrapper^>^ get() { return stochastValues; }
				}

				std::shared_ptr<Statistics::VariableStochastValuesSet> GetValue()
				{
					m_value->StochastValues.clear();

					//for (int i = 0; i < stochastValues->Count; i++)
					//{
					//	m_value->StochastValues.push_back(this->stochastValues[i]->GetValue());
					//}

					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}








