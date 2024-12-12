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
#include "../Deltares.Probabilistic/Combine/DesignPointCombiner.h"
#include "../Deltares.Probabilistic/Combine/HohenbichlerFORM.h"
#include "../Deltares.Probabilistic/Combine/combineElements.h"
#include "../Deltares.Probabilistic/Combine/upscaling.h"

using namespace Deltares::Reliability;

struct betaAlphaCF
{
    double  beta;
    double* alpha;
    double* rho;
    double* duration;
    double* correlation_length;
    int     size;
    int     stride_alpha;
    int     stride_duration;
};

struct multipleElements
{
    betaAlphaCF* designPoints;
    int size;
};

struct combinerSettings
{
    combineAndOr combAndOr;
    CombinerType combinerType;
};

elements fillElements(multipleElements* m_elements)
{
    auto elm = elements();
    const int nrStoch = m_elements->designPoints[0].size;
    for (int i = 0; i < m_elements->size; i++)
    {
        auto alpha = vector1D(nrStoch);
        for (int j = 0; j < nrStoch; j++)
        {
            alpha(j) = m_elements->designPoints[i].alpha[j*m_elements->designPoints[i].stride_alpha];
        }
        auto ab = alphaBeta(m_elements->designPoints[i].beta, alpha);
        elm.push_back(ab);
    }
    return elm;
}

extern "C"
void combineMultipleElementsGeneral(multipleElements* elements, betaAlphaCF* dpOut, combinerSettings* settings)
{
    const int nrStoch = dpOut->size;
    std::vector< std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
    for (int i = 0; i < nrStoch; i++)
    {
        auto s = std::make_shared<Deltares::Statistics::Stochast>();
        stochasts.push_back(s);
    }
    auto dpCombiner = DesignPointCombiner(settings->combinerType, Deltares::Numeric::MersenneTwister);
    auto designPoints = std::vector<std::shared_ptr<DesignPoint>>();
    for (int i = 0; i < elements->size; i++)
    {
        auto dp = std::make_shared<DesignPoint>();
        dp->Beta = elements->designPoints[i].beta;
        for (int j = 0; j < nrStoch; j++)
        {
            auto alphaj = std::make_shared<StochastPointAlpha>();
            alphaj->Alpha = elements->designPoints[i].alpha[j*elements->designPoints[i].stride_alpha];
            alphaj->Stochast = stochasts[j];
            alphaj->U = -dp->Beta * alphaj->Alpha;
            dp->Alphas.push_back(alphaj);
        }
        designPoints.push_back(dp);
    }
    auto selfCorrelation = std::make_shared<Deltares::Statistics::SelfCorrelationMatrix>();
    for (int i = 0; i < nrStoch; i++)
    {
        selfCorrelation->setSelfCorrelation(designPoints[0]->Alphas[i]->Stochast, elements->designPoints[0].rho[i]);
    }
    auto result = dpCombiner.combineDesignPoints(settings->combAndOr, designPoints, selfCorrelation);
    dpOut->beta = result->Beta;
    for (int j = 0; j < nrStoch; j++)
    {
        dpOut->alpha[j] = result->Alphas[j]->Alpha;
    }
}

extern "C"
int combinemultipleelements_c(multipleElements* elements, betaAlphaCF* dpOut)
{
    const int nrStoch = dpOut->size;
    auto cmb = combineElements();
    auto elm = fillElements(elements);
    auto rhoC = vector1D(nrStoch);
    for (int i = 0; i < nrStoch; i++)
    {
        rhoC(i) = elements->designPoints[0].rho[i];
    }
    auto result = cmb.combineMultipleElements(elm, rhoC, combineAndOr::combOr);

    dpOut->beta = result.ab.getBeta();
    for (int i = 0; i < nrStoch; i++)
    {
        dpOut->alpha[i*dpOut->stride_alpha] = result.ab.getAlphaI(i);
    }
    return result.n;
}

extern "C"
int upscalelengthc(betaAlphaCF* dpCrossSection, double* sectionLength, betaAlphaCF* dpSection)
{
    const int nStochasts = dpCrossSection->size;
    vector1D alpha = vector1D(nStochasts);
    vector1D rhoxk = vector1D(nStochasts);
    vector1D dxk = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        alpha(i) = dpCrossSection->alpha[i];
        rhoxk(i) = dpCrossSection->rho[i];
        dxk(i) = dpCrossSection->correlation_length[i];
    }
    auto crossSectionElement = alphaBeta(dpCrossSection->beta, alpha);

    auto up = upscaling();
    auto alphaBeta = up.upscaleLength(crossSectionElement, rhoxk, dxk, *sectionLength);

    dpSection->beta = alphaBeta.first.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        dpSection->alpha[i] = alphaBeta.first.getAlphaI(i);
    }
    return alphaBeta.second;
}

