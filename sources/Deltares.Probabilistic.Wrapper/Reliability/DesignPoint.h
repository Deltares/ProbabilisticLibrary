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

#include "../../Deltares.Probabilistic/Reliability/DesignPoint.h"
#include "../Model/Evaluation.h"
#include "../Model/StochastPointAlpha.h"
#include "../Model/StochastPoint.h"
#include "../Statistics/Stochast.h"
#include "../Model/Message.h"
#include "ConvergenceReport.h"
#include "ReliabilityResult.h"
#include "../Utils/TagRepository.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;
            using namespace Deltares::Statistics::Wrappers;

            public ref class DesignPoint : public Wrappers::StochastPoint
            {
            private:
                SharedPointerProvider<Reliability::DesignPoint>* shared = nullptr;
                ConvergenceReport^ convergenceReport = gcnew Wrappers::ConvergenceReport();
                System::Collections::Generic::List<Wrappers::ReliabilityResult^>^ reliabilityResults = gcnew System::Collections::Generic::List<ReliabilityResult^>();
                System::Collections::Generic::List<Wrappers::Evaluation^>^ evaluations = gcnew System::Collections::Generic::List<Wrappers::Evaluation^>();
                System::Collections::Generic::List<Wrappers::Message^>^ messages = gcnew System::Collections::Generic::List<Wrappers::Message^>();
                System::Collections::Generic::List<Wrappers::DesignPoint^>^ contributingDesignPoints = gcnew System::Collections::Generic::List<Wrappers::DesignPoint^>();

                void setDesignPointInfo();
                void setDesignPoint(System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts);
                void setDesignPoints(System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ contributingDesignPoints);

                bool HasMatchingAlphaValues();
            protected:
                void setNativeObject(std::shared_ptr<Models::StochastPoint> nativeStochastPoint) override
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(static_pointer_cast<Reliability::DesignPoint>(nativeStochastPoint));
                    StochastPoint::setNativeObject(nativeStochastPoint);
                }
            public:
                DesignPoint() : StochastPoint(std::make_shared<Reliability::DesignPoint>())
                {
                    setDesignPoint(gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>());
                }

                DesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint, System::Collections::Generic::IList<Statistics::Wrappers::Stochast^>^ stochasts) : StochastPoint(designPoint)
                {
                    setDesignPoint(stochasts);
                }

                DesignPoint(std::shared_ptr<Reliability::DesignPoint> designPoint, System::Collections::Generic::IList<Reliability::Wrappers::DesignPoint^>^ designPoints) : StochastPoint(designPoint)
                {
                    setDesignPoints(designPoints);
                }
                ~DesignPoint() { this->!DesignPoint(); }
                !DesignPoint() { delete shared; }

                property System::String^ Identifier
                {
                    System::String^ get() { return NativeSupport::toManaged(shared->object->Identifier); }
                    void set (System::String^ value) { shared->object->Identifier = NativeSupport::toNative(value); }
                }

                property double ProbabilityFailure
                {
                    double get() { return Statistics::StandardNormal::getQFromU(this->Beta); }
                }

                property double ProbabilityNonFailure
                {
                    double get() { return Statistics::StandardNormal::getPFromU(this->Beta); }
                }

                property double ReturnPeriod
                {
                    double get() { return Statistics::StandardNormal::getTFromP(this->ProbabilityFailure); }
                }

                property Wrappers::ConvergenceReport^ ConvergenceReport
                {
                    Wrappers::ConvergenceReport^ get() { return convergenceReport; }
                    void set(Wrappers::ConvergenceReport^ value) { convergenceReport = value; }
                }

                property System::Collections::Generic::List<ReliabilityResult^>^ ReliabilityResults
                {
                    System::Collections::Generic::List<ReliabilityResult^>^ get() { return reliabilityResults; }
                }

                property System::Collections::Generic::List<Wrappers::Evaluation^>^ Evaluations
                {
                    System::Collections::Generic::List<Wrappers::Evaluation^>^ get() { return evaluations; }
                }

                property System::Collections::Generic::List<Wrappers::Message^>^ Messages
                {
                    System::Collections::Generic::List<Wrappers::Message^>^ get() { return messages; }
                }

                property System::Collections::Generic::List<DesignPoint^>^ ContributingDesignPoints
                {
                    System::Collections::Generic::List<DesignPoint^>^ get() { return contributingDesignPoints; }
                }

                void AssignTags(TagRepository^ tagRepository);

                std::shared_ptr<Reliability::DesignPoint> getDesignPoint() override
                {
                    if (!HasMatchingAlphaValues())
                    {
                        shared->object->Alphas.clear();

                        for (int i = 0; i < this->Alphas->Count; i++)
                        {
                            shared->object->Alphas.push_back(this->Alphas[i]->GetNativeAlpha());
                        }
                    }

                    return shared->object;
                }
            };
        }
    }
}


