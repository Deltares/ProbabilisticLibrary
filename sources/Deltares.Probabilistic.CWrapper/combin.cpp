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
#include "../Deltares.Probabilistic/Combine/DesignPointCombiner.h"
#include "../Deltares.Probabilistic/Combine/HohenbichlerFORM.h"
#include "../Deltares.Probabilistic/Combine/combineElements.h"
#include "../Deltares.Probabilistic/Combine/upscaling.h"
#include "../Deltares.Probabilistic/Combine/intEqualElements.h"

using namespace Deltares::Reliability;

elements fillElements(double* betaElement, double* alphaElement, int nrElms, int nrStoch)
{
    auto elm = elements();
    for (int i = 0; i < nrElms; i++)
    {
        auto alpha = vector1D(nrStoch);
        for (int j = 0; j < nrStoch; j++)
        {
            alpha(j) = alphaElement[i + j * nrElms];
        }
        auto ab = alphaBeta(betaElement[i], alpha);
        elm.push_back(ab);
    }
    return elm;
}

extern "C"
void combineMultipleElementsGeneral(double* betaElement, double* alphaElement, double* rho,
    double* beta, double* alpha, combineAndOr combAndOrIn, CombinerType combinerType, int nrElms, int nrStoch)
{
    std::vector< std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
    for (int i = 0; i < nrStoch; i++)
    {
        auto s = std::make_shared<Deltares::Statistics::Stochast>();
        stochasts.push_back(s);
    }
    auto dpCombiner = DesignPointCombiner(combinerType, Deltares::Numeric::MersenneTwister);
    auto designPoints = std::vector<std::shared_ptr<DesignPoint>>();
    for (int i = 0; i < nrElms; i++)
    {
        auto dp = std::make_shared<DesignPoint>();
        dp->Beta = betaElement[i];
        for (int j = 0; j < nrStoch; j++)
        {
            auto alphaj = std::make_shared<StochastPointAlpha>();
            alphaj->Alpha = alphaElement[i + j * nrElms];
            alphaj->Stochast = stochasts[j];
            alphaj->U = -dp->Beta * alphaj->Alpha;
            dp->Alphas.push_back(alphaj);
        }
        designPoints.push_back(dp);
    }
    auto selfCorrelation = std::make_shared<Deltares::Statistics::SelfCorrelationMatrix>();
    for (int i = 0; i < nrStoch; i++)
    {
        selfCorrelation->setSelfCorrelation(designPoints[0]->Alphas[i]->Stochast, rho[i]);
    }
    auto result = dpCombiner.combineDesignPoints(combAndOrIn, designPoints, selfCorrelation);
    *beta = result->Beta;
    for (int j = 0; j < nrStoch; j++)
    {
        alpha[j] = result->Alphas[j]->Alpha;
    }
}

extern "C"
int combinemultipleelements_c(double* betaElement, double* alphaElement, double* rho,
    double* beta, double* alpha, combineAndOr combAndOrIn, int nrElms, int nrStoch)
{
    auto cmb = combineElements();
    auto elm = fillElements(betaElement, alphaElement, nrElms, nrStoch);
    auto rhoC = vector1D(nrStoch);
    for (int i = 0; i < nrStoch; i++)
    {
        rhoC(i) = rho[i];
    }
    auto result = cmb.combineMultipleElements(elm, rhoC, combAndOrIn);

    *beta = result.ab.getBeta();
    for (int i = 0; i < nrStoch; i++)
    {
        alpha[i] = result.ab.getAlphaI(i);
    }
    return result.n;
}

extern "C"
int upscalelengthc(double* betaCrossSection, double* alphaCrossSection, double* rhoXK, double* dXK,
    double* sectionLength, double* betaSection, double* alphaSection, double* breachLength, int nStochasts)
{
    vector1D alpha = vector1D(nStochasts);
    vector1D rhoxk = vector1D(nStochasts);
    vector1D dxk = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        alpha(i) = alphaCrossSection[i];
        rhoxk(i) = rhoXK[i];
        dxk(i) = dXK[i];
    }
    auto crossSectionElement = alphaBeta(*betaCrossSection, alpha);

    auto up = upscaling();
    auto alphaBeta = up.upscaleLength(crossSectionElement, rhoxk, dxk, *sectionLength, *breachLength);

    *betaSection = alphaBeta.first.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        alphaSection[i] = alphaBeta.first.getAlphaI(i);
    }
    return alphaBeta.second;
}

extern "C"
int upscaleintimec(double* nrTimes, double* beta, double* alpha, double* inRhoT, int nStochasts)
{
    auto alfa = vector1D(nStochasts);
    auto rho = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        alfa(i) = alpha[i];
        rho(i) = inRhoT[i];
    }
    auto ab = alphaBeta(*beta, alfa);

    auto up = upscaling();
    int n = up.upscaleInTime(*nrTimes, ab, rho);

    *beta = ab.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        alpha[i] = ab.getAlphaI(i);
    }
    return n;
}

