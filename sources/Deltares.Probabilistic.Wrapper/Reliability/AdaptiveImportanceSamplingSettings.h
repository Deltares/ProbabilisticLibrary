#pragma once

#include "ClusterSettings.h"
#include "../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSamplingSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
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

			public ref class AdaptiveImportanceSamplingSettings : IHasRunSettings, IHasStochastSetting
			{
			private:
				SharedPointerProvider<Reliability::AdaptiveImportanceSamplingSettings>* shared = nullptr;
				Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
			public:
				AdaptiveImportanceSamplingSettings()
				{
					shared = new SharedPointerProvider(new Reliability::AdaptiveImportanceSamplingSettings());
					shared->object->StartPointSettings = StartPointSettings->GetSettings();
					shared->object->RunSettings = RunSettings->GetSettings();
				}
				AdaptiveImportanceSamplingSettings(std::shared_ptr<Reliability::AdaptiveImportanceSamplingSettings> settings)
				{
					shared = new SharedPointerProvider(settings);
				}
				~AdaptiveImportanceSamplingSettings() { this->!AdaptiveImportanceSamplingSettings(); }
				!AdaptiveImportanceSamplingSettings() { delete shared; }

				property int MaximumSamples
				{
					int get() { return shared->object->MaximumSamples; }
					void set(int value) { shared->object->MaximumSamples = value; }
				}

				property int MaximumSamplesNoResult
				{
					int get() { return shared->object->MaximumSamplesNoResult; }
					void set(int value) { shared->object->MaximumSamplesNoResult = value; }
				}

				property double VarianceFactor
				{
					double get() { return shared->object->VarianceFactor; }
					void set(double value) { shared->object->VarianceFactor = value; }
				}

				property double FractionFailed
				{
					double get() { return shared->object->FractionFailed; }
					void set(double value) { shared->object->FractionFailed = value; }
				}

				property int MinimumFailedSamples
				{
					int get() { return shared->object->MinimumFailedSamples; }
					void set(int value) { shared->object->MinimumFailedSamples = value; }
				}

				property int MinVarianceLoops
				{
					int get() { return shared->object->MinVarianceLoops; }
					void set(int value) { shared->object->MinVarianceLoops = value; }
				}

				property int MaxVarianceLoops
				{
					int get() { return shared->object->MaxVarianceLoops; }
					void set(int value) { shared->object->MaxVarianceLoops = value; }
				}

				property bool Clustering
				{
					bool get() { return shared->object->Clustering; }
					void set(bool value) { shared->object->Clustering = value; }
				}

				property bool AutoMaximumSamplesNoResult
				{
					bool get() { return shared->object->AutoMaximumSamplesNoResult; }
					void set(bool value) { shared->object->AutoMaximumSamplesNoResult = value; }
				}

				property double EpsWeightSample
				{
					double get() { return shared->object->EpsWeightSample; }
					void set(double value) { shared->object->EpsWeightSample = value; }
				}

				property double LoopVarianceIncrement
				{
					double get() { return shared->object->LoopVarianceIncrement; }
					void set(double value) { shared->object->LoopVarianceIncrement = value; }
				}

				property double MaxBeta
				{
					double get() { return shared->object->MaxBeta; }
					void set(double value) { shared->object->MaxBeta = value; }
				}

				property double RestartStepSize
				{
					double get() { return shared->object->RestartStepSize; }
					void set(double value) { shared->object->RestartStepSize = value; }
				}

				property bool StartPointOnLimitState
				{
					bool get() { return shared->object->StartPointOnLimitState; }
					void set(bool value) { shared->object->StartPointOnLimitState = value; }
				}

				virtual property Wrappers::RunSettings^ RunSettings
				{
					Wrappers::RunSettings^ get() { return runSettings; }
					void set(Wrappers::RunSettings^ value) { runSettings = value; }
				}

				StartPointCalculatorSettings^ StartPointSettings = gcnew Wrappers::StartPointCalculatorSettings();

				ClusterSettings^ ClusterSettings = gcnew Wrappers::ClusterSettings();

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

				std::shared_ptr<Reliability::AdaptiveImportanceSamplingSettings> GetSettings()
				{
					shared->object->StochastSet->stochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					shared->object->StartPointSettings = StartPointSettings->GetSettings();
					shared->object->RunSettings = RunSettings->GetSettings();
					shared->object->ClusterSettings = ClusterSettings->GetSettings();

					return shared->object;
				}
			};
		}
	}
}



