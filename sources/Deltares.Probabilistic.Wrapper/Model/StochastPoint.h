#pragma once

#include "ModelSample.h"
#include "StochastPointAlpha.h"
#include "../../Deltares.Probabilistic/Model/StochastPoint.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;

			public ref class StochastPoint
			{
			private:
				SharedPointerProvider<Models::StochastPoint>* shared = nullptr;

				System::Collections::Generic::List<StochastPointAlpha^>^ alphas = gcnew System::Collections::Generic::List<StochastPointAlpha^>();

			protected:

				void setStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts);

			public:
				StochastPoint()
				{
					shared = new SharedPointerProvider(new Models::StochastPoint());
				}

				StochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts)
				{
					setStochastPoint(stochastPoint, stochasts);
				}

				~StochastPoint() { this->!StochastPoint(); }
				!StochastPoint() { delete shared;	}

				property double Beta
				{
					double get() { return shared->object->Beta; }
					void set(double value) { shared->object->Beta = value; }
				}

				property System::Collections::Generic::List<StochastPointAlpha^>^ Alphas
				{
					System::Collections::Generic::List<StochastPointAlpha^>^ get() { return alphas; }
				}

                ModelSample^ GetModelSample()
				{
                    return gcnew ModelSample(shared->object->getModelSample());
				}
			};
		}
	}
}


