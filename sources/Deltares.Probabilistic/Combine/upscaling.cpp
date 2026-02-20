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
#include "upscaling.h"
#include "HohenbichlerFORM.h"
#include "intEqualElements.h"
#include "../Math/NumericSupport.h"
#include "../Statistics/StandardNormal.h"
#include <algorithm>
#include <cmath>
#include <numbers>
#include <format>
#include <limits>

#include "ComputeBetaSection.h"

using namespace Deltares::Statistics;

namespace Deltares::Reliability
{
    using namespace Deltares::Numeric;

    /// <summary>
    /// Method for combining failure probabilities over equal elements, with exceptions for correlations close to zero
    /// </summary>
    /// <param name="nrTimes"> the input design points </param>
    /// <param name="element"> the design point of a single time element </param>
    /// <param name="inRhoT"> Correlation coefficients for each of the variables, in time </param>
    /// <returns> the number of not converged intermediate calculations </returns>
    int upscaling::upscaleInTime(const double nrTimes, alphaBeta& element, const vector1D& inRhoT) const
    {
        constexpr double rhoMax = 1.0 - 1.0e-10;  // Max allowed value of correlation coefficient (very close to one)
        constexpr double rhoMin = 1.0e-10;        // Min allowed value of correlation coefficient (very close to zero)
        constexpr double epsi = 0.01;           // Perturbation amount, part of computation of equivalent alpha values

        size_t nStochasts = element.size(); // Number of stochastic variables
        int failures = 0;

        auto alphaT = vector1D(nStochasts); // Equivalent alpha values for all variables, of the combined element

        //
        // Determine the correlation between two time elements
        //
        double rhoT = element.sumOfInners(element, inRhoT); // rhoT : Correlation coefficient between element 1 and element 2
        rhoT = std::clamp(rhoT, rhoMin, rhoMax);

        //
        // Compute failure probability and beta of the combined n elements
        //
        auto betaT = upscaleBeta(element.getBeta(), rhoT, nrTimes, failures); // Beta value for the combined element

        //
        // Get equivalent alpha and beta values for the combined element in 6 steps
        //

        //
        // Compute the alpha value for the correlated part: alpha_correlated = [beta(epsi) - beta(0)]/epsi
        //
        // Step 1/6: Perturbed initial beta value, use u = epsi
        //
        auto bk = element.getBeta() - sqrt(rhoT) * epsi;

        //
        // Step 2/6: Compute the beta of the combined element, based on perturbed initial beta
        //

        // Beta value from perturbation of u, part of computation of equivalent alpha values
        auto betaTK = upscaleBeta(bk, rhoT, nrTimes, failures);

        //
        // Step 3/6: Compute the correlated part of alpha (dependent on the sign of the perturbation)
        //

        // alphaC : Correlated part of the equivalent alpha value
        double alphaC = std::min(1.0, std::max(-1.0, (betaT - betaTK) / epsi));
        if (alphaC != 1.0) // TODO: compare real with tol 1e-12 or so
        {
            //
            // Step 4/6: Compute the alpha value for the uncorrelated part of alpha
            //

            // alphaO : Uncorrelated part of the equivalent alpha value
            double alphaO = sqrt(1.0 - alphaC * alphaC);

            //
            // Step 5/6: Determine the combined equivalent alpha
            //
            for (size_t i = 0; i < nStochasts; i++)
            {
                alphaT(i) = alphaC / sqrt(rhoT) * element.getAlphaI(i) * sqrt(inRhoT(i))
                    + alphaO / sqrt(1.0 - rhoT) * element.getAlphaI(i) * sqrt(1.0 - inRhoT(i));
            }
            //
            // Step 6/6: Normalize the alpha TODO: make normalize optional and do it at the end of upscaleToLargestBlock
            //
            element.setAlpha(alphaT);
            element.normalize();
        }

        //
        // Write results over original input
        //
        element.setBeta(betaT);
        return failures;
    }

    // helper routine for upscaleInTime
    double upscaling::upscaleBeta(double elm, const double rhoT, const double nrTimes, int& failures) const
    {
        if (rhoT > rhoLowLim)
        {
            //
            // Compute via numerical integration
            //
            auto eqElm = intEqualElements();
            double betaT = eqElm.integrateEqualElements(elm, rhoT, nrTimes);
            return betaT;
        }
        else
        {
            //
            // Compute via Hohenbichler with FORM and out-crossing
            //
            double Pf = StandardNormal::getQFromU(elm);
            auto pf2cf1 = hhb.PerformHohenbichler(elm, Pf, rhoT);    // pf2cf1 : pair(Probability, success flag)
            if (pf2cf1.second != 0) failures++;
            double PfT = Pf + (nrTimes - 1.0) * (Pf - pf2cf1.first * Pf);
            PfT = std::min(1.0, PfT);
            return StandardNormal::getUFromQ(PfT);
        }
    }

