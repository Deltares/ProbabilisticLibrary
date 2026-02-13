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
#include "DesignPointCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            DesignPoint^ DesignPointCombiner::Combine(CombinationType combination, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints)
            {
                return Combine(combination, designPoints, nullptr, nullptr, nullptr);
            }

            DesignPoint^ DesignPointCombiner::Combine(CombinationType combination,
                System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints,
                Deltares::Statistics::Wrappers::SelfCorrelationMatrix^ selfCorrelationMatrix,
                Deltares::Statistics::Wrappers::CorrelationMatrix^ correlationMatrix,
                Models::Wrappers::ProgressIndicator^ progressIndicator)
            {
                std::vector<std::shared_ptr<Reliability::DesignPoint>> nativeDesignPoints;

                for (int i = 0; i < designPoints->Count; i++)
                {
                    nativeDesignPoints.push_back(designPoints[i]->getDesignPoint());
                }

                const std::shared_ptr<Statistics::SelfCorrelationMatrix> nativeSelfCorrelationMatrix = selfCorrelationMatrix != nullptr ? selfCorrelationMatrix->GetNativeSelfCorrelationMatrix() : nullptr;
                const std::shared_ptr<Statistics::CorrelationMatrix> nativeCorrelationMatrix = correlationMatrix != nullptr ? correlationMatrix->GetCorrelationMatrix() : nullptr;
                const std::shared_ptr<Models::ProgressIndicator> nativeProgressIndicator = progressIndicator != nullptr ? progressIndicator->GetProgressIndicator() : nullptr;
                const combineAndOr combineAndOr = GetCombinationType(combination);

                std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->combineDesignPoints(combineAndOr, nativeDesignPoints, nativeSelfCorrelationMatrix, nativeCorrelationMatrix, nativeProgressIndicator);

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, designPoints);

                return designPoint;
            }

            DesignPoint^ DesignPointCombiner::CombineExcluding(
                System::Collections::Generic::IList<Statistics::Wrappers::Scenario^>^ scenarios,
                System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints,
                Deltares::Statistics::Wrappers::CorrelationMatrix^ correlationMatrix)
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

                const std::shared_ptr<Statistics::CorrelationMatrix> nativeCorrelationMatrix = correlationMatrix != nullptr ? correlationMatrix->GetCorrelationMatrix() : nullptr;

                std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->combineDesignPointsExcluding(nativeScenarios, nativeDesignPoints, nativeCorrelationMatrix);

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, designPoints);

                return designPoint;
            }
        }
    }
}

