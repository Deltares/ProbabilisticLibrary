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

				HistogramValue(double lowerBound, double upperBound, double amount)
				{
					this->LowerBound = lowerBound;
					this->UpperBound = upperBound;
					this->Amount = amount;
				}

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

				property double NormalizedAmount
				{
					double get() { return shared->object->NormalizedAmount; }
					void set(double value) { shared->object->NormalizedAmount = value; }
				}

				property double CumulativeNormalizedAmount
				{
					double get() { return shared->object->CumulativeNormalizedAmount; }
					void set(double value) { shared->object->CumulativeNormalizedAmount = value; }
				}

				// TODO: remove after table distribution available in c++
				bool Contains(double x)
				{
					return shared->object->contains(x);
				}

				// TODO: remove after table distribution available in c++
				bool IsValid()
				{
					return shared->object->isValid();
				}

				// TODO: remove after table distribution available in c++
				property double Size
				{
					double get() { return shared->object->getSize(); }
				}

				property double Center
				{
					double get() { return shared->object->getCenter(); }
				}

				std::shared_ptr<Statistics::HistogramValue> GetValue()
				{
					return shared->object;
				}
			};
		}
	}
}







