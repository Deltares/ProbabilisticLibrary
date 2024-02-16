#pragma once

#include "../../Deltares.Probabilistic/Statistics/HistogramValue.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Statistics
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class HistogramValue
			{
			private:
				SharedPointerProvider<Statistics::HistogramValue>* shared = new SharedPointerProvider(new Statistics::HistogramValue());
			public:
				HistogramValue() { }
				~HistogramValue() { this->!HistogramValue(); }
				!HistogramValue() { delete shared; }

				property double LowerBound
				{
					double get() { return shared->object->LowerBound; }
					void set(double value) { shared->object->LowerBound = value; }
				}

				property double UpperBound
				{
					double get() { return shared->object->UpperBound; }
					void set(double value) { shared->object->UpperBound = value; }
				}

				property double Amount
				{
					double get() { return shared->object->Amount; }
					void set(double value) { shared->object->Amount = value; }
				}

				std::shared_ptr<Statistics::HistogramValue> GetValue()
				{
					return shared->object;
				}
			};
		}
	}
}







