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

#include "../../Deltares.Probabilistic/Uncertainty/DirectionalSamplingSettingsS.h"
#include "../Model/RunSettings.h"
#include "../Model/RandomSettings.h"
#include "../Statistics/ProbabilityValue.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Uncertainty
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class DirectionalSamplingSettingsS : IHasRunSettings
            {
            private:
                SharedPointerProvider<Uncertainty::DirectionalSamplingSettingsS>* shared = new SharedPointerProvider(new Uncertainty::DirectionalSamplingSettingsS());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                DirectionalSamplingSettingsS() {}
                ~DirectionalSamplingSettingsS() { this->!DirectionalSamplingSettingsS(); }
                !DirectionalSamplingSettingsS() { delete shared; }

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

                property int NumberDirections
                {
                    int get() { return shared->object->NumberDirections; }
                    void set(int value) { shared->object->NumberDirections = value; }
                }

                property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
                }

                property double VariationCoefficientFailure
                {
                    double get() { return shared->object->VariationCoefficientFailure; }
                    void set(double value) { shared->object->VariationCoefficientFailure = value; }
                }

                int GetRequiredSamples(int nStochasts)
                {
                    return shared->object->getRequiredSamples(nStochasts);
                }

                static int GetRequiredSamples(double probability, double variationCoefficient, int nStochasts)
                {
                    return Uncertainty::DirectionalSamplingSettingsS::getRequiredSamples(probability, variationCoefficient, nStochasts);
                }

                property bool DeriveSamplesFromVariationCoefficient
                {
                    bool get() { return shared->object->DeriveSamplesFromVariationCoefficient; }
                    void set(bool value) { shared->object->DeriveSamplesFromVariationCoefficient = value; }
                }

                System::Collections::Generic::List<Statistics::Wrappers::ProbabilityValue^>^ RequestedQuantiles = gcnew System::Collections::Generic::List<Statistics::Wrappers::ProbabilityValue^>();

                Wrappers::RandomSettings^ RandomSettings = gcnew Wrappers::RandomSettings();

                virtual property Wrappers::RunSettings^ RunSettings
                {
                    Wrappers::RunSettings^ get() { return runSettings; }
                    void set(Wrappers::RunSettings^ value) { runSettings = value; }
                }

                std::shared_ptr<Uncertainty::DirectionalSamplingSettingsS> GetSettings()
                {
                    shared->object->RequestedQuantiles.clear();
                    for (int i = 0; i < RequestedQuantiles->Count; i++)
                    {
                        shared->object->RequestedQuantiles.push_back(RequestedQuantiles[i]->GetValue());
                    }

                    shared->object->randomSettings = RandomSettings->GetSettings(),
                        shared->object->RunSettings = RunSettings->GetSettings();
                    return shared->object;
                }
            };
        }
    }
}


