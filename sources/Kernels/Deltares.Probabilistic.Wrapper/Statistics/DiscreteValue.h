#pragma once

#include "../../Deltares.Probabilistic/Statistics/DiscreteValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class DiscreteValue
			{
			private:
				Statistics::DiscreteValue* m_value;
				SharedPointerProvider<Statistics::DiscreteValue>* sharedPointer = new SharedPointerProvider<Statistics::DiscreteValue>();

			public:
				DiscreteValue()
				{
					m_value = new Statistics::DiscreteValue();
				}
				DiscreteValue(double x, double amount)
				{
					m_value = new Statistics::DiscreteValue(x, amount);
					m_value->X = x;
					m_value->Amount = amount;
				}
				~DiscreteValue() { this->!DiscreteValue(); }
				!DiscreteValue() { delete sharedPointer; }

				property double X
				{
					double get() { return m_value->X; }
					void set(double value) { m_value->X = value; }
				}

				property double Amount
				{
					double get() { return m_value->Amount; }
					void set(double value) { m_value->Amount = value; }
				}

				property double NormalizedAmount
				{
					double get() { return m_value->NormalizedAmount; }
				}

				property double CumulativeNormalizedAmount
				{
					double get() { return m_value->CumulativeNormalizedAmount; }
				}

				std::shared_ptr<Statistics::DiscreteValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}




