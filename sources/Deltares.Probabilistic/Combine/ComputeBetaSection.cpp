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
    /// <summary> Method used in upscaling for computing the beta of a section from the beta of a cross-section. </summary>
    /// <param name = "betaCrossSection"> Reliability index of the cross-section </param>
    /// <param name = "sectionLength"> Length of the section </param>
    /// <param name = "rhoZ"> Correlation Z-function </param>
    /// <param name = "dz"> Correlation length </param>
    /// <param name = "deltaL"> Delta L </param>
    /// <returns> Reliability index for the section and indication of non-converged Hohenbichler calculation </returns>
    std::pair<double, int> ComputeBetaSection::Compute(const double betaCrossSection, const double sectionLength,
        const double rhoZ, const double dz, const double deltaL)
    {
        constexpr int    nGridPoints = 30001; // Number of grid points for v in numerical integration
        constexpr double vLower = -30.0;  // Lower bound of v-values in the numerical integration
        constexpr double vUpper = 30.0;  // Upper bound of v-values in the numerical integration

        // Compute failure probability cross-section from beta
        const double pf = StandardNormal::getQFromU(betaCrossSection); // Failure probability cross-section
        double pfX; // Failure probability section
        int conv; // indicator of non-converged Hohenbichler calculation
        if (rhoZ > 0.001)
        {
            double vDelta = (vUpper - vLower) / static_cast<double>(nGridPoints - 1); // Step size in the numerical integration over v in [vLower, vUpper]
            double p = StandardNormal::getPFromU(betaCrossSection); // Probability
            auto termI = std::vector<double>(nGridPoints); // i-th term to add
            for (size_t i = 0; i < nGridPoints; i++)
            {
                const double v = vLower + vDelta * static_cast<double>(i);
                const double x = (betaCrossSection - sqrt(rhoZ) * v) / sqrt(1.0 - rhoZ); // x is: beta*
                const double nf = (sectionLength - deltaL) / (std::numbers::sqrt2 * std::numbers::pi) / dz * exp(-x * x / 2.0); // Number of cross-sections
                termI[i] = (1.0 - p * exp(-nf)) * exp(-v * v / 2.0) / sqrt(2.0 * std::numbers::pi) * vDelta;
            }
            //
            // add up all terms; start with the smallest numbers
            //
            pfX = 0.0;
            for (size_t i = 0; i < nGridPoints / 2; i++)
            {
                pfX += termI[i];
                pfX += termI[nGridPoints - i - 1];
            }
            pfX += termI[nGridPoints / 2];
            pfX = std::max(pfX, pf);
            conv = 0;
        }
        else
        {
            auto [pfVV, nFail] = hhb.PerformHohenbichler(betaCrossSection, pf, rhoZ); // Failure probability vv, Hohenbichler
            conv = nFail;
            pfX = pf + (sectionLength - deltaL) / deltaL * (pf - pfVV * pf);
            pfX = std::min(pfX, 1.0);
        }

        const double betaSection = StandardNormal::getUFromQ(pfX);
        return { betaSection, conv };
    }

    std::string ComputeBetaSection::createMessage(const double deltaL, const double rhoZ, const double dZ)
    {
        return std::format("Intermediate results: Delta L = {0:.6F}; rhoZ = {1:.6F}; dZ = {2:.6F}", deltaL, rhoZ, dZ);
    }

}
