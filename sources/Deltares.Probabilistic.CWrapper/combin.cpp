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

elements fillElements(const multipleElements* m_elements)
{
    auto elm = elements();
    const int number_of_stochasts = m_elements->designPoints[0].size;
    for (int i = 0; i < m_elements->size; i++)
    {
        auto alpha = vector1D(number_of_stochasts);
        for (int j = 0; j < number_of_stochasts; j++)
        {
            alpha(j) = m_elements->designPoints[i].alpha[j*m_elements->designPoints[i].stride_alpha];
        }
        auto ab = alphaBeta(m_elements->designPoints[i].beta, alpha);
        elm.push_back(ab);
    }
    return elm;
}

/// <summary>
/// c-wrapper for combineDesignPoints
/// </summary>
/// <param name="elements"> the input design points </param>
/// <param name="dp_out"> the resulting design point </param>
/// <param name="settings"> settings for the combine calculation </param>
extern "C"
void combineMultipleElementsGeneral(const multipleElements* elements, betaAlphaCF* dp_out, const combinerSettings settings)
{
    const int number_of_stochasts = elements->designPoints[0].size;
    std::vector< std::shared_ptr<Deltares::Statistics::Stochast>> stochasts;
    for (int i = 0; i < number_of_stochasts; i++)
    {
        auto s = std::make_shared<Deltares::Statistics::Stochast>();
        stochasts.push_back(s);
    }
    auto dpCombiner = DesignPointCombiner(settings.combinerType);
    auto designPoints = std::vector<std::shared_ptr<DesignPoint>>();
    for (int i = 0; i < elements->size; i++)
    {
        auto dp = std::make_shared<DesignPoint>();
        dp->Beta = elements->designPoints[i].beta;
        for (int j = 0; j < number_of_stochasts; j++)
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
    for (int i = 0; i < number_of_stochasts; i++)
    {
        selfCorrelation->setSelfCorrelation(designPoints[0]->Alphas[i]->Stochast, elements->designPoints[0].rho[i]);
    }
    auto result = dpCombiner.combineDesignPoints(settings.combAndOr, designPoints, selfCorrelation);
    dp_out->beta = result->Beta;
    for (int j = 0; j < number_of_stochasts; j++)
    {
        dp_out->alpha[j] = result->Alphas[j]->Alpha;
    }
}

/// <summary>
/// c-wrapper for combineElements::combineMultipleElements
/// </summary>
/// <param name="elements"> the input design points </param>
/// <param name="resulting_design_point"> the resulting design point </param>
/// <returns> the number of not converged intermediate calculations </returns>
extern "C"
int combineMultipleElements(const multipleElements* elements, betaAlphaCF* resulting_design_point)
{
    const int number_of_stochasts = elements->designPoints[0].size;
    auto cmb = combineElements();
    auto elm = fillElements(elements);
    auto rhoC = vector1D(number_of_stochasts);
    for (int i = 0; i < number_of_stochasts; i++)
    {
        rhoC(i) = elements->designPoints[0].rho[i];
    }
    auto result = cmb.combineMultipleElements(elm, rhoC, combineAndOr::combOr);

    resulting_design_point->beta = result.ab.getBeta();
    for (int i = 0; i < number_of_stochasts; i++)
    {
        resulting_design_point->alpha[i*resulting_design_point->stride_alpha] = result.ab.getAlphaI(i);
    }
    return result.n;
}

/// <summary>
/// c-wrapper for upscaling::upscaleLength:
/// upscaling from cross-section to section with a certain length
/// </summary>
/// <param name="design_point_cross_section"> the input design point </param>
/// <param name="section_length"> the section length </param>
/// <param name="design_point_section"> the resulting design point </param>
/// <returns> the number of not converged intermediate calculations </returns>
extern "C"
int upscaleLength(const betaAlphaCF* design_point_cross_section,
    const double section_length, betaAlphaCF* design_point_section)
{
    const int number_of_stochasts = design_point_cross_section->size;
    vector1D alpha = vector1D(number_of_stochasts);
    vector1D rhoxk = vector1D(number_of_stochasts);
    vector1D dxk = vector1D(number_of_stochasts);
    for (int i = 0; i < number_of_stochasts; i++)
    {
        alpha(i) = design_point_cross_section->alpha[i];
        rhoxk(i) = design_point_cross_section->rho[i];
        dxk(i) = design_point_cross_section->correlation_length[i];
    }
    auto crossSectionElement = alphaBeta(design_point_cross_section->beta, alpha);

    auto up = upscaling();
    std::string message;
    const auto [alphaBeta, nFail] = up.upscaleLength(crossSectionElement, rhoxk, dxk, section_length, message);

    design_point_section->beta = alphaBeta.getBeta();
    for (int i = 0; i < number_of_stochasts; i++)
    {
        design_point_section->alpha[i] = alphaBeta.getAlphaI(i);
    }
    return nFail;
}

/// <summary>
/// c-wrapper for upscaling::upscaleToLargestBlock:
/// upscaling for stochasts with different time scales
/// </summary>
/// <param name="design_point_small_block"> the input design point </param>
/// <param name="largest_block_duration"> the largest duration </param>
/// <param name="design_point_largest_block"> the resulting design point </param>
extern "C"
void upscaleToLargestBlock(const betaAlphaCF* design_point_small_block, const double largest_block_duration, betaAlphaCF* design_point_largest_block)
{
    const int number_of_stochasts = design_point_small_block->size;
    auto alpha_small_block = vector1D(number_of_stochasts);
    auto rho = vector1D(number_of_stochasts);
    auto block_durations = vector1D(number_of_stochasts);
    for (int i = 0; i < number_of_stochasts; i++)
    {
        alpha_small_block(i) = design_point_small_block->alpha[i*design_point_small_block->stride_alpha];
        rho(i) = design_point_small_block->rho[i];
        block_durations(i) = design_point_small_block->duration[i*design_point_small_block->stride_duration];
    }
    const auto small_block = alphaBeta(design_point_small_block->beta, alpha_small_block);
    alphaBeta largest_block;
    vector1D durations_largest_block;

    auto up = upscaling();
    up.upscaleToLargestBlock(small_block, rho, block_durations, largest_block_duration,
        largest_block, durations_largest_block);

    design_point_largest_block->beta = largest_block.getBeta();
    for (int i = 0; i < number_of_stochasts; i++)
    {
        design_point_largest_block->alpha[i*design_point_largest_block->stride_alpha] = largest_block.getAlphaI(i);
        design_point_largest_block->duration[i*design_point_largest_block->stride_duration] = durations_largest_block(i);
    }
}

/// <summary>
/// c-wrapper for combineTwoElementsPartialCorrelation
/// </summary>
/// <param name="first_design_point"> the first input design point </param>
/// <param name="second_design_point"> the second input design point </param>
/// <param name="resulting_design_point"> the resulting design point </param>
/// <param name="combAndOr"> and/or switch; and=0, or=1 </param>
/// <returns> the number of not converged intermediate calculations </returns>
extern "C"
int combineTwoElementsPartialCorrelation(const betaAlphaCF* first_design_point, const betaAlphaCF* second_design_point,
    betaAlphaCF* resulting_design_point, const int combAndOr)
{
    const auto number_of_stochasts = first_design_point->size;
    auto cmb = combineElements();
    auto rho = vector1D(number_of_stochasts);
    auto alfa1 = vector1D(number_of_stochasts);
    auto alfa2 = vector1D(number_of_stochasts);
    for (int i = 0; i < number_of_stochasts; i++)
    {
        rho(i) = first_design_point->rho[i*first_design_point->stride_duration];
        alfa1(i) = first_design_point->alpha[i*first_design_point->stride_alpha];
        alfa2(i) = second_design_point->alpha[i*second_design_point->stride_alpha];
    }
    const auto elm1 = alphaBeta(first_design_point->beta, alfa1);
    const auto elm2 = alphaBeta(second_design_point->beta, alfa2);
    const auto [ab, n] =
        cmb.combineTwoElementsPartialCorrelation(elm1, elm2, rho, static_cast<combineAndOr>(combAndOr));

    resulting_design_point->beta = ab.getBeta();
    for (int i = 0; i < number_of_stochasts; i++)
    {
        resulting_design_point->alpha[i*resulting_design_point->stride_alpha] = ab.getAlphaI(i);
    }
    return n;
}

/// <summary>
/// c-wrapper for combineElements::combineMultipleElementsProb
/// </summary>
/// <param name="elements"> the input design points </param>
/// <param name="percentages_in"> percentages for each design point </param>
/// <param name="resulting_design_point"> the resulting design point </param>
/// <returns> the number of not converged intermediate calculations </returns>
extern "C"
int combineMultipleElementsProb(const multipleElements* elements, const double* percentages_in, betaAlphaCF* resulting_design_point)
{
    const int number_of_elements = elements->size;
    const int number_of_stochasts = resulting_design_point->size;
    auto cmb = combineElements();
    auto elm = fillElements(elements);

    auto percentages = std::vector<double>(number_of_elements);
    for (int i = 0; i < number_of_elements; i++)
    {
        percentages[i] = percentages_in[i];
    }

    const auto [ab, n] = cmb.combineMultipleElementsProb(elm, percentages, combineAndOr::combOr);

    resulting_design_point->beta = ab.getBeta();
    for (int i = 0; i < number_of_stochasts; i++)
    {
        resulting_design_point->alpha[i*resulting_design_point->stride_alpha] = ab.getAlphaI(i);
    }
    return n;
}

/// <summary>
/// c-wrapper for combineElements::calculateCombinationWithLargestCorrelation
/// </summary>
/// <param name="elements"> the input design points </param>
/// <param name="i1max"> the first index </param>
/// <param name="i2max"> the second index </param>
extern "C"
void calculateCombinationWithLargestCorrelation(const multipleElements* elements, int* i1max, int* i2max)
{
    const int number_of_stochasts = elements->designPoints[0].size;
    const int number_of_elements = elements->size;
    vector1D rho(number_of_stochasts);
    for (int i = 0; i < number_of_stochasts; i++) rho(i) = elements->designPoints[0].rho[i];
    std::vector<alphaBeta> alpha;
    for (int i = 0; i < number_of_elements; i++)
    {
        auto alpha_i = vector1D(number_of_stochasts);
        for (int j = 1; j < number_of_stochasts; j++)
        {
            alpha_i(j) = elements->designPoints[i].alpha[j];
        }
        alphaBeta elm(elements->designPoints[i].beta, alpha_i);
        alpha.push_back(elm);
    }
    size_t j1max;
    size_t j2max;
    combineElements::calculateCombinationWithLargestCorrelation( rho, alpha, j1max, j2max);
    *i1max = static_cast<int>(j1max);
    *i2max = static_cast<int>(j2max);
}

