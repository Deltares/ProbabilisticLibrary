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
				Statistics::Wrappers::Stochast^ parameter = gcnew Statistics::Wrappers::Stochast();
				SharedPointerProvider<Models::StochastPointAlpha>* shared = nullptr;

			public:
				StochastPointAlpha()
				{
					shared = new SharedPointerProvider(new Models::StochastPointAlpha());
				}
				StochastPointAlpha(std::shared_ptr<Models::StochastPointAlpha> alpha)
				{
					shared = new SharedPointerProvider(alpha);
				}
				~StochastPointAlpha() { this->!StochastPointAlpha(); }
				!StochastPointAlpha() { delete shared; }

				property Statistics::Wrappers::Stochast^ Parameter
				{
					Statistics::Wrappers::Stochast^ get() { return parameter; }
					void set(Statistics::Wrappers::Stochast^ value) { parameter = value; }
				}

				property double Alpha
				{
					double get() { return shared->object->Alpha; }
					void set(double value) { shared->object->Alpha = value; }
				}

				property double AlphaCorrelated
				{
					double get() { return shared->object->AlphaCorrelated; }
					void set(double value) { shared->object->AlphaCorrelated = value; }
				}

				property double U
				{
					double get() { return shared->object->U; }
					void set(double value) { shared->object->U = value; }
				}

				property double X
				{
					double get() { return shared->object->X; }
					void set(double value) { shared->object->X = value; }
				}

				property double InfluenceFactor
				{
					double get() { return shared->object->InfluenceFactor; }
					void set(double value) { shared->object->InfluenceFactor = value; }
				}

                std::shared_ptr<Models::StochastPointAlpha> GetNativeAlpha()
				{
                    shared->object->Stochast = this->Parameter->GetStochast();
                    return shared->object;
				}
			};
		}
	}
}

