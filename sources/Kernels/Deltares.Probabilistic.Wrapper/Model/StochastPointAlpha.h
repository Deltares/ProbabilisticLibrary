#pragma once

#include "../../Deltares.Probabilistic/Model/StochastPointAlpha.h"
#include "../Statistics/Stochast.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models;

			public ref class StochastPointAlpha
			{
			private:
				Models::StochastPointAlpha* m_alpha;
				Statistics::Wrappers::Stochast^ parameter = gcnew Statistics::Wrappers::Stochast();
				SharedPointerProvider<Models::StochastPointAlpha>* sharedPointer = new SharedPointerProvider<Models::StochastPointAlpha>();

			public:
				StochastPointAlpha()
				{
					m_alpha = new Models::StochastPointAlpha();
					sharedPointer->setSharedPointer(m_alpha);
				}
				StochastPointAlpha(std::shared_ptr<Models::StochastPointAlpha> alpha)
				{
					m_alpha = alpha.get();
					sharedPointer->setSharedPointer(alpha);
				}
				~StochastPointAlpha() { this->!StochastPointAlpha(); }
				!StochastPointAlpha() { delete sharedPointer; }

				property Statistics::Wrappers::Stochast^ Parameter
				{
					Statistics::Wrappers::Stochast^ get() { return parameter; }
					void set(Statistics::Wrappers::Stochast^ value) { parameter = value; }
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

