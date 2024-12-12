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

#include <memory>

#include "../../Deltares.Probabilistic/Reliability/ConvergenceReport.h"
#include "../Model/Sample.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ConvergenceReport
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::ConvergenceReport>* shared = nullptr;
            public:
                ConvergenceReport()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider<Reliability::ConvergenceReport>(new Reliability::ConvergenceReport());
                }
                ConvergenceReport(std::shared_ptr<Reliability::ConvergenceReport> report)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider<Reliability::ConvergenceReport>(report);
                }

                property double Convergence
                {
                    double get() { return shared->object->Convergence; }
                    void set(double value) { shared->object->Convergence = value; }
                }

                property int FailedSamples
                {
                    int get() { return shared->object->FailedSamples; }
                    void set(int value) { shared->object->FailedSamples = value; }
                }

                property double FailFraction
                {
                    double get() { return shared->object->FailFraction; }
                    void set(double value) { shared->object->FailFraction = value; }
                }

                property double RelaxationFactor
                {
                    double get() { return shared->object->RelaxationFactor; }
                    void set(double value) { shared->object->RelaxationFactor = value; }
                }

                property double VarianceFactor
                {
                    double get() { return shared->object->VarianceFactor; }
                    void set(double value) { shared->object->VarianceFactor = value; }
                }

                property double MaxWeight
                {
                    double get() { return shared->object->MaxWeight; }
                    void set(double value) { shared->object->MaxWeight = value; }
                }

                property double FailWeight
                {
                    double get() { return shared->object->FailWeight; }
                    void set(double value) { shared->object->FailWeight = value; }
                }

                property double ZMargin
                {
                    double get() { return shared->object->ZMargin; }
                    void set(double value) { shared->object->ZMargin = value; }
                }

                property bool IsConverged
                {
                    bool get() { return shared->object->IsConverged; }
                    void set(bool value) { shared->object->IsConverged = value; }
                }

                Deltares::Models::Wrappers::Sample^ NearestStochastPoint = nullptr;
            };
        }
    }
}


