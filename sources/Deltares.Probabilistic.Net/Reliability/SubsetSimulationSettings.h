// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#pragma once

#include "DesignPointMethodSettings.h"
#include "../../Deltares.Probabilistic/Reliability/SubsetSimulationSettings.h"
#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "StartPointCalculatorSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public enum class SampleMethod { MarkovChain, AdaptiveConditional };

            public ref class SubsetSimulationSettings : Models::Wrappers::IHasRunSettings, IHasStochastSetting, Models::Wrappers::IHasRandomSettings
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::SubsetSimulationSettings>* shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::SubsetSimulationSettings());

                Models::Wrappers::RunSettings^ runSettings = gcnew Models::Wrappers::RunSettings();
                Models::Wrappers::RandomSettings^ randomSettings = gcnew Models::Wrappers::RandomSettings();

            public:
                SubsetSimulationSettings() {}
                ~SubsetSimulationSettings() { this->!SubsetSimulationSettings(); }
                !SubsetSimulationSettings() { delete shared; }

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

                property double VariationCoefficientFailure
                {
                    double get() { return shared->object->VariationCoefficient; }
                    void set(double value) { shared->object->VariationCoefficient = value; }
                }

                property double MarkovChainDeviation
                {
                    double get() { return shared->object->MarkovChainDeviation; }
                    void set(double value) { shared->object->MarkovChainDeviation = value; }
                }

                property double SubsetFraction
                {
                    double get() { return shared->object->SubsetFraction; }
                    void set(double value) { shared->object->SubsetFraction = value; }
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

                property Wrappers::SampleMethod SampleMethod
                {
                    Wrappers::SampleMethod get()
                    {
                        switch (shared->object->SampleMethod)
                        {
                        case Reliability::SampleMethodType::MarkovChain: return Wrappers::SampleMethod::MarkovChain;
                        case Reliability::SampleMethodType::AdaptiveConditional: return Wrappers::SampleMethod::AdaptiveConditional;
                        default: throw gcnew System::NotSupportedException("Sample method");
                        }
                    }
                    void set(Wrappers::SampleMethod value)
                    {
                        switch (value)
                        {
                        case Wrappers::SampleMethod::MarkovChain: shared->object->SampleMethod = Reliability::SampleMethodType::MarkovChain; break;
                        case Wrappers::SampleMethod::AdaptiveConditional:shared->object->SampleMethod = Reliability::SampleMethodType::AdaptiveConditional; break;
                        default: throw gcnew System::NotSupportedException("Sample method");
                        }
                    }
                }

                virtual property Models::Wrappers::RandomSettings^ RandomSettings
                {
                    Models::Wrappers::RandomSettings^ get() { return randomSettings; }
                    void set(Models::Wrappers::RandomSettings^ value) { randomSettings = value; }
                }

                virtual property Models::Wrappers::RunSettings^ RunSettings
                {
                    Models::Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Models::Wrappers::RunSettings^ value) { runSettings = value; }
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

                std::shared_ptr<Reliability::SubsetSimulationSettings> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    shared->object->RunSettings = RunSettings->GetSettings();
                    shared->object->randomSettings = RandomSettings->GetSettings();

                    return shared->object;
                }
            };
        }
    }
}



