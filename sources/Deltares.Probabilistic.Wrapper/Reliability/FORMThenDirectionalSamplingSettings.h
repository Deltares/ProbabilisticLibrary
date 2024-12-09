// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "../../Deltares.Probabilistic/Reliability/FORMSettings.h"
#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Model/RandomSettings.h"
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

            public ref class FORMThenDirectionalSamplingSettings : IHasRunSettings
            {
            private:
                SharedPointerProvider<Reliability::FORMSettings>* sharedForm = new SharedPointerProvider(new Reliability::FORMSettings());
                SharedPointerProvider<Reliability::DirectionalSamplingSettings>* sharedDS = new SharedPointerProvider(new Reliability::DirectionalSamplingSettings());
                Wrappers::RandomSettings^ randomSettings = gcnew Wrappers::RandomSettings();

                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();

            public:
                FORMThenDirectionalSamplingSettings() {}
                ~FORMThenDirectionalSamplingSettings() { this->!FORMThenDirectionalSamplingSettings(); }
                !FORMThenDirectionalSamplingSettings() { delete sharedForm; delete sharedDS; }

                property int MaximumIterations
                {
                    int get() { return sharedForm->object->MaximumIterations; }
                    void set(int value) { sharedForm->object->MaximumIterations = value; }
                }

                property int RelaxationLoops
                {
                    int get() { return sharedForm->object->RelaxationLoops; }
                    void set(int value) { sharedForm->object->RelaxationLoops = value; }
                }

                property double RelaxationFactor
                {
                    double get() { return sharedForm->object->RelaxationFactor; }
                    void set(double value) { sharedForm->object->RelaxationFactor = value; }
                }

                property double EpsilonBeta
                {
                    double get() { return sharedForm->object->EpsilonBeta; }
                    void set(double value) { sharedForm->object->EpsilonBeta = value; }
                }

                bool IsValid()
                {
                    return sharedForm->object->isValid() && sharedDS->object->isValid();
                }

                property bool FilterAtNonConvergence
                {
                    bool get() { return sharedForm->object->FilterAtNonConvergence; }
                    void set(bool value) { sharedForm->object->FilterAtNonConvergence = value; }
                }

                property int MinimumDirections
                {
                    int get() { return sharedDS->object->MinimumDirections; }
                    void set(int value) { sharedDS->object->MinimumDirections = value; }
                }

                property int MaximumDirections
                {
                    int get() { return sharedDS->object->MaximumDirections; }
                    void set(int value) { sharedDS->object->MaximumDirections = value; }
                }

                property double VariationCoefficient
                {
                    double get() { return sharedDS->object->VariationCoefficient; }
                    void set(double value) { sharedDS->object->VariationCoefficient = value; }
                }

                property Wrappers::DesignPointMethod DesignPointMethod
                {
                    Wrappers::DesignPointMethod get()
                    {
                        switch (sharedDS->object->designPointMethod)
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
                        case Wrappers::DesignPointMethod::NearestToMean: sharedDS->object->designPointMethod = Reliability::DesignPointMethod::NearestToMean; break;
                        case Wrappers::DesignPointMethod::CentreOfGravity: sharedDS->object->designPointMethod = Reliability::DesignPointMethod::CenterOfGravity; break;
                        case Wrappers::DesignPointMethod::CentreOfAngles: sharedDS->object->designPointMethod = Reliability::DesignPointMethod::CenterOfAngles; break;
                        default: throw gcnew System::NotSupportedException("Design point method");
                        }
                    }
                }

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set (Wrappers::RunSettings^ value) { runSettings = value; }
                }

                DirectionReliabilitySettings^ DirectionalSettings = gcnew DirectionReliabilitySettings();

                StartPointCalculatorSettings^ StartPointCalculatorSettings = gcnew Wrappers::StartPointCalculatorSettings();
                Wrappers::GradientSettings^ GradientCalculatorSettings = gcnew Wrappers::GradientSettings();

                System::Collections::Generic::List<Wrappers::StochastSettings^>^ StochastSettings = gcnew System::Collections::Generic::List<Wrappers::StochastSettings^>();

                std::shared_ptr<Reliability::FORMSettings> GetFormSettings()
                {
                    sharedForm->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        sharedForm->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    sharedForm->object->StartPointSettings = StartPointCalculatorSettings->GetSettings();
                    sharedForm->object->StartPointSettings->StochastSet = sharedForm->object->StochastSet;

                    sharedForm->object->GradientSettings = GradientCalculatorSettings->GetSettings();
                    sharedForm->object->RunSettings = RunSettings->GetSettings();

                    return sharedForm->object;
                }

                std::shared_ptr<Reliability::DirectionalSamplingSettings> GetDsSettings()
                {
                    sharedDS->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        sharedDS->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    sharedDS->object->runSettings = RunSettings->GetSettings();
                    sharedDS->object->DirectionSettings = DirectionalSettings->GetSettings();
                    sharedDS->object->randomSettings = randomSettings->GetSettings();

                    return sharedDS->object;
                }

                virtual property Wrappers::RandomSettings^ RandomSettings
                {
                    Wrappers::RandomSettings^ get() { return randomSettings; }
                    void set(Wrappers::RandomSettings^ value) { randomSettings = value; }
                }

            };
        }
    }
}


