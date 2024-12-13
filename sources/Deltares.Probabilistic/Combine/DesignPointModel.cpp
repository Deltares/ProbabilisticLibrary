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
#include "DesignPointModel.h"

#include <map>
#include <unordered_set>

#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        void DesignPointModel::calculate(std::shared_ptr<ModelSample> sample)
        {
            // construct the tangent line of the circle with radius beta and in coordinates (- alfa1 * beta, -alfa2 * beta, ...)
            // then detect whether on which side (x1, x2, .. ) is located

            // for all points on the tangent line, the following is true:
            // sum (-alfa_i * x_i) = beta, since x_i = = - alfa_i * beta
            // so  beta - sum (-alfa_i * x_i) = 0
            // so  beta + sum (alfa_i * x_i) = 0

            // if all x are zero, then we expect a z_value of beta

            // check:
            // if alfa is positive, we expect a higher value for z with increasing x (strength variable)
            // if alfa is negative, we expect a lower value for z with increasing x (load variable)

            double result = this->designPoint->Beta;

            for (size_t i = 0; i < sample->Values.size(); i++)
            {
                result += sample->Values[i] * alphas[i];
            }

            if (this->ignored && this->inverted)
            {
                result = result > 0 ? nan("") : -result;
            }
            else if (this->inverted)
            {
                result = -result;
            }
            else if (this->ignored)
            {
                result = result > 0 ? nan("") : result;
            }

            sample->Z = result;
        }

        double DesignPointModel::getDirectionBeta(std::shared_ptr<ModelSample> sample)
        {
            this->calculate(sample);

            return Numeric::NumericSupport::interpolate(0, this->designPoint->Beta, 0, sample->Z, 1, true);
        }

        void DesignPointModel::setParameters(std::vector<std::shared_ptr<Statistics::Stochast>>& normalizedStochasts, std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Statistics::Stochast>> stochastsMap, std::map<std::shared_ptr<Statistics::Stochast>, std::shared_ptr<Reliability::DesignPoint>> designPointsMap)
        {
            alphas.clear();

            // take into account that stochasts in alphas are not unique
            std::unordered_set<std::shared_ptr<Models::StochastPointAlpha>> usedAlphas;

            for (size_t i = 0; i < normalizedStochasts.size(); i++)
            {
                double alphaValue = 0;

                std::shared_ptr<Statistics::Stochast> stochast = stochastsMap[normalizedStochasts[i]];
                const bool applicable = !designPointsMap.contains(normalizedStochasts[i]) || designPointsMap[normalizedStochasts[i]] == this->designPoint;

                if (applicable)
                {
                    for (size_t j = 0; j < this->designPoint->Alphas.size(); j++)
                    {
                        if (this->designPoint->Alphas[j]->Stochast == stochast && !usedAlphas.contains(this->designPoint->Alphas[j]))
                        {
                            alphaValue = this->designPoint->Alphas[j]->Alpha;
                            usedAlphas.insert(this->designPoint->Alphas[j]);
                            break;
                        }
                    }
                }

                alphas.push_back(alphaValue);
            }
        }

        bool DesignPointModel::isVarying(std::shared_ptr<Statistics::Stochast> stochast)
        {
            for (size_t j = 0; j < this->designPoint->Alphas.size(); j++)
            {
                if (this->designPoint->Alphas[j]->Stochast == stochast && this->designPoint->Alphas[j]->Alpha != 0.0)
                {
                    return true;
                }
            }

            return false;
        }
    }
}