extern "C"
void upscaletolargestblockc(betaAlphaCF* dpSmallBlock, double* largestBlockDuration, betaAlphaCF* dpLargestBlock)
{
    int nStochasts = dpSmallBlock->size;
    auto alfasmall = vector1D(nStochasts);
    auto rho = vector1D(nStochasts);
    auto durations = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        alfasmall(i) = dpSmallBlock->alpha[i*dpSmallBlock->stride_alpha];
        rho(i) = dpSmallBlock->rho[i];
        durations(i) = dpSmallBlock->duration[i*dpSmallBlock->stride_duration];
    }
    auto smallBlock = alphaBeta(dpSmallBlock->beta, alfasmall);
    alphaBeta largestBlock;
    vector1D durationsLB;

    auto up = upscaling();
    up.upscaleToLargestBlock(smallBlock, rho, durations, *largestBlockDuration, largestBlock, durationsLB);

    dpLargestBlock->beta = largestBlock.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        dpLargestBlock->alpha[i*dpLargestBlock->stride_alpha] = largestBlock.getAlphaI(i);
        dpLargestBlock->duration[i*dpLargestBlock->stride_duration] = durationsLB(i);
    }
}

extern "C"
int combinetwoelementspartialcorrelationc2(betaAlphaCF* dp1, betaAlphaCF* dp2, betaAlphaCF* dpC, const int combAndOr)
{
    auto nStochasts = dp1->size;
    auto cmb = combineElements();
    auto rho = vector1D(nStochasts);
    auto alfa1 = vector1D(nStochasts);
    auto alfa2 = vector1D(nStochasts);
    for (int i = 0; i < nStochasts; i++)
    {
        rho(i) = dp1->rho[i];
        alfa1(i) = dp1->alpha[i];
        alfa2(i) = dp2->alpha[i];
    }
    auto elm1 = alphaBeta(dp1->beta, alfa1);
    auto elm2 = alphaBeta(dp2->beta, alfa2);
    auto elm = cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rho, static_cast<combineAndOr>(combAndOr));

    dpC->beta = elm.ab.getBeta();
    for (int i = 0; i < nStochasts; i++)
    {
        dpC->alpha[i] = elm.ab.getAlphaI(i);
    }
    return elm.n;
}

extern "C"
int combinemultipleelementsprob_c(multipleElements* elements, double* percentagesIn, betaAlphaCF* dpOut)
{
    const int nrElms = elements->size;
    const int nrStoch = dpOut->size;
    auto cmb = combineElements();
    auto elm = fillElements(elements);

    auto percentages = std::vector<double>(nrElms);
    for (int i = 0; i < nrElms; i++)
    {
        percentages[i] = percentagesIn[i];
    }

    auto result = cmb.combineMultipleElementsProb(elm, percentages, combineAndOr::combOr);

    dpOut->beta = result.ab.getBeta();
    for (int i = 0; i < nrStoch; i++)
    {
        dpOut->alpha[i*dpOut->stride_alpha] = result.ab.getAlphaI(i);
    }
    return result.n;
}

extern "C"
void calculate_combination_with_largest_correlation_c(multipleElements* elements, int* i1max, int* i2max)
{
    int nStoch = elements->designPoints[0].size;
    int nElements = elements->size;
    vector1D rho(nStoch);
    for (int i = 0; i < nStoch; i++) rho(i) = elements->designPoints[0].rho[i];
    std::vector<alphaBeta> alpha;
    for (int i = 0; i < nElements; i++)
    {
        auto alphai = vector1D(nStoch);
        for (int j = 1; j < nStoch; j++)
        {
            alphai(j) = elements->designPoints[i].alpha[j];
        }
        alphaBeta elm(elements->designPoints[i].beta, alphai);
        alpha.push_back(elm);
    }
    size_t j1max;
    size_t j2max;
    combineElements::calculateCombinationWithLargestCorrelation( rho, alpha, j1max, j2max);
    *i1max = static_cast<int>(j1max);
    *i2max = static_cast<int>(j2max);
}

