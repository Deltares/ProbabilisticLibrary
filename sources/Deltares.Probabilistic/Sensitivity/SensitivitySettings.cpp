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
#include "SensitivitySettings.h"

#include <memory>

namespace Deltares::Sensitivity
{
    std::shared_ptr<SensitivityMethod> SensitivitySettings::GetSensitivityMethod()
    {
        switch (this->SensitivityMethod)
        {
        case SensitivityMethodType::SensitivitySobol: return this->GetSobolMethod();
        case SensitivityMethodType::SensitivitySingleVariation: return this->GetSingleVariationMethod();

        default: throw Reliability::probLibException("Reliability method");
        }
    }

    std::shared_ptr<Sobol> SensitivitySettings::GetSobolMethod() const
    {
        std::shared_ptr<Sobol> sobol = std::make_shared<Sobol>();

        sobol->Settings->MaximumSamples = this->MaximumSamples;
        sobol->Settings->RunSettings = this->RunSettings;

        return sobol;
    }

    std::shared_ptr<SingleVariation> SensitivitySettings::GetSingleVariationMethod() const
    {
        std::shared_ptr<SingleVariation> singleVariation = std::make_shared<SingleVariation>();

        singleVariation->Settings->LowValue = this->LowValue;
        singleVariation->Settings->HighValue = this->HighValue;
        singleVariation->Settings->RunSettings = this->RunSettings;

        return singleVariation;
    }

    bool SensitivitySettings::isValid() const
    {
        switch (this->SensitivityMethod)
        {
        case SensitivityMethodType::SensitivitySobol: return GetSobolMethod()->Settings->isValid();
        case SensitivityMethodType::SensitivitySingleVariation: return GetSingleVariationMethod()->Settings->isValid();
        default: throw Reliability::probLibException("Reliability method");
        }
    }

    std::string SensitivitySettings::getSensitivityMethodTypeString(SensitivityMethodType method)
    {
        switch (method)
        {
        case SensitivityMethodType::SensitivitySobol: return "sobol";
        case SensitivityMethodType::SensitivitySingleVariation: return "single_variation";
        default: throw Reliability::probLibException("Reliability method");
        }
    }

    SensitivityMethodType SensitivitySettings::getSensitivityMethodType(std::string method)
    {
        if (method == "sobol") return SensitivityMethodType::SensitivitySobol;
        else if (method == "single_variation") return SensitivityMethodType::SensitivitySingleVariation;
        else throw Reliability::probLibException("Reliability method");
    }
}


