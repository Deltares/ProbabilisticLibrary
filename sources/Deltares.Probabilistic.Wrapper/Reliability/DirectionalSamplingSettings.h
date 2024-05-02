#pragma once
#include "DesignPointMethodSettings.h"
#include "DirectionReliabilitySettings.h"

#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"

namespace Deltares
{
	namespace Reliability
	{
		namespace Wrappers
		{
			using namespace Deltares::Utils::Wrappers;
			using namespace Deltares::Models::Wrappers;

			public ref class DirectionalSamplingSettings : IHasRandomSettings, IHasRunSettings, IHasStochastSetting
			{
			private:
				SharedPointerProvider<Reliability::DirectionalSamplingSettings>* shared = new SharedPointerProvider<Reliability::DirectionalSamplingSettings>(new Reliability::DirectionalSamplingSettings());

				Wrappers::RandomSettings^ randomSettings = gcnew Wrappers::RandomSettings();
				Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();

			public:
				DirectionalSamplingSettings() {}
				~DirectionalSamplingSettings() { this->!DirectionalSamplingSettings(); }
				!DirectionalSamplingSettings() { delete shared; }

				property int MinimumSamples
				{
					int get() { return shared->object->MinimumDirections; }
					void set(int value) { shared->object->MinimumDirections = value; }
				}

				property int MaximumSamples
				{
					int get() { return shared->object->MaximumDirections; }
					void set(int value) { shared->object->MaximumDirections = value; }
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

				DirectionReliabilitySettings^ DirectionalSettings = gcnew DirectionReliabilitySettings();

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

				std::shared_ptr<Reliability::DirectionalSamplingSettings> GetSettings()
				{
					shared->object->randomSettings = RandomSettings->GetSettings();
					shared->object->DirectionSettings = DirectionalSettings->GetSettings();
					shared->object->runSettings = RunSettings->GetSettings();

					shared->object->StochastSet->stochastSettings.clear();
					for (int i = 0; i < StochastSettings->Count; i++)
					{
						shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
					}

					return shared->object;
				}
			};
		}
	}
}


