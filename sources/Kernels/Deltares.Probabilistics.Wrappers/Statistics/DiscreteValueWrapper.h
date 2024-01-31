#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/DiscreteValue.h"
#include "../../Deltares.Probabilistic.Kernels/Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class DiscreteValueWrapper
			{
			private:
				Statistics::DiscreteValue* m_value;
				Utils::SharedPointerProvider<Statistics::DiscreteValue>* sharedPointer = new Utils::SharedPointerProvider<Statistics::DiscreteValue>();

			public:
				DiscreteValueWrapper()
				{
					m_value = new Statistics::DiscreteValue();
				}
				DiscreteValueWrapper(double x, double amount)
				{
					m_value = new Statistics::DiscreteValue(x, amount);
					m_value->X = x;
					m_value->Amount = amount;
				}
				~DiscreteValueWrapper() { this->!DiscreteValueWrapper(); }
				!DiscreteValueWrapper() { delete sharedPointer; }

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

				std::shared_ptr<Statistics::DiscreteValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}




