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
#include "HohenbichlerFORM.h"

#include <format>
#include <numbers>
#include <vector>
#include <cmath>

using namespace Deltares::Statistics;

namespace Deltares::Reliability
{
    /// <summary> constructor </summary>
    /// <param name = "input"> input struct </param>
    ComputeBetaSection::ComputeBetaSection(const BetaSectionInput& input) : input(input)
    {
        deltaL = input.dz / input.beta * sqrt(std::numbers::pi) / sqrt(1.0 - input.rho_z);
        deltaL = std::max(deltaL, 0.01);
    }

    /// <summary> Method used in upscaling for computing the beta of a section from the beta of a cross-section. </summary>
    /// <param name = "beta_cross_section"> Reliability index of the cross-section </param>
    /// <returns> Reliability index for the section </returns>
    double ComputeBetaSection::Compute(const double beta_cross_section)
    {
        // Compute failure probability cross-section from beta
        const double pf = StandardNormal::getQFromU(beta_cross_section); // Failure probability cross-section
        double pfX; // Failure probability section
        if (input.rho_z > 0.001)
        {
            constexpr int number_of_grid_points = 30001;
            constexpr double lower_bound = -30.0; // Lower bound of v-values in the numerical integration
            constexpr double upper_bound = 30.0; // upper bound of v-values in the numerical integration

            // Step size in the numerical integration over v in [vLower, vUpper]
            constexpr double vDelta = (upper_bound - lower_bound) / static_cast<double>(number_of_grid_points - 1);

            const double p = StandardNormal::getPFromU(beta_cross_section); // Probability
            auto termI = std::vector<double>(number_of_grid_points); // i-th term to add
            for (size_t i = 0; i < number_of_grid_points; i++)
            {
                const double v = lower_bound + vDelta * static_cast<double>(i);
                const double x = (beta_cross_section - sqrt(input.rho_z) * v) / sqrt(1.0 - input.rho_z); // x is: beta*
                const double nf = (input.section_length - deltaL) / (std::numbers::sqrt2 * std::numbers::pi)
                    / input.dz * exp(-x * x / 2.0); // Number of cross-sections
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
            // Calculate failure probability vv with Hohenbichler
            constexpr auto hohenbichler = HohenbichlerFORM();
            auto [pfVV, nFail] = hohenbichler.PerformHohenbichler(beta_cross_section, pf, input.rho_z);
            conv += nFail;
            pfX = pf + (input.section_length - deltaL) / deltaL * (pf - pfVV * pf);
            pfX = std::min(pfX, 1.0);
        }

        const double betaSection = StandardNormal::getUFromQ(pfX);
        return betaSection;
    }

    /// <summary>
    /// Get the number of non-converged Hohenbichler runs
    /// </summary>
    int ComputeBetaSection::getCounterNonConv() const
    {
        return conv;
    }

    /// <summary>
    /// Get intermediate term deltaL
    /// </summary>
    double ComputeBetaSection::getDeltaL() const
    {
        return deltaL;
    }

    /// <summary>
    /// Get the flag whether we have a length effect at all
    /// </summary>
    bool ComputeBetaSection::hasLengthEffect() const
    {
        return deltaL < input.section_length;
    }

    /// <summary>
    /// Get a summary of intermediate terms
    /// </summary>
    std::string ComputeBetaSection::createMessage() const
    {
        return std::format("Intermediate results: Delta L = {:.6F}; rhoZ = {:.6F}; dZ = {:.6F}", deltaL, input.rho_z, input.dz);
    }

}
