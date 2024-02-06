#pragma once

#include "../../Deltares.Probabilistic/Statistics/FragilityValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class FragilityValueWrapper
			{
			private:
				Statistics::FragilityValue* m_value;
				SharedPointerProvider<Statistics::FragilityValue>* sharedPointer = new SharedPointerProvider<Statistics::FragilityValue>();
			public:
				FragilityValueWrapper()
				{
					m_value = new Statistics::FragilityValue();
				}
				~FragilityValueWrapper() { this->!FragilityValueWrapper(); }
				!FragilityValueWrapper() { delete sharedPointer; }
				property double X
				{
					double get() { return m_value->X; }
					void set(double value) { m_value->X = value; }
				}

				property double Reliability
				{
					double get() { return m_value->Reliability; }
					void set(double value) { m_value->Reliability = value; }
				}

				std::shared_ptr<Statistics::FragilityValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}