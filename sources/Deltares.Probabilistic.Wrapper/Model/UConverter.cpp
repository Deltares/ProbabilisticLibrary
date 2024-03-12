#include "UConverter.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			UConverter::UConverter(System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts, Statistics::Wrappers::CorrelationMatrix^ correlationMatrix)
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

			bool UConverter::IsVaryingStochast(int index)
			{
				return shared->object->isVaryingStochast(index);
			}

			StochastPoint^ UConverter::GetBaseStochastPoint(double beta, array<double>^ alphas, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts)
			{
				std::shared_ptr<Models::StochastPoint> stochastPoint = shared->object->GetStochastPoint(beta, NativeSupport::toNative(alphas));

				return gcnew StochastPoint(stochastPoint, stochasts);
			}
		}
	}
}
