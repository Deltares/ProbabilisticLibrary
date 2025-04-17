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
#include "DirectionSectionsCalculation.h"

#include <iostream>

#include "../Math/NumericSupport.h"
#include "../Math/RootFinders/LinearRootFinder.h"
#include "../Math/RootFinders/BisectionRootFinder.h"

namespace Deltares::Reliability
{
    using namespace Deltares::Numeric;

    double DirectionSectionsCalculation::getBetaFromSections(const std::vector<DirectionSection>& sections,
        const bool FindMinimalValue)
    {
        // sum the probabilities
        double failingProbability = 0.0;
        double nonFailingProbability = 0.5; // start counting at u = 0

        for (int i = sections.size() - 1; i >= 0; i--)
        {
            switch (sections[i].Type)
            {
            case DoubleType::Positive:
                nonFailingProbability += sections[i].getProbability();
                break;
            case DoubleType::Negative:
                failingProbability += sections[i].getProbability();
                break;
            default:
                //nothing to do
                break;
            }
        }

        if (failingProbability == 0 && nonFailingProbability == 0.5)
        {
            return nan("");
        }
        else if (nonFailingProbability == 0.5 && FindMinimalValue)
        {
            double zmin = 1e99;
            double rmin = 0.0;
            for (const auto& section : sections)
            {
                if (section.ZHigh < zmin && section.ZHigh != 0.0)
                {
                    rmin = section.UHigh;
                    zmin = section.ZHigh;
                }
            }
            return rmin;
        }
        else
        {
            double probFailure = failingProbability / (failingProbability + nonFailingProbability);
            return Statistics::StandardNormal::getUFromQ(probFailure);
        }
    }

    bool DirectionSectionsCalculation::CanPrecomputeSample() const
    {
        if (Settings.modelVaryingType == ModelVaryingType::Varying) return true;

        const auto nValues = zValues.values.size();
        const auto last = zValues.values.back().z;
        double previous = -999.0;
        if (nValues > 1) previous = zValues.values[nValues - 2].z;

        const bool signChanged = z0 * last < 0.0;
        const bool wrongDirection = std::abs(last) > std::abs(previous);
        const bool done = (signChanged || wrongDirection);
        return !done;
    }

    double DirectionSectionsCalculation::GetPrecomputeUvalue() const
    {
        double uDivDsdu = static_cast<double>(zValues.values.size());
        return std::min(uDivDsdu * Settings.Dsdu, Settings.MaximumLengthU);
    }

    void DirectionSectionsCalculation::ProvidePrecomputeValue(const PrecomputeValue& zValue)
    {
        zValues.values.push_back(zValue);
    }

    std::vector<DirectionSection> DirectionSectionsCalculation::getDirectionSections(Models::ModelRunner& modelRunner,
        const BetaValueTask& directionTask)
    {
        std::vector<DirectionSection> sections;

        const int sectionsCount = Settings.SectionCount();

        bool found = false;
        const bool monotone = Settings.modelVaryingType == ModelVaryingType::Monotone;
        const auto dirCalcSettings = DirectionCalculationSettings(directionTask.invertZ, Settings.Dsdu, Settings.MaximumLengthU);
        auto model = ZGetter(modelRunner, Settings);
        auto directionCalculation = DirectionCalculation(model, *directionTask.UValues, dirCalcSettings);
        double prevzHigh = directionCalculation.GetZ(0, zValues);

        for (int k = 0; k <= sectionsCount && !this->isStopped(); k++)
        {
            if (!found)
            {
                double uLow = k * Settings.Dsdu;
                double uHigh = std::min((k + 1) * Settings.Dsdu, Settings.MaximumLengthU);

                double zLow = prevzHigh;
                double zHigh = directionCalculation.GetZ(k+1, zValues);

                prevzHigh = zHigh;

                DoubleType zHighType = NumericSupport::getDoubleType(zHigh);
                DoubleType zLowType = NumericSupport::getDoubleType(zLow);

                if (zHighType != zLowType)
                {
                    double zChange = nan("");
                    double uChange = findBetaBetweenBoundaries(modelRunner, directionCalculation, uLow, uHigh, zLow, zHigh, zChange);

                    DoubleType zChangeType = NumericSupport::getDoubleType(zChange);

                    if (zLowType == DoubleType::NaN && zChangeType != zHighType)
                    {
                        sections.emplace_back(zLowType, uLow, uChange);

                        double uSubChange = findBetaBetweenBoundaries(modelRunner, directionCalculation, uChange, uHigh, zChange, zHigh, zChange);

                        sections.emplace_back(zChangeType, uChange, uSubChange);
                        sections.emplace_back(zHighType, uSubChange, uHigh);
                    }
                    else if (zHighType == DoubleType::NaN && zChangeType != zLowType)
                    {
                        double uSubChange = findBetaBetweenBoundaries(modelRunner, directionCalculation, uLow, uChange, zLow, zChange, zChange);

                        sections.emplace_back(zLowType, uLow, uSubChange);
                        sections.emplace_back(zChangeType, uSubChange, uChange);
                        sections.emplace_back(zHighType, uChange, uHigh);
                    }
                    else
                    {
                        sections.emplace_back(zLowType, uLow, uChange, zLow, zChange);
                        sections.emplace_back(zHighType, uChange, uHigh, zHigh, zChange);
                    }

                    if (monotone)
                    {
                        found = true;
                    }
                }
                else
                {
                    sections.emplace_back(zHighType, uLow, uHigh, zLow, zHigh);

                    if (monotone && zLowType != DoubleType::NaN && zHighType != DoubleType::NaN)
                    {
                        if (NumericSupport::compareDouble(std::fabs(zHigh), std::fabs(zLow)) == CmpResult::Greater)
                        {
                            found = true;
                        }
                    }
                }
            }
        }

        // add the remainder to the last
        auto lastSection = sections.back();

        DoubleType lastSectionType = lastSection.Type;

        // when the last type is zero, assume that the line has crossed zero
        if (lastSection.Type == DoubleType::Zero && sections.size() >= 2)
        {
            DoubleType prevSectionType = sections[sections.size() - 2].Type;
            if (prevSectionType == DoubleType::Positive)
            {
                lastSectionType = DoubleType::Negative;
            }
            else if (prevSectionType == DoubleType::Negative)
            {
                lastSectionType = DoubleType::Positive;
            }
        }

        sections.emplace_back(lastSectionType, lastSection.UHigh, Statistics::StandardNormal::BetaMax);

        return sections;
    }

