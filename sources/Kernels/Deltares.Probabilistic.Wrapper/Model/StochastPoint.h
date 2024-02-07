#pragma once

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
				Models::StochastPoint* m_stochastPoint;
				SharedPointerProvider<Models::StochastPoint>* sharedPointer = new SharedPointerProvider<Models::StochastPoint>();

				System::Collections::Generic::List<StochastPointAlpha^>^ alphas = gcnew System::Collections::Generic::List<StochastPointAlpha^>();

			protected:

				void SetStochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint, System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts);

			public:
				StochastPoint()
				{
					m_stochastPoint = new Models::StochastPoint();
					sharedPointer->setSharedPointer(m_stochastPoint);
				}
				StochastPoint(std::shared_ptr<Models::StochastPoint> stochastPoint)
				{
					m_stochastPoint = stochastPoint.get();
					sharedPointer->setSharedPointer(stochastPoint);
				}
				~StochastPoint() { this->!StochastPoint(); }
				!StochastPoint() { delete sharedPointer;	}

				property double Beta
				{
					double get() { return m_stochastPoint->Beta; }
					void set(double value) { m_stochastPoint->Beta = value; }
				}

				property System::Collections::Generic::List<StochastPointAlpha^>^ Alphas
				{
					System::Collections::Generic::List<StochastPointAlpha^>^ get() { return alphas; }
				}
			};
		}
	}
}


