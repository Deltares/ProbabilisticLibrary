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
#include "DesignPoint.h"
#include "../Model/Message.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace System::Collections::Generic;

            void DesignPoint::setDesignPointInfo()
            {
                if (shared->object->convergenceReport != nullptr)
                {
                    this->ConvergenceReport = gcnew Wrappers::ConvergenceReport(shared->object->convergenceReport);
                }

                for (int i = 0; i < shared->object->ReliabililityResults.size(); i++)
                {
                    Wrappers::ReliabilityResult^ resultWrapper = gcnew Wrappers::ReliabilityResult(shared->object->ReliabililityResults[i]);
                    this->ReliabilityResults->Add(resultWrapper);
                }

                for (int i = 0; i < shared->object->Evaluations.size(); i++)
                {
                    Models::Wrappers::Evaluation^ evaluationWrapper = gcnew Models::Wrappers::Evaluation(shared->object->Evaluations[i]);
                    this->Evaluations->Add(evaluationWrapper);
                }

                for (int i = 0; i < shared->object->Messages.size(); i++)
                {
                    Models::Wrappers::Message^ messageWrapper = gcnew Models::Wrappers::Message(shared->object->Messages[i]);
                    this->Messages->Add(messageWrapper);
                }
            }

            void DesignPoint::setDesignPoint(IList<Statistics::Wrappers::Stochast^>^ stochasts)
            {
                setStochastPoint(shared->object, stochasts);

                setDesignPointInfo();

                for (int i = 0; i < shared->object->ContributingDesignPoints.size(); i++)
                {
                    DesignPoint^ designPointWrapper = gcnew DesignPoint(shared->object->ContributingDesignPoints[i], stochasts);
                    this->ContributingDesignPoints->Add(designPointWrapper);
                }
            }

            void DesignPoint::setDesignPoints(IList<DesignPoint^>^ contributingDesignPoints)
            {
                List<Statistics::Wrappers::Stochast^>^ stochasts = gcnew List<Statistics::Wrappers::Stochast^>();

                for (int i = 0; i < contributingDesignPoints->Count; i++)
                {
                    for (int j = 0; j < contributingDesignPoints[i]->Alphas->Count; j++)
                    {
                        stochasts->Add(contributingDesignPoints[i]->Alphas[j]->Parameter);
                    }
                }

                setStochastPoint (shared->object, stochasts);

                setDesignPointInfo();

                for (int i = 0; i < shared->object->ContributingDesignPoints.size(); i++)
                {
                    for (int j = 0; j < contributingDesignPoints->Count; j++)
                    {
                        if (contributingDesignPoints[j]->getDesignPoint() == shared->object->ContributingDesignPoints[i])
                        {
                            this->ContributingDesignPoints->Add(contributingDesignPoints[j]);
                        }
                    }
                }
            }

            void DesignPoint::AssignTags(Utils::Wrappers::TagRepository^ tagRepository)
            {
                for (int i = 0; i < this->Evaluations->Count; i++)
                {
                    this->Evaluations[i]->AssignTag(tagRepository);
                }

                for (int i = 0; i < this->ContributingDesignPoints->Count; i++)
                {
                    this->ContributingDesignPoints[i]->AssignTags(tagRepository);
                }
            }

            bool DesignPoint::HasMatchingAlphaValues()
            {
                if (this->Alphas->Count != shared->object->Alphas.size())
                {
                    return false;
                }

                for (int i = 0; i < this->Alphas->Count; i++)
                {
                    if (this->Alphas[i]->GetNativeAlpha() != shared->object->Alphas[i])
                    {
                        return false;
                    }
                }

                return true;
            }

        }
    }
}

