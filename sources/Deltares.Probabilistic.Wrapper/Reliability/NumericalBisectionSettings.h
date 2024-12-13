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

#include "../../Deltares.Probabilistic/Reliability/NumericalBisectionSettings.h"
#include "../Model/RunSettings.h"
#include "../Reliability/StartPointCalculatorSettings.h"
#include "../Reliability/StochastSettings.h"
#include "../Utils/SharedPointerProvider.h"
#include "DesignPointMethodSettings.h"


namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class NumericalBisectionSettings : IHasRunSettings, IHasStochastSetting
            {
            private:
                SharedPointerProvider<Reliability::NumericalBisectionSettings>* shared = new SharedPointerProvider(new Reliability::NumericalBisectionSettings());
                Wrappers::RunSettings^ runSettings = gcnew Wrappers::RunSettings();
            public:
                NumericalBisectionSettings()
                {
                    shared = new SharedPointerProvider(new Reliability::NumericalBisectionSettings());
                }

                ~NumericalBisectionSettings() { this->!NumericalBisectionSettings(); }
                !NumericalBisectionSettings() { delete shared; }

                property double EpsilonBeta
                {
                    double get() { return shared->object->EpsilonBeta; }
                    void set(double value) { shared->object->EpsilonBeta = value; }
                }

                property int MinimumIterations
                {
                    int get() { return shared->object->MinimumIterations; }
                    void set(int value) { shared->object->MinimumIterations = value; }
                }

                property int MaximumIterations
                {
                    int get() { return shared->object->MaximumIterations; }
                    void set(int value) { shared->object->MaximumIterations = value; }
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

                std::shared_ptr<Reliability::NumericalBisectionSettings> GetSettings()
                {
                    shared->object->StochastSet->stochastSettings.clear();
                    for (int i = 0; i < StochastSettings->Count; i++)
                    {
                        shared->object->StochastSet->stochastSettings.push_back(StochastSettings[i]->GetSettings());
                    }

                    shared->object->runSettings = RunSettings->GetSettings();

                    return shared->object;
                }

            };
        }
    }
}

