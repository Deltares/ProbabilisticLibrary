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

#include "ClusterSettings.h"
#include "ImportanceSamplingSettings.h"
#include "../../Deltares.Probabilistic/Reliability/AdaptiveImportanceSamplingSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "StartPointCalculatorSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class AdaptiveImportanceSamplingSettings : Models::Wrappers::IHasRunSettings, IHasStochastSetting
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::AdaptiveImportanceSamplingSettings>* shared = nullptr;
            public:
                AdaptiveImportanceSamplingSettings()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Reliability::AdaptiveImportanceSamplingSettings());
                    shared->object->startPointSettings = StartPointSettings->GetSettings();
                    shared->object->importanceSamplingSettings = ImportanceSamplingSettings->GetSettings();
                }
                AdaptiveImportanceSamplingSettings(std::shared_ptr<Reliability::AdaptiveImportanceSamplingSettings> settings)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(settings);
                }
                ~AdaptiveImportanceSamplingSettings() { this->!AdaptiveImportanceSamplingSettings(); }
                !AdaptiveImportanceSamplingSettings() { delete shared; }

                property int MinVarianceLoops
                {
                    int get() { return shared->object->MinVarianceLoops; }
                    void set(int value) { shared->object->MinVarianceLoops = value; }
                }

                property int MaxVarianceLoops
                {
                    int get() { return shared->object->MaxVarianceLoops; }
                    void set(int value) { shared->object->MaxVarianceLoops = value; }
                }

                property int MinimumFailedSamples
                {
                    int get() { return shared->object->MinimumFailedSamples; }
                    void set(int value) { shared->object->MinimumFailedSamples = value; }
                }

                property bool AutoMaximumSamples
                {
                    bool get() { return shared->object->AutoMaximumSamples; }
                    void set(bool value) { shared->object->AutoMaximumSamples = value; }
                }

                property double VarianceFactor
                {
                    double get() { return shared->object->VarianceFactor; }
                    void set(double value) { shared->object->VarianceFactor = value; }
                }

                property double LoopVarianceIncrement
                {
                    double get() { return shared->object->LoopVarianceIncrement; }
                    void set(double value) { shared->object->LoopVarianceIncrement = value; }
                }

                property double FractionFailed
                {
                    double get() { return shared->object->FractionFailed; }
                    void set(double value) { shared->object->FractionFailed = value; }
                }

                property bool Clustering
                {
                    bool get() { return shared->object->Clustering; }
                    void set(bool value) { shared->object->Clustering = value; }
                }

                property double EpsWeightSample
                {
                    double get() { return shared->object->EpsWeightSample; }
                    void set(double value) { shared->object->EpsWeightSample = value; }
                }

                property double MaxBeta
                {
                    double get() { return shared->object->MaxBeta; }
                    void set(double value) { shared->object->MaxBeta = value; }
                }

                property double StartValueStepSize
                {
                    double get() { return shared->object->StartValueStepSize; }
                    void set(double value) { shared->object->StartValueStepSize = value; }
                }

                property bool StartPointOnLimitState
                {
                    bool get() { return shared->object->StartPointOnLimitState; }
                    void set(bool value) { shared->object->StartPointOnLimitState = value; }
                }

                virtual property Models::Wrappers::RunSettings^ RunSettings
                {
                    Models::Wrappers::RunSettings^ get() { return ImportanceSamplingSettings->RunSettings; }
                    void set(Models::Wrappers::RunSettings^ value) { ImportanceSamplingSettings->RunSettings = value; }
                }

                StartPointCalculatorSettings^ StartPointSettings = gcnew Wrappers::StartPointCalculatorSettings();

                ClusterSettings^ ClusterSettings = gcnew Wrappers::ClusterSettings();

                ImportanceSamplingSettings^ ImportanceSamplingSettings = gcnew Wrappers::ImportanceSamplingSettings();

                virtual Deltares::Reliability::Wrappers::StochastSettings^ GetStochastSetting(Statistics::Wrappers::Stochast^ stochast)
                {
                    for (int i = 0; i < ImportanceSamplingSettings->StochastSettings->Count; i++)
                    {
                        if (ImportanceSamplingSettings->StochastSettings[i]->Stochast == stochast)
                        {
                            return ImportanceSamplingSettings->StochastSettings[i];
                        }
                    }

                    return nullptr;
                }

                std::shared_ptr<Reliability::AdaptiveImportanceSamplingSettings> GetSettings()
                {
                    shared->object->startPointSettings = StartPointSettings->GetSettings();
                    shared->object->clusterSettings = ClusterSettings->GetSettings();
                    shared->object->importanceSamplingSettings = ImportanceSamplingSettings->GetSettings();

                    return shared->object;
                }
            };
        }
    }
}