    double DirectionSectionsCalculation::findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
        const DirectionCalculation& directionCalculation,
        double uLow, double uHigh, double zLow, double zHigh, double& z)
    {
        return findBetaBetweenBoundariesAllowNaN(directionCalculation, uLow, uHigh, zLow, zHigh, z);
    }

    double DirectionSectionsCalculation::findBetaBetweenBoundariesAllowNaN(const DirectionCalculation& directionCalculation,
        double uLow, double uHigh, double zLow, double zHigh, double& z)
    {
        if (std::isnan(zLow) || std::isnan(zHigh))
        {
            double step = uHigh - uLow;
            bool lastAdaptedLow = false;

            while (step > Settings.EpsilonUStepSize)
            {
                step = step / 2;

                if (std::isnan(zLow))
                {
                    uLow += step;
                    zLow = directionCalculation.GetZ(uLow);
                    lastAdaptedLow = true;
                }
                else if (std::isnan(zHigh))
                {
                    uHigh -= step;
                    zHigh = directionCalculation.GetZ(uHigh);
                    lastAdaptedLow = false;
                }
                else if (lastAdaptedLow)
                {
                    uHigh = uLow;
                    zHigh = zLow;
                    uLow -= step;
                    zLow = directionCalculation.GetZ(uLow);
                }
                else
                {
                    uLow = uHigh;
                    zLow = zHigh;
                    uHigh += step;
                    zHigh = directionCalculation.GetZ(uHigh);
                }
            }

            if (std::isnan(zLow))
            {
                z = zHigh;
                return uHigh;
            }
            else if (std::isnan(zHigh))
            {
                z = zLow;
                return uLow;
            }
            else
            {
                double u = (uLow + uHigh) / 2;
                z = directionCalculation.GetZ(u);

                return u;
            }
        }
        else
        {
            double zTolerance = GetZTolerance(Settings, uLow, uHigh, zLow, zHigh);

            auto linearSearchCalculation = LinearRootFinder(zTolerance, Settings.MaximumIterations);

            auto low = XValue(uLow, zLow);
            auto high = XValue(uHigh, zHigh);
            auto resultRootFinder = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });

            // TODO: PROBL-42 remove linear search , because bisection is more robust
            if (std::isnan(resultRootFinder.X))
            {
                constexpr double xTolerance = 0.01;
                auto bisectionCalculation = BisectionRootFinder(zTolerance, xTolerance);
                resultRootFinder = bisectionCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });
            }

            z = std::isnan(resultRootFinder.X) ? nan("") : resultRootFinder.Value;

            return resultRootFinder.X;
        }
    }

    double DirectionSectionsCalculation::GetZTolerance(const DirectionReliabilitySettings& settings, double uLow, double uHigh, double zLow, double zHigh)
    {
        const double uDiff = std::abs((zHigh - zLow) / (uHigh - uLow)) * settings.EpsilonUStepSize;

        if (!std::isnan(uDiff))
        {
            return std::min(settings.EpsilonZStepSize, uDiff);
        }
        else
        {
            return settings.EpsilonZStepSize;
        }
    }

}

