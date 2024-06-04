#pragma once

#include "../../Deltares.Probabilistic/Reliability/NumericalIntegrationSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StartPointCalculatorSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "DesignPointMethodSettings.h"


namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class NumericalIntegrationSettings : IHasRunSettings, IHasStochastSetting
			{
			private:
				SharedPointerProvider<Reliability::NumericalIntegrationSettings>* shared = new SharedPointerProvider(new Reliability::NumericalIntegrationSettings());
				Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
			public:
				NumericalIntegrationSettings()
				{
					shared = new SharedPointerProvider(new Reliability::NumericalIntegrationSettings());
				}

				~NumericalIntegrationSettings() { this->!NumericalIntegrationSettings(); }
				!NumericalIntegrationSettings() { delete shared; }

				property Wrappers::DesignPointMethod DesignPointMethod
				{
					Wrappers::DesignPointMethod get()
					{
						switch (shared->object->designPointMethod)
						{
						case Reliability::DesignPointMethod::NearestToMean: return Wrappers::DesignPointMethod::NearestToMean;
						case Reliability::DesignPointMethod::CenterOfGravity: return Wrappers::DesignPointMethod::CentreOfGravity;
						case Reliability::DesignPointMethod::CenterOfAngles: return Wrappers::DesignPointMethod::CentreOfAngles;
						default: throw gcnew System::NotSupportedException("Design point method");
						}
					}
					void set(Wrappers::DesignPointMethod value)
					{
						switch (value)
						{
						case Wrappers::DesignPointMethod::NearestToMean: shared->object->designPointMethod = Reliability::DesignPointMethod::NearestToMean; break;
						case Wrappers::DesignPointMethod::CentreOfGravity:shared->object->designPointMethod = Reliability::DesignPointMethod::CenterOfGravity; break;
						case Wrappers::DesignPointMethod::CentreOfAngles: shared->object->designPointMethod = Reliability::DesignPointMethod::CenterOfAngles; break;
						default: throw gcnew System::NotSupportedException("Design point method");
						}
					}
				}

				virtual property Wrappers::RunSettings^ RunSettings
				{
					Wrappers::RunSettings^ get() { return runSettings; }
					void set(Wrappers::RunSettings^ value) { runSettings = value; }
				}

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				virtual Deltares::Reliability::Wrappers::StochastSettings^ GetStochastSetting(Statistics::Wrappers::Stochast^ stochast)
				{
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						if (StochastSettings[i]->Stochast == stochast)
						{
							return StochastSettings[i];
						}
					}

					return nullptr;
				}

				bool IsValid()
				{
					return shared->object->isValid();
				}

				std::shared_ptr<Reliability::NumericalIntegrationSettings> GetSettings()
				{
					shared->object->StochastSet->stochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					shared->object->runSettings = RunSettings->GetSettings();

					return shared->object;
				}

			};
		}
	}
}

