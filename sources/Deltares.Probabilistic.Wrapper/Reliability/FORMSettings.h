#pragma once
#include "../../Deltares.Probabilistic/Reliability/FORMSettings.h"
#include "../Model/GradientSettings.h"
#include "../Model/RunSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "StartPointCalculatorSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class FORMSettings : IHasRunSettings
			{
			private:
				SharedPointerProvider<Reliability::FORMSettings>* shared = new SharedPointerProvider(new Reliability::FORMSettings());

				Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();

			public:
				FORMSettings() {}
				~FORMSettings() { this->!FORMSettings(); }
				!FORMSettings() { delete shared; }

				property int MaximumIterations
				{
					int get() { return shared->object->MaximumIterations; }
					void set(int value) { shared->object->MaximumIterations = value; }
				}

				property int RelaxationLoops
				{
					int get() { return shared->object->RelaxationLoops; }
					void set(int value) { shared->object->RelaxationLoops = value; }
				}

				property double RelaxationFactor
				{
					double get() { return shared->object->RelaxationFactor; }
					void set(double value) { shared->object->RelaxationFactor = value; }
				}

				property double EpsilonBeta
				{
					double get() { return shared->object->EpsilonBeta; }
					void set(double value) { shared->object->EpsilonBeta = value; }
				}

				bool IsValid()
				{
					return shared->object->isValid();
				}

				property bool FilterAtNonConvergence
				{
					bool get() { return shared->object->FilterAtNonConvergence; }
					void set(bool value) { shared->object->FilterAtNonConvergence = value; }
				}


				virtual property Wrappers::RunSettings^ RunSettings
				{
					Wrappers::RunSettings^ get() { return runSettings; }
					void set (Wrappers::RunSettings^ value) { runSettings = value; }
				}

				StartPointCalculatorSettings^ StartPointCalculatorSettings = gcnew Wrappers::StartPointCalculatorSettings();
				Wrappers::GradientSettings^ GradientCalculatorSettings = gcnew Wrappers::GradientSettings();

				System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

				std::shared_ptr<Reliability::FORMSettings> GetSettings()
				{
					shared->object->StochastSet->StochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->StochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					shared->object->StartPointSettings = StartPointCalculatorSettings->GetSettings();
					shared->object->StartPointSettings->StochastSet = shared->object->StochastSet;

					shared->object->GradientSettings = GradientCalculatorSettings->GetSettings();
					shared->object->RunSettings = RunSettings->GetSettings();

					return shared->object;
				}
			};
		}
	}
}


