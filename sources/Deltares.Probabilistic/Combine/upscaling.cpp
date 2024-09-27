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
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <cfloat>
#include "upscaling.h"
#include "HohenbichlerFORM.h"
#include "intEqualElements.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        // \brief Method for combining failure probabilities over equal elements, with exceptions for correlations close to zero
        // \param nrTimes   : Number of time the original time
        // \param beta      : Reliability index of a single time element
        // \param alpha(:)  : Influence coefficients of a single time element
        // \param inRhoT(:) : Correlation coefficients for each of the variables, in time
        int upscaling::upscaleInTime(const double nrTimes, alphaBeta& element, const vector1D& inRhoT)
        {
            const double rhoMax = 1.0 - 1.0e-10;  // Max allowed value of correlation coefficient (very close to one)
            const double rhoMin = 1.0e-10;        // Min allowed value of correlation coefficient (very close to zero)
            const double epsi = 0.01;           // Perturbation amount, part of computation of equivalent alpha values

            size_t nStochasts = element.size(); // Number of stochastic variables
            int failures = 0;

            auto alphaT = vector1D(nStochasts); // Equivalent alpha values for all variables, of the combined element

            //
            // Determine the correlation between two time elements
            //
            double rhoT = element.sumOfInners(element, inRhoT); // rhoT : Correlation coefficient between element 1 and element 2
            rhoT = std::max(std::min(rhoT, rhoMax), rhoMin);

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
            // Step 3/6: Compute the correlated part of alpha (dependant on the sign of the pertubation)
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
        double upscaling::upscaleBeta(double elm, const double rhoT, const double nrTimes, int& failures)
        {
            if (rhoT > rhoLowLim)
            {
                //
                // Compute via numerical integration
                //
                auto eqElm = intEqualElements();
                double betaT;
                betaT = eqElm.integrateEqualElements(elm, rhoT, nrTimes);
                return betaT;
            }
            else
            {
                //
                // Compute via Hohenbichler with FORM and outcrossing
                //
                double Pf = StandardNormal::getQFromU(elm);
                auto pf2cf1 = hhb.PerformHohenbichler(elm, Pf, rhoT);    // pf2cf1 : pair(Probability, success flag)
                if (pf2cf1.second != 0) failures++;
                double PfT = Pf + (nrTimes - 1.0) * (Pf - pf2cf1.first * Pf);
                PfT = std::min(1.0, PfT);
                return StandardNormal::getUFromQ(PfT);
            }
        }

        // \brief This method upscales from a cross section to a given section length
        // \param crossSectionElement : Reliability index and alpha cross section
        // \param rhoXK(:) : Correlation variables
        // \param dXK(:) : Correlation length variables
        // \param sectionLength : Section length
        // \param minimumFailureLength : minimum failure length (breach length in certain mechanisms)
        // \return Reliability index, alpha for section and indication of non-converged Hohenbichler calculations
        std::pair<alphaBeta, int> upscaling::upscaleLength(alphaBeta& crossSectionElement,
            const vector1D& rhoXK, const vector1D& dXK, const double sectionLength, double minimumFailureLength)
        {
            const double deltaBeta = 0.01; // perturbation of beta in computation alpha in the upscaling from cross section to segment
            //
            // Get number of variables
            //
            size_t nrVar = crossSectionElement.size();
            //
            // Calculate correlation
            //
            double rhoZ = DBL_MAX;
            if (sectionLength != 0.0)
            {
                rhoZ = crossSectionElement.sumOfInners(crossSectionElement, rhoXK);
            }
            if (rhoZ >= rhoLimit)
            {   // No length effect (correlation = 1)
                return { crossSectionElement, 0 };
            }

            //
            // Calculate length effect
            //
            //
            // Calculate correlation length dz
            //
            double dz = 0.0;
            for (size_t i = 0; i < nrVar; i++)
            {
                if (dXK(i) != 0.0)
                {
                    dz += (1.0 - rhoXK(i)) * pow(crossSectionElement.getAlphaI(i) / dXK(i), 2);
                }
            }
            dz = dz / (1.0 - rhoZ);
            dz = sqrt(1.0 / dz);
            //
            // Calculate delta L and breach L
            //
            double deltaL = dz / crossSectionElement.getBeta() * sqrt(M_PI) / sqrt(1.0 - rhoZ);
            deltaL = std::max(deltaL, 0.01);

            if (minimumFailureLength < 0) minimumFailureLength = deltaL;

            int failures = 0;

            if (minimumFailureLength < sectionLength)
            {
                alphaBeta element;
                element.setAlpha(vector1D(nrVar));
                //
                // Calculate beta for section from the beta of the cross section
                deltaL = std::min(deltaL, sectionLength);
                auto betaSection = ComputeBetaSection(crossSectionElement.getBeta(), sectionLength, minimumFailureLength, rhoZ, dz, deltaL);
                if (betaSection.second != 0) failures++;
                element.setBeta(betaSection.first);
                //
                // Calculate alpha section
                //
                // Correlated part. Perturbation of the betaCrossSection
                double betaK = crossSectionElement.getBeta() - sqrt(rhoZ) * deltaBeta;
                // Calculate beta for section from the beta of the cross section
                auto betaKX = ComputeBetaSection(betaK, sectionLength, minimumFailureLength, rhoZ, dz, deltaL);
                if (betaKX.second != 0) failures++;

                double alphaC = (element.getBeta() - betaKX.first) / deltaBeta;
                alphaC = std::min(alphaC, 1.0);
                alphaC = std::max(alphaC, -1.0);
                //
                // Uncorrelated part
                //
                double alphaU = sqrt(1.0 - alphaC * alphaC);
                //
                // Calculate resulting alpha
                //
                rhoZ = std::min(rhoZ, rhoLimit);
                rhoZ = std::max(rhoZ, 0.00001);
                for (size_t i = 0; i < nrVar; i++)
                {
                    if (crossSectionElement.getAlphaI(i) == 0.0)
                    {
                        element.setAlpha(i, 0.0);
                    }
                    else
                    {
                        double rhoV = rhoXK(i);
                        double lengthV = dXK(i);
                        double rhoK = rhoV + (1.0 - rhoV) * exp(-pow(deltaL / lengthV, 2));
                        element.setAlpha(i, alphaC / sqrt(rhoZ) * crossSectionElement.getAlphaI(i) *
                            sqrt(rhoK) +
                            alphaU / sqrt(1.0 - rhoZ) * crossSectionElement.getAlphaI(i) *
                            sqrt(1.0 - rhoK));
                    }
                }
                //
                // Normalize alpha vector for section
                element.normalize();
                return { element, failures };
            }
            else
            {
                // No length effect (breachL < sectionLength)
                return { crossSectionElement, 0 };
            }
        }

        // \brief Method used in upscaling for computing the beta of a section from the beta of a cross section.
        // \param betaCrossSection : Reliability index of the cross section
        // \param sectionLength : Length of the section
        // \param minimumFailureLength : minimum failure length (breach length in certain mechanisms)
        // \param rhoZ : Correlation Z-function
        // \param dz : Correlation length
        // \param deltaL : Delta L
        // \return Reliability index for the section and indication of non-converged Hohenbichler calculation
        std::pair<double, int> upscaling::ComputeBetaSection(const double betaCrossSection, const double sectionLength,
            const double minimumFailureLength, const double rhoZ, const double dz, const double deltaL)
        {
            const int    nGridPoints = 30001;    // Number gridpoints for v in numerical integration
            const double vLower = -30.0;  // Lower bound of v-values in the numerical integration
            const double vUpper = 30.0;  // Upper bound of v-values in the numerical integration

        //   Compute failure probability cross section from beta
            double pf = StandardNormal::getQFromU(betaCrossSection); // Failure probability cross section
            double pfX; // Failure probability section
            int conv = 0;
            if (rhoZ > 0.001)
            {
                double vDelta = (vUpper - vLower) / double(nGridPoints - 1); // Step size in the numerical integration over v in [vLower, vUpper]
                double p = StandardNormal::getPFromU(betaCrossSection); // Probability
                if (sectionLength <= minimumFailureLength)
                {
                    pfX = pf;
                }
                else
                {
                    auto termI = std::vector<double>(nGridPoints); // i-th term to add
                    for (size_t i = 0; i < nGridPoints; i++)
                    {
                        double v = vLower + vDelta * double(i);
                        double x = (betaCrossSection - sqrt(rhoZ) * v) / sqrt(1.0 - rhoZ); // x = beta*
                        double nf = std::max((sectionLength - minimumFailureLength), 0.0) / (sqrt(2.0) * M_PI) / dz * exp(-x * x / 2.0); // Number of cross section
                        termI[i] = (1.0 - p * exp(-nf)) * exp(-v * v / 2.0) / sqrt(2.0 * M_PI) * vDelta;
                    }
                    //
                    // add up all terms; start with smallest numbers
                    //
                    pfX = 0.0;
                    for (size_t i = 0; i < nGridPoints / 2; i++)
                    {
                        pfX += termI[i];
                        pfX += termI[nGridPoints - i - 1];
                    }
                    pfX += termI[nGridPoints / 2];
                }
                pfX = std::max(pfX, pf);
                conv = 0;
            }
            else
            {
                auto pfVV = hhb.PerformHohenbichler(betaCrossSection, pf, rhoZ); // Failure probability vv, Hohenbichler
                conv = pfVV.second;
                pfX = pf + std::max((sectionLength - minimumFailureLength), 0.0) / deltaL * (pf - pfVV.first * pf);
                pfX = std::min(pfX, 1.0);
            }

            double betaSection = StandardNormal::getUFromQ(pfX);
            return { betaSection, conv };
        }

        // \brief Method for upscaling random variables to the largest block duration
        // \param smallBlock : Reliability index and alpha input
        // \param rhoTSmallBlock(:) : Correlations
        // \param blockDurations(:) : Block durations vector input
        // \param largestBlockDuration : Target block duration
        // \param largestBlock : Reliability index and alpha result
        // \param durationsLargestBlock(:) : Block durations vector result
        void upscaling::upscaleToLargestBlock(const alphaBeta& smallBlock,
            const vector1D& rhoTSmallBlock, const vector1D& blockDurations, const double largestBlockDuration,
            alphaBeta& largestBlock, vector1D& durationsLargestBlock)
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
}
