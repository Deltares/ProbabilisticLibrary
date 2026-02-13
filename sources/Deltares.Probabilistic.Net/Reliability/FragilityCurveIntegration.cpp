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

#include "FragilityCurveIntegration.h"

namespace Deltares
{
    namespace Reliability
    {
        namespace Wrappers
        {
            DesignPoint^ FragilityCurveIntegration::GetDesignPoint(Statistics::Wrappers::Stochast^ stochast, Statistics::Wrappers::Stochast^ fragilityCurve, Statistics::Wrappers::Stochast^ normalizingFragilityCurve)
            {
                std::shared_ptr<Statistics::Stochast> nativeStochast = stochast->GetStochast();
                std::shared_ptr<Statistics::Stochast> nativeFragilityCurve = fragilityCurve->GetStochast();
                std::shared_ptr<Statistics::Stochast> nativeNormalizingFragilityCurve = nullptr;
                if (normalizingFragilityCurve != nullptr)
                {
                    nativeNormalizingFragilityCurve = normalizingFragilityCurve->GetStochast();
                }

                shared->object->Settings = this->Settings->GetSettings();
                const std::shared_ptr<Reliability::DesignPoint> nativeDesignPoint = shared->object->getDesignPoint(nativeStochast, nativeFragilityCurve, nativeNormalizingFragilityCurve);

                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ stochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();
                stochasts->Add(stochast);
                stochasts->Add(fragilityCurve);
                stochasts->AddRange(this->GetReferencedStochasts(fragilityCurve));

                if (normalizingFragilityCurve != nullptr)
                {
                    stochasts->Add(normalizingFragilityCurve);
                    stochasts->AddRange(this->GetReferencedStochasts(normalizingFragilityCurve));
                }

                Wrappers::DesignPoint^ designPoint = gcnew Wrappers::DesignPoint(nativeDesignPoint, stochasts);

                return designPoint;
            };

            System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ FragilityCurveIntegration::GetReferencedStochasts(Statistics::Wrappers::Stochast^ stochast)
            {
                System::Collections::Generic::List<Statistics::Wrappers::Stochast^>^ referencedStochasts = gcnew System::Collections::Generic::List<Statistics::Wrappers::Stochast^>();

                if (stochast->DistributionType == Statistics::Wrappers::DistributionType::CDFCurve)
                {
                    for (int i = 0; i < stochast->FragilityValues->Count; i++)
                    {
                        if (stochast->FragilityValues[i]->DesignPoint != nullptr)
                        {
                            Deltares::Models::Wrappers::StochastPoint^ stochastPoint = (Deltares::Models::Wrappers::StochastPoint^)stochast->FragilityValues[i]->DesignPoint;
                            if (stochastPoint != nullptr)
                            {
                                for (int j = 0; j < stochastPoint->Alphas->Count; j++)
                                {
                                    if (stochastPoint->Alphas[j]->Parameter != nullptr && !referencedStochasts->Contains(stochastPoint->Alphas[j]->Parameter))
                                    {
                                        if (!referencedStochasts->Contains(stochastPoint->Alphas[j]->Parameter))
                                        {
                                            referencedStochasts->Add(stochastPoint->Alphas[j]->Parameter);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                return referencedStochasts;
            }
        }
    }
}

