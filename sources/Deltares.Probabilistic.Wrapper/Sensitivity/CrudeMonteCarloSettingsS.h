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

                property bool CalculateCorrelations
                {
                    bool get() { return shared->object->CalculateCorrelations; }
                    void set(bool value) { shared->object->CalculateCorrelations = value; }
                }

                property bool CalculateInputCorrelations
                {
                    bool get() { return shared->object->CalculateInputCorrelations; }
                    void set(bool value) { shared->object->CalculateInputCorrelations = value; }
                }

                property int Samples
                {
                    int get() { return shared->object->MaximumSamples; }
                    void set(int value) { shared->object->MaximumSamples = value; }
                }

                property double VariationCoefficient
                {
                    double get() { return shared->object->VariationCoefficient; }
                    void set(double value) { shared->object->VariationCoefficient = value; }
                }

                property double ProbabilityForConvergence
                {
                    double get() { return shared->object->ProbabilityForConvergence; }
                    void set(double value) { shared->object->ProbabilityForConvergence = value; }
                }

                property double RequiredSamples
                {
                    double get() { return shared->object->getRequiredSamples(); }
                    void set(double value) { shared->object->setRequiredSamples(value); }
                }

                property bool DeriveSamplesFromVariationCoefficient
                {
                    bool get() { return shared->object->DeriveSamplesFromVariationCoefficient; }
                    void set(bool value) { shared->object->DeriveSamplesFromVariationCoefficient = value; }
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


