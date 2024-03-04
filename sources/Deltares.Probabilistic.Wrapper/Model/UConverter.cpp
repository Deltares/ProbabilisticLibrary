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
				std::vector<double> uValues(alphas->Length);

				for (int i = 0; i < alphas->Length; i++)
				{
					if (beta != 0) 
					{
						uValues[i] = -beta * alphas[i];
					}
					else
					{
						uValues[i] = -alphas[i];
					}
				}

				std::shared_ptr<Models::Sample> sample = std::make_shared<Models::Sample>(uValues);

				std::shared_ptr<Models::StochastPoint> stochastPoint = shared->object->GetStochastPoint(sample, beta);

				stochastPoint->updateInfluenceFactors();

				return gcnew StochastPoint(stochastPoint, stochasts);
			}
		}
	}
}
