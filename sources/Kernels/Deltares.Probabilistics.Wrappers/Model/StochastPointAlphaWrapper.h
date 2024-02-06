#pragma once

#include "../../Deltares.Probabilistic.Kernels/Model/StochastPointAlpha.h"
#include "../Statistics/StochastWrapper.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models;

			public ref class StochastPointAlphaWrapper
			{
			private:
				StochastPointAlpha* m_alpha;
				Statistics::Wrappers::StochastWrapper^ parameter = gcnew Statistics::Wrappers::StochastWrapper();
				SharedPointerProvider<StochastPointAlpha>* sharedPointer = new SharedPointerProvider<StochastPointAlpha>();

			public:
				StochastPointAlphaWrapper()
				{
					m_alpha = new StochastPointAlpha();
					sharedPointer->setSharedPointer(m_alpha);
				}
				StochastPointAlphaWrapper(std::shared_ptr<StochastPointAlpha> alpha)
				{
					m_alpha = alpha.get();
					sharedPointer->setSharedPointer(alpha);
				}
				~StochastPointAlphaWrapper() { this->!StochastPointAlphaWrapper(); }
				!StochastPointAlphaWrapper() { delete sharedPointer; }

				property Statistics::Wrappers::StochastWrapper^ Parameter
				{
					Statistics::Wrappers::StochastWrapper^ get() { return parameter; }
					void set(Statistics::Wrappers::StochastWrapper^ value) { parameter = value; }
				}

				property double Alpha
				{
					double get() { return m_alpha->Alpha; }
					void set(double value) { m_alpha->Alpha = value; }
				}

				property double AlphaCorrelated
				{
					double get() { return m_alpha->AlphaCorrelated; }
					void set(double value) { m_alpha->AlphaCorrelated = value; }
				}

				property double U
				{
					double get() { return m_alpha->U; }
					void set(double value) { m_alpha->U = value; }
				}

				property double X
				{
					double get() { return m_alpha->X; }
					void set(double value) { m_alpha->X = value; }
				}

				property double InfluenceFactor
				{
					double get() { return m_alpha->InfluenceFactor; }
					void set(double value) { m_alpha->InfluenceFactor = value; }
				}
			};
		}
	}
}

