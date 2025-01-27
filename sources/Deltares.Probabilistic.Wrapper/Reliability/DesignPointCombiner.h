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

#include "../../Deltares.Probabilistic/Combine/DesignPointCombiner.h"
#include "../Model/Evaluation.h"
#include "../Model/StochastPointAlpha.h"
#include "../Model/Message.h"
#include "../Model/ProgressIndicator.h"
#include "../Statistics/Stochast.h"
#include "../Statistics/Scenario.h"
#include "../Statistics/CorrelationMatrix.h"
#include "../Statistics/SelfCorrelationMatrix.h"
#include "DesignPoint.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;

            public enum class CombinerType
            {
                Hohenbichler,
                ImportanceSampling,
                DirectionalSampling
            };

            public enum class CombinationType
            {
                OneFails,
                AllFail
            };

            public ref class DesignPointCombiner
            {
            private:
                SharedPointerProvider<Reliability::DesignPointCombiner>* shared = new SharedPointerProvider(new Reliability::DesignPointCombiner());

                combineAndOr GetCombinationType(CombinationType combination)
                {
                    switch (combination)
                    {
                    case CombinationType::OneFails: return combineAndOr::combOr;
                    case CombinationType::AllFail: return combineAndOr::combAnd;
                    default: throw gcnew System::NotSupportedException("combination type");
                    }
                }

            public:
                DesignPointCombiner() {}
                ~DesignPointCombiner() { this->!DesignPointCombiner(); }
                !DesignPointCombiner() { delete shared; }

                property Wrappers::CombinerType CombinerType
                {
                    Wrappers::CombinerType get()
                    {
                        switch (shared->object->combinerType)
                        {
                        case Reliability::CombinerType::Hohenbichler: return Wrappers::CombinerType::Hohenbichler;
                        case Reliability::CombinerType::ImportanceSampling: return Wrappers::CombinerType::ImportanceSampling;
                        case Reliability::CombinerType::DirectionalSampling: return Wrappers::CombinerType::DirectionalSampling;
                        default: throw gcnew System::NotSupportedException("Combiner type");
                        }
                    }
                    void set(Wrappers::CombinerType value)
                    {
                        switch (value)
                        {
                        case Wrappers::CombinerType::Hohenbichler: shared->object->combinerType = Reliability::CombinerType::Hohenbichler; break;
                        case Wrappers::CombinerType::ImportanceSampling:shared->object->combinerType = Reliability::CombinerType::ImportanceSampling; break;
                        case Wrappers::CombinerType::DirectionalSampling: shared->object->combinerType = Reliability::CombinerType::DirectionalSampling; break;
                        default: throw gcnew System::NotSupportedException("Combiner type");
                        }
                    }
                }

                    DesignPoint^ Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints, Deltares::Statistics::Wrappers::SelfCorrelationMatrix^ selfCorrelationMatrix, Wrappers::ProgressIndicator^ progressIndicator)
                {
                    std::vector<std::shared_ptr<Reliability::DesignPoint>> nativeDesignPoints;

                    for (int i = 0; i < designPoints->Count; i++)
                    {
                        nativeDesignPoints.push_back(designPoints[i]->getDesignPoint());
                    }

                    const std::shared_ptr<Statistics::SelfCorrelationMatrix> nativeSelfCorrelationMatrix = selfCorrelationMatrix != nullptr ? selfCorrelationMatrix->GetNativeSelfCorrelationMatrix() : nullptr;
                    const std::shared_ptr<Models::ProgressIndicator> nativeProgressIndicator = progressIndicator != nullptr ? progressIndicator->GetProgressIndicator() : nullptr;
                    const combineAndOr combineAndOr = GetCombinationType(combination);

                    std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->combineDesignPoints(combineAndOr, nativeDesignPoints, nativeSelfCorrelationMatrix, nativeProgressIndicator);

                    Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, designPoints);

                    return designPoint;
                }

                DesignPoint^ CombineExcluding(System::Collections::Generic::IList<Statistics::Wrappers::Scenario^>^ scenarios, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints, Deltares::Statistics::Wrappers::SelfCorrelationMatrix^ selfCorrelationMatrix)
                {
                    std::vector<std::shared_ptr<Statistics::Scenario>> nativeScenarios;
                    for (int i = 0; i < scenarios->Count; i++)
                    {
                        nativeScenarios.push_back(scenarios[i]->GetNativeScenario());
                    }

                    std::vector<std::shared_ptr<Reliability::DesignPoint>> nativeDesignPoints;
                    for (int i = 0; i < designPoints->Count; i++)
                    {
                        nativeDesignPoints.push_back(designPoints[i]->getDesignPoint());
                    }

                    const std::shared_ptr<Statistics::SelfCorrelationMatrix> nativeSelfCorrelationMatrix = selfCorrelationMatrix != nullptr ? selfCorrelationMatrix->GetNativeSelfCorrelationMatrix() : nullptr;

                    std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->combineDesignPointsExcluding(nativeScenarios, nativeDesignPoints, nativeSelfCorrelationMatrix);

                    Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, designPoints);

                    return designPoint;
                }

                DesignPoint^ Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints)
                {
                    return Combine(combination, designPoints, nullptr, nullptr);
                }
            };
        }
    }
}
            

