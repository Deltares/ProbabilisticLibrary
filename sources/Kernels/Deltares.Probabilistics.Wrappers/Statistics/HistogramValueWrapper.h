#pragma once

#include "../../Deltares.Probabilistic.Kernels/Statistics/HistogramValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class HistogramValueWrapper
			{
			private:
				Statistics::HistogramValue* m_value;
				SharedPointerProvider<Statistics::HistogramValue>* sharedPointer = new SharedPointerProvider<Statistics::HistogramValue>();
			public:
				HistogramValueWrapper()
				{
					m_value = new Statistics::HistogramValue();
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







