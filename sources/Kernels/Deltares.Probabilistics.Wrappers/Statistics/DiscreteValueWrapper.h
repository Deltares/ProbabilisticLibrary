#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/DiscreteValue.h"

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

			public:
				DiscreteValueWrapper()
				{
					m_value = new Statistics::DiscreteValue();
				}
				DiscreteValueWrapper(double x, double amount)
				{
					m_value = new Statistics::DiscreteValue();
					m_value->X = x;
					m_value->Amount = amount;
				}
				~DiscreteValueWrapper() { this->!DiscreteValueWrapper(); }
				!DiscreteValueWrapper() { delete m_value; }

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

				Statistics::DiscreteValue* GetValue()
				{
					return m_value;
				}
			};
		}
	}
}




