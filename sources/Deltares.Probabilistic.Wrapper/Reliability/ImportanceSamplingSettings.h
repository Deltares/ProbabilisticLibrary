#pragma once

#include "../../Deltares.Probabilistic/Reliability/ImportanceSamplingSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
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

			public ref class ImportanceSamplingSettings : IHasRunSettings, IHasRandomSettings, IHasStochastSetting
			{
			private:
				SharedPointerProvider<Reliability::ImportanceSamplingSettings>* shared = nullptr;
				Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
				Wrappers::RandomSettings^ randomSettings = gcnew Wrappers::RandomSettings();
			public:
				ImportanceSamplingSettings()
				{
					shared = new SharedPointerProvider(new Reliability::ImportanceSamplingSettings());
					shared->object->randomSettings = RandomSettings->GetSettings();
				}
				ImportanceSamplingSettings(std::shared_ptr<Reliability::ImportanceSamplingSettings> settings)
				{
					shared = new SharedPointerProvider(settings);
				}
				~ImportanceSamplingSettings() { this->!ImportanceSamplingSettings(); }
				!ImportanceSamplingSettings() { delete shared; }

				ImportanceSamplingSettings^ Clone()
				{
					return gcnew ImportanceSamplingSettings(shared->object->clone());
				}

				property int MinimumSamples
				{
					int get() { return shared->object->MinimumSamples; }
					void set(int value) { shared->object->MinimumSamples = value; }
				}

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

				property double VariationCoefficient
				{
					double get() { return shared->object->VariationCoefficient; }
					void set(double value) { shared->object->VariationCoefficient = value; }
				}

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

				property int Counter
				{
					int get() { return shared->object->Counter; }
					void set(int value) { shared->object->Counter = value; }
				}

				property bool Clustering
				{
					bool get() { return shared->object->Clustering; }
					void set(bool value) { shared->object->Clustering = value; }
				}

				property int MaxVarianceLoops
				{
					int get() { return shared->object->MaxVarianceLoops; }
					void set(int value) { shared->object->MaxVarianceLoops = value; }
				}

				property int MaxClusters
				{
					int get() { return shared->object->MaxClusters; }
					void set(int value) { shared->object->MaxClusters = value; }
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

				property double VarianceFactor
				{
					double get() { return shared->object->VarianceFactor; }
					void set(double value) { shared->object->VarianceFactor = value; }
				}

				property bool OptimizeNumberClusters
				{
					bool get() { return shared->object->OptimizeNumberClusters; }
					void set(bool value) { shared->object->OptimizeNumberClusters = value; }
				}

				virtual property Wrappers::RandomSettings^ RandomSettings
				{
					Wrappers::RandomSettings^ get() { return randomSettings; }
					void set(Wrappers::RandomSettings^ value) { randomSettings = value; }
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

				std::shared_ptr<Reliability::ImportanceSamplingSettings> GetSettings()
				{
					shared->object->StochastSet->stochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					shared->object->randomSettings = RandomSettings->GetSettings();
					shared->object->RunSettings = RunSettings->GetSettings();

					return shared->object;
				}
			};
		}
	}
}


