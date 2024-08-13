#pragma once

#include "../../Deltares.Probabilistic/Sensitivity/CrudeMonteCarloSettingsS.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Sensitivity
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class CrudeMonteCarloSettingsS : IHasRunSettings
            {
            private:
                SharedPointerProvider<Sensitivity::CrudeMonteCarloSettingsS>* shared = new SharedPointerProvider(new Sensitivity::CrudeMonteCarloSettingsS());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                CrudeMonteCarloSettingsS()
                {
                    shared->object->randomSettings = RandomSettings->GetSettings();
                }
                ~CrudeMonteCarloSettingsS() { this->!CrudeMonteCarloSettingsS(); }
                !CrudeMonteCarloSettingsS() { delete shared; }

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

                property double VariationCoefficient
                {
                    double get() { return shared->object->VariationCoefficient; }
                    void set(double value) { shared->object->VariationCoefficient = value; }
                }

                property double NumberSamplesProbability
                {
                    double get() { return shared->object->NumberSamplesProbability; }
                    void set(double value) { shared->object->NumberSamplesProbability = value; }
                }

                Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                System::Collections::Generic::List<Reliability::Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Reliability::Wrappers::StochastSettings^>();

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Sensitivity::CrudeMonteCarloSettingsS> GetSettings()
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


