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
#include "DirectionReliability.h"
#include "DirectionReliabilitySettings.h"
#include "DirectionCalculation.h"
#include "../Model/ModelRunner.h"
#include "../Math/NumericSupport.h"
#include "../Math/RootFinders/LinearRootFinder.h"
#include "../Math/RootFinders/BisectionRootFinder.h"
#include <memory>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> DirectionReliability::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
            double z = modelRunner->getZValue(zeroSample);
            double z0 = getZFactor(z);

            std::shared_ptr<Sample> directionSample = this->Settings->StochastSet->getStartPoint();

            double beta = getBeta(*modelRunner, directionSample, z0);

            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(directionSample, beta);

            return designPoint;
        }

        double DirectionReliability::getBeta(Models::ModelRunner& modelRunner, std::shared_ptr<Sample> directionSample, double z0)
        {
            auto zValues = PrecomputeValues();
            return getBeta(modelRunner, directionSample, z0, zValues);
        }

        double DirectionReliability::getBeta(Models::ModelRunner& modelRunner, std::shared_ptr<Sample> directionSample,
            double z0, const PrecomputeValues& zValues)
        {
            auto normalizedSample = directionSample->getNormalizedSample();

            auto task = BetaValueTask();
            task.Settings = Settings;
            task.UValues = normalizedSample;
            task.z0 = z0;

            double beta = getDirectionBeta(modelRunner, task, zValues);
            beta *= z0;

            directionSample->AllowProxy = task.UValues->AllowProxy;

            return beta;
        }

        double DirectionReliability::getDirectionBeta(Models::ModelRunner& modelRunner,
            const BetaValueTask& directionTask, const PrecomputeValues& zValues)
        {
            std::shared_ptr<Sample> uDirection = directionTask.UValues->getNormalizedSample();

            if (modelRunner.canCalculateBeta())
            {
                return modelRunner.getBeta(directionTask.UValues);
            }
            else
            {
                bool invertZ = directionTask.z0 < 0.0;

                auto sections = getDirectionSections(modelRunner, directionTask.Settings,
                    uDirection, invertZ, zValues);

                double beta = getBetaFromSections(sections);

                directionTask.UValues->AllowProxy = uDirection->AllowProxy;

                return beta;
            }
        }

        std::vector<DirectionSection> DirectionReliability::getDirectionSections(Models::ModelRunner& modelRunner,
            std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, const PrecomputeValues& zValues)
        {
            std::vector<DirectionSection> sections;

            int sectionsCount = static_cast<int>(settings->MaximumLengthU / settings->Dsdu);
            if (sectionsCount * settings->Dsdu < settings->MaximumLengthU)
            {
                sectionsCount = sectionsCount + 1;
            }

            bool found = false;
            bool monotone = settings->modelVaryingType == ModelVaryingType::Monotone;
            auto dirCalcSettings = DirectionCalculationSettings(invertZ, settings->Dsdu, settings->MaximumLengthU);
            auto directionCalculation = DirectionCalculation(modelRunner, uDirection, dirCalcSettings);
            double prevzHigh = directionCalculation.GetZ(0, zValues);

            for (int k = 0; k <= sectionsCount && !this->isStopped(); k++)
            {
                if (!found)
                {
                    double uLow = k * settings->Dsdu;
                    double uHigh = std::min((k + 1) * settings->Dsdu, settings->MaximumLengthU);

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

        double DirectionReliability::findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
            const DirectionCalculation& directionCalculation,
            double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            return findBetaBetweenBoundariesAllowNaN(directionCalculation, uLow, uHigh, zLow, zHigh, z);
        }

        double DirectionReliability::findBetaBetweenBoundariesAllowNaN(const DirectionCalculation& directionCalculation,
            double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            if (std::isnan(zLow) || std::isnan(zHigh))
            {
                double step = uHigh - uLow;
                bool lastAdaptedLow = false;

                while (step > Settings->EpsilonUStepSize)
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
                double zTolerance = GetZTolerance(*Settings, uLow, uHigh, zLow, zHigh);

                auto linearSearchCalculation = LinearRootFinder(zTolerance, Settings->MaximumIterations);

                auto low = XValue(uLow, zLow);
                auto high = XValue(uHigh, zHigh);
                double uResult = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });

                // TODO: PROBL-42 remove linear search , because bisection is more robust
                if (std::isnan(uResult))
                {
                    constexpr double xTolerance = 0.01;
                    auto bisectionCalculation = BisectionRootFinder(zTolerance, xTolerance);
                    uResult = bisectionCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });
                }

                z = std::isnan(uResult) ? nan("") : directionCalculation.GetZ(uResult);

                return uResult;
            }
        }

        double DirectionReliability::getBetaFromSections(const std::vector<DirectionSection>& sections) const
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
            else if (nonFailingProbability == 0.5 && this->Settings->FindMinimalValue)
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


        double DirectionReliability::GetZTolerance(const DirectionReliabilitySettings& settings, double uLow, double uHigh, double zLow, double zHigh)
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

        double DirectionReliabilityForDirectionalSampling::findBetaBetweenBoundaries(Models::ModelRunner& modelRunner,
            const DirectionCalculation& directionCalculation,
            double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            if (std::isnan(zLow) || std::isnan(zHigh))
            {
                return findBetaBetweenBoundariesAllowNaN(directionCalculation, uLow, uHigh, zLow, zHigh, z);
            }
            else
            {
                const double zTolerance = GetZTolerance(*Settings, uLow, uHigh, zLow, zHigh);

                auto linearSearchCalculation = LinearRootFinder(zTolerance, Settings->MaximumIterations);

                auto low = XValue(uLow, zLow);
                auto high = XValue(uHigh, zHigh);
                double uResult = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });

                z = std::isnan(uResult) ? nan("") : directionCalculation.GetZ(uResult);

                if (modelRunner.Settings->IsProxyModel())
                {
                    if (std::isnan(uResult))
                    {
                        auto bisectionCalculation = BisectionRootFinder(zTolerance);
                        uResult = bisectionCalculation.CalculateValue(uLow, uHigh, 0.0, [directionCalculation](double v) { return directionCalculation.GetZ(v); });
                    }

                    const auto ThresholdOffset = modelRunner.Settings->proxySettings->ThresholdOffset;
                    if (modelRunner.Settings->proxySettings->ShouldUpdateFinalSteps && !isProxyAllowed(ThresholdOffset, uResult, this->Threshold))
                    {
                        directionCalculation.uDirection->AllowProxy = false;

                        double z0 = directionCalculation.GetZProxy(0, false);
                        double zResult = directionCalculation.GetZProxy(uResult, false);

                        if (std::isnan(zResult))
                        {
                            z = zResult;
                            modelRunner.removeTask(directionCalculation.uDirection->IterationIndex);
                            return Settings->MaximumLengthU;
                        }
                        else if (NumericSupport::GetSign(z0) == NumericSupport::GetSign(zResult) && std::fabs(zResult) >= std::fabs(z0))
                        {
                            z = zResult;
                            modelRunner.removeTask(directionCalculation.uDirection->IterationIndex);
                            return Settings->MaximumLengthU;
                        }
                        else
                        {
                            double uNew = NumericSupport::interpolate(0, z0, 0, zResult, uResult, true);
                            if (isProxyAllowed(ThresholdOffset, uNew, Threshold))
                            {
                                z = zResult;
                                return std::min(uNew, Settings->MaximumLengthU);
                            }
                        }

                        auto lowProxy = XValue(0.0, z0);
                        auto highProxy = XValue(uResult, zResult);
                        uResult = linearSearchCalculation.CalculateValue(lowProxy, highProxy, 0.0, [directionCalculation](double v) { return directionCalculation.GetZNoProxy(v); });
                        if (std::isnan(uResult))
                        {
                            z = zResult;
                            uResult = Settings->MaximumLengthU;
                        }
                        else
                        {
                            z = directionCalculation.GetZProxy(uResult, false);
                        }
                    }
                }

                return uResult;
            }
        };

        bool DirectionReliabilityForDirectionalSampling::isProxyAllowed(double ThresholdOffset, double u, double threshold)
        {
            return std::isnan(u) || u > threshold + ThresholdOffset;
        }
    }
}


