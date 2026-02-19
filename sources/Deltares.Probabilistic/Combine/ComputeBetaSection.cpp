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

#include "ComputeBetaSection.h"
#include "../Statistics/StandardNormal.h"

#include <format>
#include <numbers>
#include <vector>
#include <cmath>

using namespace Deltares::Statistics;

namespace Deltares::Reliability
{
    /// <summary> constructor </summary>
    /// <param name = "section_length"> Length of the section </param>
    /// <param name = "rhoZ"> Correlation Z-function </param>
    /// <param name = "dz"> Correlation length </param>
    /// <param name = "deltaL"> Delta L </param>
    ComputeBetaSection::ComputeBetaSection(const double section_length, const double rhoZ, const double dz, const double deltaL)
        : section_length(section_length), rhoZ(rhoZ), dz(dz), deltaL(deltaL)
    {
    }

    /// <summary> Method used in upscaling for computing the beta of a section from the beta of a cross-section. </summary>
    /// <param name = "betaCrossSection"> Reliability index of the cross-section </param>
    /// <returns> Reliability index for the section </returns>
    double ComputeBetaSection::Compute(const double betaCrossSection)
    {
        // Compute failure probability cross-section from beta
        const double pf = StandardNormal::getQFromU(betaCrossSection); // Failure probability cross-section
        double pfX; // Failure probability section
        if (rhoZ > 0.001)
        {
            constexpr int number_of_grid_points = 30001;
            constexpr double lower_bound = -30.0;  // Lower bound of v-values in the numerical integration
            constexpr double upper_bound = 30.0;
            double vDelta = (upper_bound - lower_bound) / static_cast<double>(number_of_grid_points - 1); // Step size in the numerical integration over v in [vLower, vUpper]
            double p = StandardNormal::getPFromU(betaCrossSection); // Probability
            auto termI = std::vector<double>(number_of_grid_points); // i-th term to add
            for (size_t i = 0; i < number_of_grid_points; i++)
            {
                const double v = lower_bound + vDelta * static_cast<double>(i);
                const double x = (betaCrossSection - sqrt(rhoZ) * v) / sqrt(1.0 - rhoZ); // x is: beta*
                const double nf = (section_length - deltaL) / (std::numbers::sqrt2 * std::numbers::pi) / dz * exp(-x * x / 2.0); // Number of cross-sections
                termI[i] = (1.0 - p * exp(-nf)) * exp(-v * v / 2.0) / sqrt(2.0 * std::numbers::pi) * vDelta;
            }
            //
            // add up all terms; start with the smallest numbers
            //
            pfX = 0.0;
            for (size_t i = 0; i < number_of_grid_points / 2; i++)
            {
                pfX += termI[i];
                pfX += termI[number_of_grid_points - i - 1];
            }
            pfX += termI[number_of_grid_points / 2];
            pfX = std::max(pfX, pf);
        }
        else
        {
            auto [pfVV, nFail] = hhb.PerformHohenbichler(betaCrossSection, pf, rhoZ); // Failure probability vv, Hohenbichler
            conv += nFail;
            pfX = pf + (section_length - deltaL) / deltaL * (pf - pfVV * pf);
            pfX = std::min(pfX, 1.0);
        }

        const double betaSection = StandardNormal::getUFromQ(pfX);
        return betaSection;
    }

    int ComputeBetaSection::getCounterNonConv() const
    {
        return conv;
    }

    std::string ComputeBetaSection::createMessage() const
    {
        return std::format("Intermediate results: Delta L = {0:.6F}; rhoZ = {1:.6F}; dZ = {2:.6F}", deltaL, rhoZ, dz);
    }

}
