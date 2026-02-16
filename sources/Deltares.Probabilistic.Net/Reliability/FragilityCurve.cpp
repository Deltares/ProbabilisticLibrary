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
#include "FragilityCurve.h"
#include "../Statistics/FragilityValue.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            void FragilityCurve::updateNativeObject()
            {
                if (!HasMatchingFragilityValues())
                {
                    shared->object->getProperties()->FragilityValues.clear();

                    for (int i = 0; i < this->FragilityValues->Count; i++)
                    {
                        shared->object->getProperties()->FragilityValues.push_back(this->FragilityValues[i]->GetValue());
                    }
                }
            }

            Deltares::Models::Wrappers::StochastPoint^ FragilityCurve::GetRealization(double x)
            {
                this->updateNativeObject();

                std::shared_ptr<Models::StochastPoint> stochastPoint = shared->object->getDesignPoint(x);

                if (stochastPoint != nullptr)
                {
                    System::Collections::Generic::List<Stochast^>^ stochasts = gcnew System::Collections::Generic::List<Stochast^>();
                    for (size_t i = 0; i < this->FragilityValues->Count; i++)
                    {
                        Statistics::Wrappers::FragilityValue^ fragilityValue = this->FragilityValues[i];
                        if (fragilityValue->DesignPoint != nullptr)
                        {
                            Models::Wrappers::StochastPoint^ designPoint = static_cast<Models::Wrappers::StochastPoint^>(fragilityValue->DesignPoint);
                            for (size_t j = 0; j < designPoint->Alphas->Count; j++)
                            {
                                if (!stochasts->Contains(designPoint->Alphas[j]->Parameter))
                                {
                                    stochasts->Add(designPoint->Alphas[j]->Parameter);
                                }
                            }
                        }
                    }

                    stochastPoint->updateInfluenceFactors();

                    return gcnew Models::Wrappers::StochastPoint(stochastPoint, stochasts);
                }
                else
                {
                    return nullptr;
                }
            };

            bool FragilityCurve::HasMatchingFragilityValues()
            {
                if (this->FragilityValues->Count != shared->object->getProperties()->FragilityValues.size())
                {
                    return false;
                }

                for (int i = 0; i < this->FragilityValues->Count; i++)
                {
                    if (this->FragilityValues[i]->GetValue() != shared->object->getProperties()->FragilityValues[i])
                    {
                        return false;
                    }

                    if (this->FragilityValues[i]->DesignPoint != nullptr && shared->object->getProperties()->FragilityValues[i]->designPoint == nullptr)
                    {
                        return false;
                    }
                }

                return true;
            }
        }
    }
}


