// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
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
#include "../Model/GradientSettings.h"
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

            public ref class Settings : IHasRandomSettings, IHasRunSettings, IHasStochastSetting
            {
            private:
                SharedPointerProvider<Reliability::Settings>* shared = new SharedPointerProvider<Reliability::Settings>(new Reliability::Settings());

                Wrappers::GradientSettings^ gradientSettings = gcnew Wrappers::GradientSettings();
                Wrappers::RandomSettings^ randomSettings = gcnew Wrappers::RandomSettings();
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();

            public:
                Settings() {}
                ~Settings() { this->!Settings(); }
                !Settings() { delete shared; }

                virtual property int MinimumSamples
                {
                    int get() { return shared->object->MinimumDirections; }
                    void set(int value) { shared->object->MinimumDirections = value; }
                }

                virtual property int MaximumSamples
                {
                    int get() { return shared->object->MaximumDirections; }
                    void set(int value) { shared->object->MaximumDirections = value; }
                }

                virtual property int MaximumSamplesNoResult
                {
                    int get() { return shared->object->MaximumSamplesNoResult; }
                    void set(int value) { shared->object->MaximumSamplesNoResult = value; }
                }

                virtual property double VariationCoefficient
                {
                    double get() { return shared->object->VariationCoefficient; }
                    void set(double value) { shared->object->VariationCoefficient = value; }
                }

                virtual property int MinimumIterations
                {
                    int get() { return shared->object->MinimumIterations; }
                    void set(int value) { shared->object->MinimumIterations = value; }
                }

                virtual property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
                }

                virtual property int MinimumVarianceLoops
                {
                    int get() { return shared->object->MinimumVarianceLoops; }
                    void set(int value) { shared->object->MinimumVarianceLoops = value; }
                }

                virtual property int MaximumVarianceLoops
                {
                    int get() { return shared->object->MaximumVarianceLoops; }
                    void set(int value) { shared->object->MaximumVarianceLoops = value; }
                }

                virtual property double StartValue
                {
                    double get() { return shared->object->StartValue; }
                    void set(double value) { shared->object->StartValue = value; }
                }

                virtual property double VarianceFactor
                {
                    double get() { return shared->object->VarianceFactor; }
                    void set(double value) { shared->object->VarianceFactor = value; }
                }

                virtual property double FractionFailed
                {
                    double get() { return shared->object->FractionFailed; }
                    void set(double value) { shared->object->FractionFailed = value; }
                }

                virtual property int Intervals
                {
                    int get() { return shared->object->Intervals; }
                    void set(int value) { shared->object->Intervals = value; }
                }

                virtual property int RelaxationLoops
                {
                    int get() { return shared->object->RelaxationLoops; }
                    void set(int value) { shared->object->RelaxationLoops = value; }
                }

                virtual property double RelaxationFactor
                {
                    double get() { return shared->object->RelaxationFactor; }
                    void set(double value) { shared->object->RelaxationFactor = value; }
                }

                virtual property double EpsilonBeta
                {
                    double get() { return shared->object->EpsilonBeta; }
                    void set(double value) { shared->object->EpsilonBeta = value; }
                }

                virtual property bool FilterAtNonConvergence
                {
                    bool get() { return shared->object->FilterAtNonConvergence; }
                    void set(bool value) { shared->object->FilterAtNonConvergence = value; }
                }

                virtual property int MaxIterationsGrowthFactor
                {
                    int get() { return shared->object->MaxIterationsGrowthFactor; }
                    void set(int value) { shared->object->MaxIterationsGrowthFactor = value; }
                }

                virtual property double MarkovChainDeviation
                {
                    double get() { return shared->object->MarkovChainDeviation; }
                    void set(double value) { shared->object->MarkovChainDeviation = value; }
                }

                virtual property double SubsetFraction
                {
                    double get() { return shared->object->SubsetFraction; }
                    void set(double value) { shared->object->SubsetFraction = value; }
                }

                virtual property Wrappers::DesignPointMethod DesignPointMethod
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

                virtual property Wrappers::SampleMethod SampleMethod
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

                virtual property Wrappers::GradientSettings^ GradientSettings
                {
                    Wrappers::GradientSettings^ get() { return gradientSettings; }
                    void set(Wrappers::GradientSettings^ value) { gradientSettings = value; }
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

                std::shared_ptr<Reliability::Settings> GetSettings()
                {
                    shared->object->GradientSettings = GradientSettings->GetSettings();
                    shared->object->RandomSettings = RandomSettings->GetSettings();
                    shared->object->DirectionSettings = DirectionalSettings->GetSettings();
                    shared->object->RunSettings = RunSettings->GetSettings();

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