extern "C"
void upscaletolargestblockc(double* betaSmallBlock, double* alphaSmallBlock, double* rhoTSmallBlock,
    double* blockDurations, double* largestBlockDuration,
    double* betaLargestBlock, double* alphaLargestBlock, double* durationsLargestBlock, int nStochasts)
{
    auto alfasmall = vector1D(nStochasts);
    auto rho = vector1D(nStochasts);
    auto durations = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        alfasmall(i) = alphaSmallBlock[i];
        rho(i) = rhoTSmallBlock[i];
        durations(i) = blockDurations[i];
    }
    auto smallBlock = alphaBeta(*betaSmallBlock, alfasmall);
    alphaBeta largestBlock;
    vector1D durationsLB;

    auto up = upscaling();
    up.upscaleToLargestBlock(smallBlock, rho, durations, *largestBlockDuration, largestBlock, durationsLB);

    *betaLargestBlock = largestBlock.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        alphaLargestBlock[i] = largestBlock.getAlphaI(i);
        durationsLargestBlock[i] = durationsLB(i);
    }
}

extern "C"
void integrateequalelements(double * beta, double * rhoT, double * nrElements, double * betaT)
{
    auto ee = intEqualElements();
    *betaT = ee.integrateEqualElements(*beta, *rhoT, *nrElements);
}

extern "C"
int combinetwoelementspartialcorrelationc1(double* beta1, double* alpha1, double* beta2, double* alpha2, double* rhoP,
    double* betaC, double* alphaC, const int combAndOr, const int nStochasts, double* alphaI, double* alphaII)
{
    auto rho = vector1D(nStochasts);
    auto alfa1 = vector1D(nStochasts);
    auto alfa2 = vector1D(nStochasts);
    auto alfai = vector1D(nStochasts);
    auto alfaii = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        rho(i) = rhoP[i];
        alfa1(i) = alpha1[i];
        alfa2(i) = alpha2[i];
    }
    auto elm1 = alphaBeta(*beta1, alfa1);
    auto elm2 = alphaBeta(*beta2, alfa2);

    auto cmb = combineElements();
    auto elm = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rho, (combineAndOr)combAndOr, alfai, alfaii);

    *betaC = elm.ab.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        alphaC[i] = elm.ab.getAlphaI(i);
        alphaI[i] = alfai(i);
        alphaII[i] = alfaii(i);
    }
    return elm.n;
}

extern "C"
int combinetwoelementspartialcorrelationc2(double* beta1, double* alpha1, double* beta2, double* alpha2, double* rhoP,
    double* betaC, double* alphaC, const int combAndOr, const int nStochasts)
{
    auto cmb = combineElements();
    auto rho = vector1D(nStochasts);
    auto alfa1 = vector1D(nStochasts);
    auto alfa2 = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        rho(i) = rhoP[i];
        alfa1(i) = alpha1[i];
        alfa2(i) = alpha2[i];
    }
    auto elm1 = alphaBeta(*beta1, alfa1);
    auto elm2 = alphaBeta(*beta2, alfa2);
    auto elm = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rho, (combineAndOr)combAndOr);

    *betaC = elm.ab.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        alphaC[i] = elm.ab.getAlphaI(i);
    }
    return elm.n;
}

extern "C"
int combinemultipleelementsspatialcorrelated_c(double* betaElement, double* alphaElement, double* rhoIn,
    double* beta, double* alpha, const combineAndOr combAndOr, int nrElms, int nrStoch)
{
    auto cmb = combineElements();
    auto elm = fillElements(betaElement, alphaElement, nrElms, nrStoch);

    std::vector<std::vector<vector1D>> rho;
    for (int k = 0; k < nrElms; k++)
    {
        auto q = std::vector<vector1D>();
        for (int j = 0; j < nrElms; j++)
        {
            auto r = vector1D(nrStoch);
            for (int i = 0; i < nrStoch; i++)
            {
                r(i) = rhoIn[j + nrElms * k + nrElms * nrElms * i];
            }
            q.push_back(r);
        }
        rho.push_back(q);
    }

    auto result = cmb.combineMultipleElementsSpatialCorrelated(elm, rho, combAndOr);

    *beta = result.ab.getBeta();
    for (int i = 0; i < nrStoch; i++)
    {
        alpha[i] = result.ab.getAlphaI(i);
    }
    return result.n;
}

extern "C"
int combinemultipleelementsprob_c(double* betaElement, double* alphaElement, double* percentagesIn,
    double* beta, double* alpha, const int combAndOr, int nrElms, int nrStoch)
{
    auto cmb = combineElements();
    auto elm = fillElements(betaElement, alphaElement, nrElms, nrStoch);

    auto percentages = std::vector<double>(nrElms);
    for (int i = 0; i < nrElms; i++)
    {
        percentages[i] = percentagesIn[i];
    }

    auto result = cmb.combineMultipleElementsProb(elm, percentages, (combineAndOr)combAndOr);

    *beta = result.ab.getBeta();
    for (int i = 0; i < nrStoch; i++)
    {
        alpha[i] = result.ab.getAlphaI(i);
    }
    return result.n;
}

