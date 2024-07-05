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

			public ref class FragilityValue
			{
			private:
				SharedPointerProvider<Statistics::FragilityValue>* shared = nullptr;
			public:
                FragilityValue()
                {
                    shared = new SharedPointerProvider(new Statistics::FragilityValue());
                }

                FragilityValue(std::shared_ptr<Statistics::FragilityValue> fragilityValue)
                {
                    shared = new SharedPointerProvider(fragilityValue);
                }

                ~FragilityValue() { this->!FragilityValue(); }
				!FragilityValue() { delete shared; }

				property double X
				{
					double get() { return shared->object->X; }
					void set(double value)
					{
					    shared->object->X = value;
                        shared->object->setDirty();
					}
				}

				property double Reliability
				{
					double get() { return shared->object->Reliability; }
					void set(double value)
					{
					    shared->object->Reliability = value;
                        shared->object->setDirty();
					}
				}

				std::shared_ptr<Statistics::FragilityValue> GetValue()
				{
					return shared->object;
				}
			};
		}
	}
}