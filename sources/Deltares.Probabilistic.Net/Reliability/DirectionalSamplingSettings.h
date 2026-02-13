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

#include "../../Deltares.Probabilistic/Reliability/DirectionalSamplingSettings.h"
#include "../Model/RandomSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StochastSettings.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class DirectionalSamplingSettings : Models::Wrappers::IHasRandomSettings, Models::Wrappers::IHasRunSettings, IHasStochastSetting
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::DirectionalSamplingSettings>* shared = new Utils::Wrappers::SharedPointerProvider<Reliability::DirectionalSamplingSettings>(new Reliability::DirectionalSamplingSettings());

                Models::Wrappers::RandomSettings^ randomSettings = gcnew Models::Wrappers::RandomSettings();
                Models::Wrappers::RunSettings^ runSettings = gcnew Models::Wrappers::RunSettings();

            public:
                DirectionalSamplingSettings() {}
                ~DirectionalSamplingSettings() { this->!DirectionalSamplingSettings(); }
                !DirectionalSamplingSettings() { delete shared; }

                property int MinimumSamples
                {
                    int get() { return shared->object->MinimumDirections; }
                    void set(int value) { shared->object->MinimumDirections = value; }
                }

                property int MaximumSamples
                {
                    int get() { return shared->object->MaximumDirections; }
                    void set(int value) { shared->object->MaximumDirections = value; }
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

                std::shared_ptr<Reliability::DirectionalSamplingSettings> GetSettings()
                {
                    shared->object->randomSettings = RandomSettings->GetSettings();
                    shared->object->DirectionSettings = DirectionalSettings->GetSettings();
                    shared->object->runSettings = RunSettings->GetSettings();

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


