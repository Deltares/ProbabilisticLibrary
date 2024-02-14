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

					sharedPointer->setSharedPointer(m_value);
				}
				DiscreteValue(double x, double amount)
				{
					m_value = new Statistics::DiscreteValue(x, amount);
					m_value->X = x;
					m_value->Amount = amount;

					sharedPointer->setSharedPointer(m_value);
				}
				~DiscreteValue() { this->!DiscreteValue(); }
				!DiscreteValue()
				{
					delete sharedPointer;
				}

				property double X
				{
					double get() { return sharedPointer->getSharedPointer()->X; }
					void set(double value) { sharedPointer->getSharedPointer()->X = value; }
				}

				property double Amount
				{
					double get() { return sharedPointer->getSharedPointer()->Amount; }
					void set(double value) { sharedPointer->getSharedPointer()->Amount = value; }
				}

				property double NormalizedAmount
				{
					double get() { return sharedPointer->isInitialized() ? sharedPointer->getSharedPointer()->NormalizedAmount : nan(""); }
				}

				property double CumulativeNormalizedAmount
				{
					double get() { return sharedPointer->isInitialized() ? sharedPointer->getSharedPointer()->CumulativeNormalizedAmount : nan(""); }
				}

				std::shared_ptr<Statistics::DiscreteValue> GetValue()
				{
					return sharedPointer->getSharedPointer(m_value);
				}
			};
		}
	}
}




