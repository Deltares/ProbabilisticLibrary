#pragma once

#include "../Utils/NativeSupport.h"
#include "../Utils/SharedPointerProvider.h"
#include "../../Deltares.Probabilistic/Model/UConverter.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Model/StochastPoint.h"

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
				UConverter(System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts, Statistics::Wrappers::CorrelationMatrix^ correlationMatrix);

				virtual property int VaryingStochastCount
				{
					int get() { return shared->object->getVaryingStochastCount(); }
				}

				bool IsVaryingStochast(int index);

				virtual Wrappers::StochastPoint^ GetBaseStochastPoint(double beta, array<double>^ alphas, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts);

				std::shared_ptr<Models::UConverter> GetConverter()
				{
					return shared->object;
				}
			};
		}
	}
}

