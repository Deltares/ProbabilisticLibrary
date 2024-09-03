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
#include "VariableStochastValueSet.h"

#include <algorithm>
#include <cmath>
#include "StochastProperties.h"
#include "DistributionType.h"
#include "VariableStochastValue.h"
#include "../Math/NumericSupport.h"
#include "Distributions/DistributionLibrary.h"

namespace Deltares
{
    namespace Statistics
    {
        using namespace Deltares::Numeric;

        void VariableStochastValuesSet::initializeForRun()
        {
            xValues.clear();
            locations.clear();
            scales.clear();
            minimums.clear();
            maximums.clear();
            shapes.clear();
            shapesB.clear();
            shifts.clear();
            shiftsB.clear();
            observations.clear();

            std::sort(this->StochastValues.begin(), this->StochastValues.end(), [](std::shared_ptr<VariableStochastValue> val1, std::shared_ptr<VariableStochastValue> val2) {return val1->X < val2->X; });

            for (size_t i = 0; i < this->StochastValues.size(); i++)
            {
                std::shared_ptr<VariableStochastValue> value = this->StochastValues[i];

                xValues.push_back(value->X);
                locations.push_back(value->Stochast->Location);
                scales.push_back(value->Stochast->Scale);
                minimums.push_back(value->Stochast->Minimum);
                maximums.push_back(value->Stochast->Maximum);
                shapes.push_back(value->Stochast->Shape);
                shapesB.push_back(value->Stochast->ShapeB);
                shifts.push_back(value->Stochast->Shift);
                shiftsB.push_back(value->Stochast->ShiftB);
                observations.push_back(value->Stochast->Observations);
            }
        }

        std::shared_ptr<StochastProperties> VariableStochastValuesSet::getInterpolatedStochast(double x)
        {
            std::shared_ptr<StochastProperties> properties = std::make_shared<StochastProperties>();

            properties->Location = NumericSupport::interpolate(x, this->xValues, this->locations);
            properties->Scale = NumericSupport::interpolate(x, this->xValues, this->scales);
            properties->Minimum = NumericSupport::interpolate(x, this->xValues, this->minimums);
            properties->Maximum = NumericSupport::interpolate(x, this->xValues, this->maximums);
            properties->Shape = NumericSupport::interpolate(x, this->xValues, this->shapes);
            properties->ShapeB = NumericSupport::interpolate(x, this->xValues, this->shapesB);
            properties->Shift = NumericSupport::interpolate(x, this->xValues, this->shifts);
            properties->ShiftB = NumericSupport::interpolate(x, this->xValues, this->shiftsB);
            properties->Observations = round(NumericSupport::interpolate(x, this->xValues, this->observations));

            return properties;
        }
        
        bool VariableStochastValuesSet::isVarying(DistributionType distributionType)
        {
            std::shared_ptr<Distribution> distribution = DistributionLibrary::getDistribution(distributionType, false, false);

            for (size_t i = 0; i < StochastValues.size(); i++)
            {
                if (distribution->isVarying(StochastValues[i]->Stochast))
                {
                    return true;
                }
            }

            return false;
        }

        void VariableStochastValuesSet::copyFrom (std::shared_ptr<VariableStochastValuesSet> source)
        {
            this->StochastValues.clear();

            for (std::shared_ptr<VariableStochastValue> value : source->StochastValues)
            {
                this->StochastValues.push_back(value->clone());
            }
        }
    };
}


