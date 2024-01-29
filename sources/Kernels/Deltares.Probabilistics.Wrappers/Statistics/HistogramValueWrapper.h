#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/HistogramValue.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			public ref class HistogramValueWrapper
			{
			private:
				Statistics::HistogramValue* m_value;

			public:
				HistogramValueWrapper()
				{
					m_value = new Statistics::HistogramValue();
				}
				~HistogramValueWrapper() { this->!HistogramValueWrapper(); }
				!HistogramValueWrapper() { delete m_value; }

				property double LowerBound
				{
					double get() { return m_value->LowerBound; }
					void set(double value) { m_value->LowerBound = value; }
				}

				property double UpperBound
				{
					double get() { return m_value->UpperBound; }
					void set(double value) { m_value->UpperBound = value; }
				}

				property double Amount
				{
					double get() { return m_value->Amount; }
					void set(double value) { m_value->Amount = value; }
				}

				Statistics::HistogramValue* GetValue()
				{
					return m_value;
				}
			};
		}
	}
}







