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

#include "../../Deltares.Probabilistic/Reliability/ReliabilityResult.h"
#include "../Utils/SharedPointerProvider.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            public ref class ReliabilityResult
            {
            private:
                Utils::Wrappers::SharedPointerProvider<Reliability::ReliabilityResult>* shared = nullptr ;

            public:
                ReliabilityResult()
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(new Deltares::Reliability::ReliabilityResult());
                }

                ReliabilityResult(std::shared_ptr<Reliability::ReliabilityResult> result)
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(result);

                    this->ProxyIteration = result->ProxyIteration;
                    this->Run = result->Run;
                    this->Index = result->Index;
                    this->Reliability = result->Reliability;
                    this->LowReliability = result->LowReliability;
                    this->HighReliability = result->HighReliability;
                    this->Samples = result->Samples;
                    this->ConvBeta = result->ConvBeta;
                    this->ProxyConvergence = result->ProxyConvergence;
                    this->Variation = result->Variation;
                    this->Difference = result->Difference;
                    this->Contribution = result->Contribution;
                }

                property int ProxyIteration
                {
                    int get() { return shared->object->ProxyIteration; }
                    void set(int value) { shared->object->ProxyIteration = value; }
                }

                property int Run
                {
                    int get() { return shared->object->Run; }
                    void set(int value) { shared->object->Run = value; }
                }

                property int Index
                {
                    int get() { return shared->object->Index; }
                    void set(int value) { shared->object->Index = value; }
                }

                property double Reliability
                {
                    double get() { return shared->object->Reliability; }
                    void set(double value) { shared->object->Reliability = value; }
                }

                property double LowReliability
                {
                    double get() { return shared->object->LowReliability; }
                    void set(double value) { shared->object->LowReliability = value; }
                }

                property double HighReliability
                {
                    double get() { return shared->object->HighReliability; }
                    void set(double value) { shared->object->HighReliability = value; }
                }

                property double Samples
                {
                    double get() { return shared->object->Samples; }
                    void set(double value) { shared->object->Samples = value; }
                }

                property double ConvBeta
                {
                    double get() { return shared->object->ConvBeta; }
                    void set(double value) { shared->object->ConvBeta = value; }
                }

                property double ProxyConvergence
                {
                    double get() { return shared->object->ProxyConvergence; }
                    void set(double value) { shared->object->ProxyConvergence = value; }
                }

                property double Variation
                {
                    double get() { return shared->object->Variation; }
                    void set(double value) { shared->object->Variation = value; }
                }

                property double Difference
                {
                    double get() { return shared->object->Difference; }
                    void set(double value) { shared->object->Difference = value; }
                }

                property double Contribution
                {
                    double get() { return shared->object->Contribution; }
                    void set(double value) { shared->object->Contribution = value; }
                }

                std::shared_ptr<Deltares::Reliability::ReliabilityResult> GetNativeReliabilityResult()
                {
                    return shared->object;
                }

                bool IsMeaningful(ReliabilityResult^ before, ReliabilityResult^ after)
                {
                    std::shared_ptr<Deltares::Reliability::ReliabilityResult> nativeBefore = before != nullptr ? before->GetNativeReliabilityResult() : nullptr;
                    std::shared_ptr<Deltares::Reliability::ReliabilityResult> nativeAfter = after != nullptr ? after->GetNativeReliabilityResult() : nullptr;

                    return shared->object->IsMeaningful(nativeBefore, nativeAfter);
                }

                System::String^ ToString() override
                {
                    return Reliability.ToString("g3");
                }
            };
        }
    }
}


