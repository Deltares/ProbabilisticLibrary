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
#include "KSCalculator.h"
#include <cmath>
#include <algorithm>
#include <memory>
#include <vector>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        double KSCalculator::getGoodnessOfFit(std::vector<double>& xValues, std::shared_ptr<Distribution> distribution, std::shared_ptr<StochastProperties> stochast)
        {
            if (xValues.empty())
            {
                return std::nan("");
            }
            else
            {
                std::sort(xValues.begin(), xValues.end());

                double size = xValues.size();

                double ks = 0;

                for (int i = 0; i < xValues.size(); i++)
                {
                    double cdf = distribution->getCDF(stochast, xValues[i]);

                    double diff1 = std::fabs(cdf - i / size);
                    double diff2 = std::fabs(cdf - (i + 1) / size);

                    double diff = std::max(diff1, diff2);
                    if (diff > ks)
                    {
                        ks = diff;
                    }
                }

                return ks;
            }
        }
    }
}
