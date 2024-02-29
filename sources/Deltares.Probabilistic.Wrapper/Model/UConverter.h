#pragma once

#include "../Utils/NativeSupport.h"
#include "../Utils/SharedPointerProvider.h"
#include "../../Deltares.Probabilistic/Model/UConverter.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Models;
			using namespace Deltares::Utils::Wrappers;

			public ref class UConverter
			{
			private:
				array<double>^ values = nullptr;
				System::Object^ tag = nullptr;
				SharedPointerProvider<Models::UConverter>* shared = nullptr;

			public:
				UConverter(System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts, Statistics::Wrappers::CorrelationMatrix^ correlationMatrix)
				{
					std::vector<std::shared_ptr<Statistics::Stochast>> nativeStochasts;

					for (int i = 0; i < stochasts->Count; i++)
					{
						nativeStochasts.push_back(stochasts[i]->GetStochast());
					}

					std::shared_ptr<Models::UConverter> uConverter = std::make_shared<Models::UConverter>(nativeStochasts, correlationMatrix->GetCorrelationMatrix());
					this->shared = new SharedPointerProvider(uConverter);

					this->shared->object->initializeForRun();
				}

				virtual property int VaryingStochastCount
				{
					int get() { return shared->object->getVaryingStochastCount(); }
				}

				std::shared_ptr<Models::UConverter> GetConverter()
				{
					return shared->object;
				}
			};
		}
	}
}

