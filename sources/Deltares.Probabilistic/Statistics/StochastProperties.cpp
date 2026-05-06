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

#include "StochastProperties.h"
#include "../Utils/probLibException.h"
#include <functional>
#include <algorithm>

namespace Deltares::Statistics
{
    void StochastProperties::applyValue(DistributionPropertyType property, double value)
    {
        switch (property)
        {
            case DistributionPropertyType::Location: this->Location = value; break;
            case DistributionPropertyType::Scale: this->Scale = value; break;
            case DistributionPropertyType::Minimum:    this->Minimum = value; break;
            case DistributionPropertyType::Maximum:    this->Maximum = value; break;
            case DistributionPropertyType::Shift: this->Shift = value; break;
            case DistributionPropertyType::ShiftB: this->ShiftB = value; break;
            case DistributionPropertyType::Shape: this->Shape = value; break;
            case DistributionPropertyType::ShapeB: this->ShapeB = value; break;
            case DistributionPropertyType::Observations: this->Observations = static_cast<int>(value); break;
            default: throw Reliability::probLibException("Property not supported");
        }
    }

    std::shared_ptr<StochastProperties> StochastProperties::clone() const
    {
        std::shared_ptr<StochastProperties> clone = std::make_shared<StochastProperties>();

        clone->Location = this->Location;
        clone->Scale = this->Scale;
        clone->Minimum = this->Minimum;
        clone->Maximum = this->Maximum;
        clone->Shift = this->Shift;
        clone->ShiftB = this->ShiftB;
        clone->Shape = this->Shape;
        clone->ShapeB = this->ShapeB;
        clone->Observations = this->Observations;

        return clone;
    }

    void StochastProperties::copyFrom(const std::shared_ptr<StochastProperties>& source)
    {
        this->Location = source->Location;
        this->Scale = source->Scale;
        this->Minimum = source->Minimum;
        this->Maximum = source->Maximum;
        this->Shift = source->Shift;
        this->ShiftB = source->ShiftB;
        this->Shape = source->Shape;
        this->ShapeB = source->ShapeB;
        this->Observations = source->Observations;

        this->HistogramValues.clear();
        for (const std::shared_ptr<HistogramValue>& histogramValue : source->HistogramValues)
        {
            this->HistogramValues.push_back(histogramValue->clone());
        }

        this->FragilityValues.clear();
        for (const std::shared_ptr<FragilityValue>& fragilityValue : source->FragilityValues)
        {
            this->FragilityValues.push_back(fragilityValue->clone());
        }

        this->DiscreteValues.clear();
        for (const std::shared_ptr<DiscreteValue>& discreteValue : source->DiscreteValues)
        {
            this->DiscreteValues.push_back(discreteValue->clone());
        }

        this->ContributingStochasts.clear();
        for (const std::shared_ptr<ContributingStochast>& contributingStochast : source->ContributingStochasts)
        {
            this->ContributingStochasts.push_back(contributingStochast->clone());
        }
    }


    void StochastProperties::setDirty()
    {
        dirty = true;

        for (const std::shared_ptr<HistogramValue>& histogramValue : this->HistogramValues)
        {
            histogramValue->setDirtyFunction(nullptr);
        }

        for (const std::shared_ptr<DiscreteValue>& discreteValue : this->DiscreteValues)
        {
            discreteValue->setDirtyFunction(nullptr);
        }

        for (const std::shared_ptr<FragilityValue>& fragilityValue : this->FragilityValues)
        {
            fragilityValue->setDirtyFunction(nullptr);
        }
    }

    void StochastProperties::sortFragilityValues()
    {
        std::sort(FragilityValues.begin(), FragilityValues.end(),
            [](const std::shared_ptr<FragilityValue>& p, const std::shared_ptr<FragilityValue>& q)
            {
                return p->X < q->X;
            });
    }

}
