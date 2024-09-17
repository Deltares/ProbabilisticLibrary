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
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
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


