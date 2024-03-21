#include <math.h>
#include <float.h>
#include "upscaling.h"
#include "Hohenbichler.h"
#include "intEqualElements.h"
#include "../Statistics/StandardNormal.h"

using namespace Deltares::Statistics;

namespace Deltares {
    namespace Reliability {

        //
        // Method for combining failure probabilities over equal elements, with exceptions for correlations close to zero
        //
        int upscaling::upscaleInTime(const double nrTimes, alphaBeta& element, const vector1D& inRhoT)
        {
            // nrTimes   : Number of time the origional time
            // beta      : Reliability index of a single time element
            // alpha(:)  : Influence coefficients of a single time element
            // inRhoT(:) : Correlation coefficients for each of the variables, in time

            // nStochasts : Number of stochastic variables
            // rhoT       : Correlation coefficient between element 1 and element 2
            // Pf         : Failure probability
            // PfT        : Failure probability of the combined element
            // betaT      : Beta value for the combined element
            // alphaT(:)  : Equivalent alpha values for all variables, of the combined element
            // betaK      : Perturbed initial beta, part of computation of equivalent alpha values
            // PfTK       : Failure probability from perturbation of u, part of computation of equivalent alpha values
            // betaTK     : Beta value from perturbation of u, part of computation of equivalent alpha values
            // alphaO     : Uncorrelated part of the equivalent alpha value
            // alphaC     : Correlated part of the equivalent alpha value

            const double rhoMax = 1.0 - 1.0e-10;  // Max allowed value of correlation coefficient (very close to one)
            const double rhoMin = 1.0e-10;        // Min allowed value of correlation coefficient (very close to zero)
            const double epsi = 0.01;           // Perturbation amount, part of computation of equivalent alpha values

            size_t nStochasts = element.size();
            int failures = 0;

            auto alphaT = vector1D(nStochasts);

            //
            // Determine the correlation between two time elements
            //
            double rhoT = element.sumOfInners(element, inRhoT);
            rhoT = std::max(std::min(rhoT, rhoMax), rhoMin);

            //
            // Compute failure probability and beta of the combined n elements
            //
            auto betaT = upscaleBeta(element.getBeta(), rhoT, nrTimes, failures);

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
            auto betaTK = upscaleBeta(bk, rhoT, nrTimes, failures);

            //
            // Step 3/6: Compute the correlated part of alpha (dependant on the sign of the pertubation)
            //
            double alphaC = std::min(1.0, std::max(-1.0, (betaT - betaTK) / epsi));
            if (alphaC != 1.0) // TODO: compare real with tol 1e-12 or so
            {
                //
                // Step 4/6: Compute the alpha value for the uncorrelated part of alpha
                //
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

        //
        // This method upscales from a cross section to a given section length
        std::pair<alphaBeta, int> upscaling::upscaleLength(alphaBeta& crossSectionElement,
            const vector1D& rhoXK, const vector1D& dXK, const double sectionLength, double breachL)
        {
            // crossSectionElement  : Reliability index and alpha cross section
            // rhoXK(:)             : Correlation variables
            // dXK(:)               : Correlation length variables
            // sectionLength        : Section length
            // BreachLength         : Breach length (mechanism width)

            const double deltaBeta = 0.01; // perturbation of beta in computation alpha in the upscaling from cross section to segment
            //
            // Get number of variables
            //
            size_t nrVar = crossSectionElement.size();
            //
            // Calculate correlation
            //
            double rhoZ;
            if (sectionLength != 0.0)
            {
                rhoZ = crossSectionElement.sumOfInners(crossSectionElement, rhoXK);
            }
            else
            {
                rhoZ = DBL_MAX;
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

            if (breachL < 0) breachL = deltaL;

            int failures = 0;

            if (rhoZ < rhoLimit && breachL < sectionLength)
            {
                alphaBeta element;
                element.setAlpha(vector1D(nrVar));
                //
                // Calculate beta for section from the beta of the cross section
                deltaL = std::min(deltaL, sectionLength);
                auto betaSection = ComputeBetaSection(crossSectionElement.getBeta(), sectionLength, breachL, rhoZ, dz, deltaL);
                if (betaSection.second != 0) failures++;
                element.setBeta(betaSection.first);
                //
                // Calculate alpha section
                //
                // Correlated part. Perturbation of the betaCrossSection
                double betaK = crossSectionElement.getBeta() - sqrt(rhoZ) * deltaBeta;
                // Calculate beta for section from the beta of the cross section
                auto betaKX = ComputeBetaSection(betaK, sectionLength, breachL, rhoZ, dz, deltaL);
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
                //
                // No length effect (correlation = 1)
                //
                return { crossSectionElement, 0 };
            }
        }

        //
        // Method used in upscaling for computing the beta of a section from the beta of a cross section.
        //
        std::pair<double, int> upscaling::ComputeBetaSection(const double betaCrossSection, const double sectionLength,
            const double breachL, const double rhoZ, const double dz, const double deltaL)
        {
            // betaCrossSection : Reliability index of the cross section
            // sectionLength    : Length of the section
            // breachL          : Breach length
            // rhoZ             : Correlation Z-function
            // dz               : Correlation length
            // deltaL           : Delta L
            // betaSection      : Reliability index of the section after upscaling

            const int    nGridPoints = 30001;    // Number gridpoints for v in numerical integration
            const double vLower = -30.0;  // Lower bound of v-values in the numerical integration
            const double vUpper = 30.0;  // Upper bound of v-values in the numerical integration
            // vDelta             : Step size in the numerical integration over v in [vLower, vUpper]
            // x                  : x value
            // v                  : v value
            // p                  : Probability
            // pf                 : Failure probability
            // nf                 : Number of cross section
            // pfX                : Failure probability section
            // pfVV               : Failure probability vv, Hohenbichler
            // termI(nGridPoints) : i-th term to add

        //   Compute failure probability cross section from beta
            double pf = StandardNormal::getQFromU(betaCrossSection);
            double pfX;
            int conv = 0;
            if (rhoZ > 0.001)
            {
                double vDelta = (vUpper - vLower) / double(nGridPoints - 1);
                double p = StandardNormal::getPFromU(betaCrossSection);
                if (sectionLength <= breachL)
                {
                    pfX = pf;
                }
                else
                {
                    auto termI = new double[nGridPoints + 1];
                    for (size_t i = 1; i <= nGridPoints; i++)
                    {
                        double v = vLower + vDelta * double(i - 1);
                        double x = (betaCrossSection - sqrt(rhoZ) * v) / sqrt(1.0 - rhoZ); // x = beta*
                        double nf = std::max((sectionLength - breachL), 0.0) / (sqrt(2.0) * M_PI) / dz * exp(-x * x / 2.0);
                        termI[i] = (1.0 - p * exp(-nf)) * exp(-v * v / 2.0) / sqrt(2.0 * M_PI) * vDelta;
                    }
                    //
                    // add up all terms; start with smallest numbers
                    //
                    pfX = 0.0;
                    for (size_t i = 1; i <= nGridPoints / 2; i++)
                    {
                        pfX += termI[i];
                        pfX += termI[nGridPoints + 1 - i];
                    }
                    pfX += termI[nGridPoints / 2 + 1];
                    delete[] termI;
                }
                pfX = std::max(pfX, pf);
                conv = 0;
            }
            else
            {
                auto hhb = Hohenbichler();
                auto pfVV = hhb.PerformHohenbichler(betaCrossSection, pf, rhoZ);
                conv = pfVV.second;
                pfX = pf + std::max((sectionLength - breachL), 0.0) / deltaL * (pf - pfVV.first * pf);
                pfX = std::min(pfX, 1.0);
            }

            double betaSection = StandardNormal::getUFromQ(pfX);
            return { betaSection, conv };
        }

        // Method for upscaling random variables to the largest block duration
        void upscaling::upscaleToLargestBlock(const alphaBeta& smallBlock,
            const vector1D& rhoTSmallBlock, const vector1D& blockDurations, const double largestBlockDuration,
            alphaBeta& largestBlock, vector1D& durationsLargestBlock)
        {
            // smallBlock               : Reliability index and alpha input
            // rhoTSmallBlock(:)        : Correlations
            // blockDurations(:)        : Block durations vector input
            // largestBlockDuration     : Target block duration
            // largestBlock             : Reliability index and alpha result
            // durationsLargestBlock(:) : Block durations vector result

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