    /// <summary> This method scales up from a cross-section to a given section length </summary>
    /// <param name="crossSectionElement">  Reliability index and alpha cross-section </param>
    /// <param name="rhoXK"> Correlation variables </param>
    /// <param name="dXK"> Correlation length variables </param>
    /// <param name="section_length">  Section length </param>
    /// <returns> design point after upscaling </returns>
    upscalingResult upscaling::upscaleLength(const alphaBeta& crossSectionElement,
        const vector1D& rhoXK, const vector1D& dXK, const double section_length) const
    {
        upscalingResult return_value;
        //
        // Get number of variables
        //
        const size_t number_of_stochasts = crossSectionElement.size();
        //
        // Calculate correlation
        //
        double rhoZ = section_length != 0.0 ? crossSectionElement.sumOfInners(crossSectionElement, rhoXK)
                                            :  std::numeric_limits<double>::max();
        if (rhoZ >= rhoLimit)
        {   // No length effect (correlation = 1)
            return_value.design_point = crossSectionElement;
            return return_value;
        }

        //
        // Calculate length effect
        //
        //
        // Calculate correlation length dz
        //
        double sumAlphaDxk = 0.0;
        for (size_t i = 0; i < number_of_stochasts; i++)
        {
            if (dXK(i) != 0.0)
            {
                sumAlphaDxk += (1.0 - rhoXK(i)) * pow(crossSectionElement.getAlphaI(i) / dXK(i), 2);
            }
        }
        const double dz = sqrt((1.0 - rhoZ) / sumAlphaDxk);

        auto betaSectionCalculator = ComputeBetaSection({ section_length, rhoZ, dz, crossSectionElement.getBeta() });
        return_value.message = betaSectionCalculator.createMessage();

        if (betaSectionCalculator.hasLengthEffect())
        {
            constexpr double delta_beta = 0.01;
            return_value.design_point.setAlpha(vector1D(number_of_stochasts));
            //
            // Calculate beta for section from the beta of the cross-section
            const auto betaSection = betaSectionCalculator.Compute(crossSectionElement.getBeta());
            return_value.design_point.setBeta(betaSection);
            //
            // Calculate alpha section
            //
            // Correlated part. Perturbation of the betaCrossSection
            const double betaK = crossSectionElement.getBeta() - sqrt(rhoZ) * delta_beta;
            // Calculate beta for section from the beta of the cross-section
            const auto betaKX = betaSectionCalculator.Compute(betaK);

            const double alphaC = std::clamp((return_value.design_point.getBeta() - betaKX) / delta_beta, -1.0, 1.0);
            //
            // Uncorrelated part
            //
            const double alphaU = sqrt(1.0 - alphaC * alphaC);
            //
            // Calculate resulting alpha
            //
            rhoZ = std::clamp(rhoZ, 0.00001, rhoLimit);
            const double deltaL = betaSectionCalculator.getDeltaL();
            for (size_t i = 0; i < number_of_stochasts; i++)
            {
                if (crossSectionElement.getAlphaI(i) == 0.0)
                {
                    return_value.design_point.setAlpha(i, 0.0);
                }
                else
                {
                    const double rhoV = rhoXK(i);
                    const double lengthV = dXK(i);
                    const double rhoK = rhoV + (1.0 - rhoV) * exp(-pow( deltaL / lengthV, 2));
                    return_value.design_point.setAlpha(i, alphaC / sqrt(rhoZ) * crossSectionElement.getAlphaI(i) *
                        sqrt(rhoK) +
                        alphaU / sqrt(1.0 - rhoZ) * crossSectionElement.getAlphaI(i) *
                        sqrt(1.0 - rhoK));
                }
            }
            //
            // Normalize alpha vector for section
            return_value.design_point.normalize();
            return_value.counter = betaSectionCalculator.getCounterNonConv();
        }
        else
        {
            // No length effect (breachL < sectionLength)
            return_value.design_point = crossSectionElement;
       }
        return return_value;
    }

    /// <summary> Method for upscaling random variables to the largest block duration </summary>
    /// <param name="smallBlock"> Reliability index and alpha input </param>
    /// <param name="rhoTSmallBlock"> Correlations </param>
    /// <param name="blockDurations"> Block durations vector input </param>
    /// <param name="largestBlockDuration"> Target block duration </param>
    /// <param name="largestBlock"> Reliability index and alpha result </param>
    /// <param name="durationsLargestBlock"> Block durations vector result </param>
    void upscaling::upscaleToLargestBlock(const alphaBeta& smallBlock,
        const vector1D& rhoTSmallBlock, const vector1D& blockDurations, const double largestBlockDuration,
        alphaBeta& largestBlock, vector1D& durationsLargestBlock) const
    {
        //
        // Get number of strength and load variables
        //
        const size_t nrVar = smallBlock.size();
        //
        // Allocate vectors
        //
        auto rhoT = vector1D(nrVar);

        //
        // Copy input for intermediate use
        //
        largestBlock = smallBlock;
        vector1D durations = blockDurations;

        for (;;)
        {
            //
            // Determine stochast with second shortest duration for upscaling
            double shortestDuration = durations.minval();
            double secondShortestDuration = largestBlockDuration;

            for (size_t i = 0; i < nrVar; i++)
            {
                if ((durations(i) < secondShortestDuration) &&
                    (durations(i) > shortestDuration))
                {
                    secondShortestDuration = durations(i);
                }
            }

            //
            // Check exit criterion
            //
            if (shortestDuration >= largestBlockDuration) break;

            //
            // Set rho values
            for (size_t i = 0; i < nrVar; i++)
            {
                if (durations(i) == shortestDuration)
                {
                    rhoT(i) = rhoTSmallBlock(i);
                    durations(i) = secondShortestDuration;
                }
                else
                {
                    rhoT(i) = 1.0;
                }
            }

            //
            // Upscale
            upscaleInTime(secondShortestDuration / shortestDuration, largestBlock, rhoT);
        }

        //
        // Copy results for output
        //
        durationsLargestBlock = durations;
    }
}
