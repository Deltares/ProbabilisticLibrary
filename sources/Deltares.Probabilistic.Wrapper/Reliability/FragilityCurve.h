// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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

#include "DesignPoint.h"
#include "../Statistics/Stochast.h"
#include "../../Deltares.Probabilistic/Reliability/FragilityCurve.h"
#include "../../Deltares.Probabilistic/Statistics/Stochast.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            using namespace Deltares::Utils::Wrappers;
            using namespace Deltares::Models::Wrappers;

            public ref class FragilityCurve : public Statistics::Wrappers::Stochast
            {
            private:
                SharedPointerProvider<Reliability::FragilityCurve>* shared = nullptr;

                CallBackList<FragilityCurve^>^ contributingFragilityCurves = gcnew CallBackList<FragilityCurve^>();
                void SynchronizeContributingFragilityCurves(ListOperationType listOperationType, FragilityCurve^ fragilityCurve);
                bool HasMatchingFragilityValues();

                bool synchronizing = false;
            protected:
                void setNativeObject(std::shared_ptr<Statistics::Stochast> nativeStochast) override
                {
                    shared = new Utils::Wrappers::SharedPointerProvider(static_pointer_cast<Reliability::FragilityCurve>(nativeStochast));
                    Stochast::setNativeObject(nativeStochast);
                }
            public:
                FragilityCurve() : Stochast(std::make_shared<Reliability::FragilityCurve>())
                {
                    contributingFragilityCurves->SetCallBack(gcnew ListCallBack<FragilityCurve^>(this, &FragilityCurve::SynchronizeContributingFragilityCurves));
                }
                ~FragilityCurve() { this->!FragilityCurve(); }
                !FragilityCurve() { delete shared; }

                virtual property bool Inverted 
                {
                    bool get() override { return shared->object->inverted; }
                    void set(bool value) override { shared->object->inverted = value; }
                }

                property System::Collections::Generic::IList<FragilityCurve^>^ ContributingFragilityCurves
                {
                    System::Collections::Generic::IList<FragilityCurve^>^ get() { return contributingFragilityCurves; }
                }

                virtual Deltares::Models::Wrappers::StochastPoint^ GetRealization(double x);

                // TODO: PROBL-42 remove when fragility curve integration to c++ is complete
                bool IsGloballyDescending()
                {
                    return shared->object->isGloballyDescending();
                }

                std::shared_ptr<Reliability::FragilityCurve> GetNativeValue()
                {
                    if (!HasMatchingFragilityValues())
                    {
                        shared->object->getProperties()->FragilityValues.clear();

                        for (int i = 0; i < this->FragilityValues->Count; i++)
                        {
                            shared->object->getProperties()->FragilityValues.push_back(this->FragilityValues[i]->GetValue());
                        }
                    }

                    return shared->object;
                }

            };
        }
    }
}
