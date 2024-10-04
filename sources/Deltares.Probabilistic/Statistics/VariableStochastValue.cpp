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
#include "VariableStochastValue.h"

#include "DistributionType.h"
#include "Distributions/Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        std::shared_ptr<StochastProperties> VariableStochastValue::getMergedStochast(std::shared_ptr<Distribution> distribution, std::shared_ptr<StochastProperties> defaultStochast)
        {
            std::shared_ptr<StochastProperties> source = this->Stochast->clone();

            // assign default properties if stochast property is not set (has value nan)
            if (defaultStochast != nullptr)
            {
                source->Location = getNonNanValue(source->Location, defaultStochast->Location);
                source->Scale = getNonNanValue(source->Scale, defaultStochast->Scale);
                source->Shift = getNonNanValue(source->Shift, defaultStochast->Shift);
                source->ShiftB = getNonNanValue(source->ShiftB, defaultStochast->ShiftB);
                source->Shape = getNonNanValue(source->Shape, defaultStochast->Shape);
                source->ShapeB = getNonNanValue(source->ShapeB, defaultStochast->ShapeB);
                source->Minimum = getNonNanValue(source->Minimum, defaultStochast->Minimum);
                source->Maximum = getNonNanValue(source->Maximum, defaultStochast->Maximum);
                if (source->Observations == -1)
                {
                    source->Observations = defaultStochast->Observations;
                }
            }

            if (!std::isnan(this->mean) || !std::isnan(this->deviation))
            {
                double requiredMean = std::isnan(this->mean) ? distribution->getMean(source) : this->mean;
                double requiredDeviation = std::isnan(this->deviation) ? distribution->getDeviation(source) : this->deviation;
                distribution->setMeanAndDeviation(source, requiredMean, requiredDeviation);
            }

            return source;
        }

        std::shared_ptr<VariableStochastValue> VariableStochastValue::clone()
        {
            std::shared_ptr<VariableStochastValue> value = std::make_shared<VariableStochastValue>();

            value->X = this->X;
            value->mean = this->mean;
            value->deviation = this->deviation;

            value->Stochast = this->Stochast->clone();

            return value;
        }

        double VariableStochastValue::getNonNanValue(double value1, double value2)
        {
            if (std::isnan(value1))
            {
                return value2;
            }
            else
            {
                return value1;
            }
        }

    }
}
