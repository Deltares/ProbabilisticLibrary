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
#include "DirectionReliabilitySettings.h"
#include "SubsetSimulationSettings.h"
#include "../../Deltares.Probabilistic/Reliability/Settings.h"
#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "../Reliability/StartPointCalculatorSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"


namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ReliabilitySettings : Models::Wrappers::IHasRunSettings, IHasStochastSetting
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::Settings>* shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::Settings());
                Models::Wrappers::RunSettings^ runSettings = gcnew Models::Wrappers::RunSettings();
            public:
                ReliabilitySettings()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::Settings());

                    shared->object->RandomSettings = RandomSettings->GetSettings();
                }

                ~ReliabilitySettings() { this->!ReliabilitySettings(); }
                !ReliabilitySettings() { delete shared; }

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

                Models::Wrappers::RandomSettings^ RandomSettings = gcnew Models::Wrappers::RandomSettings();

                property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
                }

                property double EpsilonBeta
                {
                    double get() { return shared->object->EpsilonBeta; }
                    void set(double value) { shared->object->EpsilonBeta = value; }
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

                property bool FilterAtNonConvergence
                {
                    bool get() { return shared->object->FilterAtNonConvergence; }
                    void set(bool value) { shared->object->FilterAtNonConvergence = value; }
                }

                property int MaxIterationsGrowthFactor
                {
                    int get() { return shared->object->MaxIterationsGrowthFactor; }
                    void set(int value) { shared->object->MaxIterationsGrowthFactor = value; }
                }

                property int MaximumSamplesNoResult
                {
                    int get() { return shared->object->MaximumSamplesNoResult; }
                    void set(int value) { shared->object->MaximumSamplesNoResult = value; }
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
                        switch (shared->object->sampleMethod)
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
                        case Wrappers::SampleMethod::MarkovChain: shared->object->sampleMethod = Reliability::SampleMethodType::MarkovChain; break;
                        case Wrappers::SampleMethod::AdaptiveConditional:shared->object->sampleMethod = Reliability::SampleMethodType::AdaptiveConditional; break;
                        default: throw gcnew System::NotSupportedException("Sample method");
                        }
                    }
                }

                DirectionReliabilitySettings^ DirectionalSettings = gcnew DirectionReliabilitySettings();

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

                std::shared_ptr<Reliability::Settings> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    shared->object->RunSettings = RunSettings->GetSettings();
                    shared->object->RandomSettings = RandomSettings->GetSettings();

                    return shared->object;
                }

            };
        }
    }
}

