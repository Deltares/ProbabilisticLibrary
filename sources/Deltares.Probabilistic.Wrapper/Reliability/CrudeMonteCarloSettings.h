#pragma once

#include "../../Deltares.Probabilistic/Reliability/CrudeMonteCarloSettings.h"
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

            public ref class CrudeMonteCarloSettings : IHasRunSettings
            {
            private:
                SharedPointerProvider<Reliability::CrudeMonteCarloSettings>* shared = new SharedPointerProvider(new Reliability::CrudeMonteCarloSettings());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                CrudeMonteCarloSettings()
                {
                    shared->object->randomSettings = RandomSettings->GetSettings();
                }
                ~CrudeMonteCarloSettings() { this->!CrudeMonteCarloSettings(); }
                !CrudeMonteCarloSettings() { delete shared; }

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

                Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

                bool IsValid()
                {
                    return shared->object->isValid();
                }

                std::shared_ptr<Reliability::CrudeMonteCarloSettings> GetSettings()
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


