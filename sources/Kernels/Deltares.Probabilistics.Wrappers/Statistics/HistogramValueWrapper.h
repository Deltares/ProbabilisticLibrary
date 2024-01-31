#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/HistogramValue.h"
#include "../../Deltares.Probabilistic.Kernels/Utils/SharedPointerProvider.h"

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
				Utils::SharedPointerProvider<Statistics::HistogramValue>* sharedPointer;
			public:
				HistogramValueWrapper()
				{
					m_value = new Statistics::HistogramValue();
					sharedPointer = new Utils::SharedPointerProvider<Statistics::HistogramValue>();
				}
				~HistogramValueWrapper() { this->!HistogramValueWrapper(); }
				!HistogramValueWrapper() { delete sharedPointer; }

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

				std::shared_ptr<Statistics::HistogramValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}







