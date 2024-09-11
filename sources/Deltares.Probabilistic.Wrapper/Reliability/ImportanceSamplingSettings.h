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

#include "../../Deltares.Probabilistic/Reliability/ImportanceSamplingSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "DesignPointMethodSettings.h"
#include "StartPointCalculatorSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class ImportanceSamplingSettings : IHasRunSettings, IHasRandomSettings, IHasStochastSetting
            {
            private:
                SharedPointerProvider<Reliability::ImportanceSamplingSettings>* shared = nullptr;
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
                Wrappers::RandomSettings^ randomSettings = gcnew Wrappers::RandomSettings();
                Wrappers::StartPointCalculatorSettings^ startPointSettings = gcnew Wrappers::StartPointCalculatorSettings();
            public:
                ImportanceSamplingSettings()
                {
                    shared = new SharedPointerProvider(new Reliability::ImportanceSamplingSettings());
                    shared->object->randomSettings = RandomSettings->GetSettings();
                }
                ImportanceSamplingSettings(std::shared_ptr<Reliability::ImportanceSamplingSettings> settings)
                {
                    shared = new SharedPointerProvider(settings);
                }
                ~ImportanceSamplingSettings() { this->!ImportanceSamplingSettings(); }
                !ImportanceSamplingSettings() { delete shared; }

                ImportanceSamplingSettings^ Clone()
                {
                    return gcnew ImportanceSamplingSettings(shared->object->clone());
                }

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

                property int MaximumSamplesNoResult
                {
                    int get() { return shared->object->MaximumSamplesNoResult; }
                    void set(int value) { shared->object->MaximumSamplesNoResult = value; }
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

                virtual property Wrappers::StartPointCalculatorSettings^ StartPointSettings
                {
                    Wrappers::StartPointCalculatorSettings^ get() { return startPointSettings; }
                    void set(Wrappers::StartPointCalculatorSettings^ value) { startPointSettings = value; }
                }

                property double VarianceFactor
                {
                    double get() { return shared->object->VarianceFactor; }
                    void set(double value) { shared->object->VarianceFactor = value; }
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

                std::shared_ptr<Reliability::ImportanceSamplingSettings> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    shared->object->startPointSettings = StartPointSettings->GetSettings();
                    shared->object->randomSettings = RandomSettings->GetSettings();
                    shared->object->runSettings = RunSettings->GetSettings();

                    return shared->object;
                }
            };
        }
    }
}


