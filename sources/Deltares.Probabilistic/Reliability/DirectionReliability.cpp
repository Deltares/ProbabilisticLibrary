// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#include "DirectionReliability.h"
#include "DirectionReliabilitySettings.h"
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

        class DirectionSection
        {
        public:
            DirectionSection(DoubleType type, double uLow, double uHigh)
            {
                this->Type = type;
                this->ULow = uLow;
                this->UHigh = uHigh;
            }

            DirectionSection(DoubleType type, double uLow, double uHigh, double zLow, double zHigh)
            {
                this->Type = type;
                this->ULow = uLow;
                this->UHigh = uHigh;
                this->ZLow = zLow;
                this->ZHigh = zHigh;
            }

            DoubleType Type;
            double ULow = 0.0;
            double UHigh = 0.0;
            double ZLow = 0.0;
            double ZHigh = 0.0;

            double getProbability()
            {
                double pHigh = Statistics::StandardNormal::getQFromU(UHigh);
                double pLow = Statistics::StandardNormal::getQFromU(ULow);
                return pLow - pHigh;
            }
        };

        class ZGetter
        {
        private:
            std::shared_ptr<Models::ModelRunner> modelRunner;
            std::shared_ptr<DirectionReliabilitySettings> settings;
        public:
            ZGetter(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings = nullptr)
            {
                this->modelRunner = modelRunner;
                this->settings = settings;
            }

            double GetZ(std::shared_ptr<Sample> uDirection, double factor, bool inverted, bool allowProxy = true)
            {
                std::shared_ptr<Sample> u = uDirection->getMultipliedSample(factor);
                u->AllowProxy = allowProxy;

                if (settings != nullptr && settings->UseInitialValues)
                {
                    for (size_t i = 0; i < u->Values.size(); i++)
                    {
                        if (!settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
                        {
                            u->Values[i] = settings->StochastSet->VaryingStochastSettings[i]->StartValue;
                        }
                    }
                }

                return GetZValueCorrected(u, inverted);
            }

            double GetZValueCorrected(std::shared_ptr<Sample> u, bool invertZ)
            {
                double z = modelRunner->getZValue(u);
                if (invertZ)
                {
                    z = -z;
                }
                return z;
            }
        };

        class DirectionCalculation
        {
        private:
            std::shared_ptr<Models::ModelRunner> modelRunner;
            std::shared_ptr<Sample> uDirection;
            bool inverted;
            ZGetter model;

        public:
            DirectionCalculation(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> uDirection, bool inverted)
                : model(ZGetter(modelRunner))
            {
                this->modelRunner = modelRunner;
                this->uDirection = uDirection;
                this->inverted = inverted;
            }

            double GetZProxy(double u, bool allowProxy)
            {
                return model.GetZ(uDirection, u, inverted, allowProxy);
            }

            double GetZ(double u)
            {
                return model.GetZ(uDirection, u, inverted, true);
            }

            double GetZNoProxy(double u)
            {
                return model.GetZ(uDirection, u, inverted, false);
            }
        };

        std::shared_ptr<DesignPoint> DirectionReliability::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner->getVaryingStochastCount());
            double z = modelRunner->getZValue(zeroSample);
            double z0 = this->getZFactor(z);

            std::shared_ptr<Sample> directionSample = this->Settings->StochastSet->getStartPoint();

            double beta = getBeta(modelRunner, directionSample, z0);

            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(directionSample, beta);

            return designPoint;
        }

        double DirectionReliability::getBeta(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<Sample> directionSample, double z0)
        {
            std::shared_ptr<Sample> normalizedSample = directionSample->getNormalizedSample();

            std::shared_ptr <BetaValueTask> task(new BetaValueTask());
            task->ModelRunner = modelRunner;
            task->Index = 0;
            task->Settings = this->Settings;
            task->UValues = normalizedSample;
            task->Iteration = 0;
            task->z0 = z0;

            double beta = this->getDirectionBeta(modelRunner, task);
            beta = beta * z0;

            directionSample->AllowProxy = task->UValues->AllowProxy;

            return beta;
        }

        double DirectionReliability::getDirectionBeta(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr <BetaValueTask> directionTask)
        {
            std::shared_ptr<Sample> uDirection = directionTask->UValues->getNormalizedSample();

            if (modelRunner->canCalculateBeta())
            {
                return modelRunner->getBeta(directionTask->UValues);
            }
            else
            {
                bool invertZ = directionTask->z0 < 0;

                std::vector<std::shared_ptr<DirectionSection>> sections = this->getDirectionSections(modelRunner, directionTask->Settings, uDirection, invertZ);

                double beta = getBetaFromSections(sections);

                directionTask->UValues->AllowProxy = uDirection->AllowProxy;

                return beta;
            }
        }

        std::vector<std::shared_ptr<DirectionSection>> DirectionReliability::getDirectionSections(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ)
        {
            std::vector<std::shared_ptr<DirectionSection>> sections;

            int sectionsCount = static_cast<int>(settings->MaximumLengthU / settings->Dsdu);
            if (sectionsCount * settings->Dsdu < settings->MaximumLengthU)
            {
                sectionsCount = sectionsCount + 1;
            }

            bool found = false;
            bool monotone = settings->modelVaryingType == ModelVaryingType::Monotone;
            std::unique_ptr<ZGetter> model(new ZGetter(modelRunner, settings));

            double prevzHigh = nan("");

            for (int k = 0; k <= sectionsCount && !this->isStopped(); k++)
            {
                if (!found)
                {
                    double uLow = k * settings->Dsdu;
                    double uHigh = std::min((k + 1) * settings->Dsdu, settings->MaximumLengthU);

                    double zLow = k > 0 ? prevzHigh : model->GetZ(uDirection, uLow, invertZ);
                    double zHigh = model->GetZ(uDirection, uHigh, invertZ);

                    prevzHigh = zHigh;

                    DoubleType zHighType = NumericSupport::getDoubleType(zHigh);
                    DoubleType zLowType = NumericSupport::getDoubleType(zLow);

                    if (zHighType != zLowType)
                    {
                        double zChange = nan("");
                        double uChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uHigh, zLow, zHigh, zChange);

                        DoubleType zChangeType = NumericSupport::getDoubleType(zChange);

                        if (zLowType == DoubleType::NaN && zChangeType != zHighType)
                        {
                            sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uChange));

                            double uSubChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uChange, uHigh, zChange, zHigh, zChange);

                            sections.push_back(std::make_shared<DirectionSection>(zChangeType, uChange, uSubChange));
                            sections.push_back(std::make_shared<DirectionSection>(zHighType, uSubChange, uHigh));
                        }
                        else if (zHighType == DoubleType::NaN && zChangeType != zLowType)
                        {
                            double uSubChange = findBetaBetweenBoundaries(modelRunner, settings, uDirection, invertZ, uLow, uChange, zLow, zChange, zChange);

                            sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uSubChange));
                            sections.push_back(std::make_shared<DirectionSection>(zChangeType, uSubChange, uChange));
                            sections.push_back(std::make_shared<DirectionSection>(zHighType, uChange, uHigh));
                        }
                        else
                        {
                            sections.push_back(std::make_shared<DirectionSection>(zLowType, uLow, uChange, zLow, zChange));
                            sections.push_back(std::make_shared<DirectionSection>(zHighType, uChange, uHigh, zHigh, zChange));
                        }

                        if (monotone)
                        {
                            found = true;
                        }
                    }
                    else
                    {
                        std::shared_ptr<DirectionSection> tempVar9 = std::make_shared<DirectionSection>(zHighType, uLow, uHigh, zLow, zHigh);
                        sections.push_back(tempVar9);

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
            std::shared_ptr<DirectionSection> lastSection = sections.back();

            DoubleType lastSectionType = lastSection->Type;

            // when the last type is zero, assume that the line has crossed zero
            if (lastSection->Type == DoubleType::Zero && sections.size() >= 2)
            {
                DoubleType prevSectionType = sections[sections.size() - 2]->Type;
                if (prevSectionType == DoubleType::Positive)
                {
                    lastSectionType = DoubleType::Negative;
                }
                else if (prevSectionType == DoubleType::Negative)
                {
                    lastSectionType = DoubleType::Positive;
                }
            }

            sections.push_back(std::make_shared<DirectionSection>(lastSectionType, lastSection->UHigh, Statistics::StandardNormal::BetaMax));

            return sections;
        }

        double DirectionReliability::findBetaBetweenBoundaries(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            return findBetaBetweenBoundariesAllowNaN(modelRunner, settings, uDirection, invertZ, uLow, uHigh, zLow, zHigh, z);
        }

        double DirectionReliability::findBetaBetweenBoundariesAllowNaN(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            std::unique_ptr<ZGetter> model(new ZGetter(modelRunner, settings));

            if (std::isnan(zLow) || std::isnan(zHigh))
            {
                double step = uHigh - uLow;
                bool lastAdaptedLow = false;

                while (step > settings->EpsilonUStepSize)
                {
                    step = step / 2;

                    if (std::isnan(zLow))
                    {
                        uLow += step;
                        zLow = model->GetZ(uDirection, uLow, invertZ);
                        lastAdaptedLow = true;
                    }
                    else if (std::isnan(zHigh))
                    {
                        uHigh -= step;
                        zHigh = model->GetZ(uDirection, uHigh, invertZ);
                        lastAdaptedLow = false;
                    }
                    else if (lastAdaptedLow)
                    {
                        uHigh = uLow;
                        zHigh = zLow;
                        uLow -= step;
                        zLow = model->GetZ(uDirection, uLow, invertZ);
                    }
                    else
                    {
                        uLow = uHigh;
                        zLow = zHigh;
                        uHigh += step;
                        zHigh = model->GetZ(uDirection, uHigh, invertZ);
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
                    z = model->GetZ(uDirection, u, invertZ);

                    return u;
                }
            }
            else
            {
                auto directionCalculation = std::make_shared<DirectionCalculation>(modelRunner, uDirection, invertZ);

                double zTolerance = GetZTolerance(settings, uLow, uHigh, zLow, zHigh);

                auto linearSearchCalculation = LinearRootFinder(zTolerance, settings->MaximumIterations);

                auto low = XValue(uLow, zLow);
                auto high = XValue(uHigh, zHigh);
                double uResult = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation->GetZ(v); });

                // TODO: PROBL-42 remove linear search , because bisection is more robust
                if (std::isnan(uResult))
                {
                    const double xTolerance = 0.01;
                    auto bisectionCalculation = BisectionRootFinder(zTolerance, xTolerance);
                    uResult = bisectionCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation->GetZ(v); });
                }

                z = std::isnan(uResult) ? nan("") : directionCalculation->GetZ(uResult);

                return uResult;
            }
        }

        double DirectionReliability::getBetaFromSections(std::vector<std::shared_ptr<DirectionSection>> sections)
        {
            // sum the probabilities
            double failingProbability = 0;
            double nonFailingProbability = 0.5; // start counting at u = 0

            for (int i = sections.size() - 1; i >= 0; i--)
            {
                switch (sections[i]->Type)
                {
                case DoubleType::Positive:
                    nonFailingProbability += sections[i]->getProbability();
                    break;
                case DoubleType::Negative:
                    failingProbability += sections[i]->getProbability();
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
                for (size_t i = 0; i < sections.size(); i++)
                {
                    if (sections[i]->ZHigh < zmin && sections[i]->ZHigh != 0.0)
                    {
                        rmin = sections[i]->UHigh;
                        zmin = sections[i]->ZHigh;
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


        double DirectionReliability::GetZTolerance(std::shared_ptr<DirectionReliabilitySettings> settings, double uLow, double uHigh, double zLow, double zHigh)
        {
            double uDiff = std::abs((zHigh - zLow) / (uHigh - uLow)) * settings->EpsilonUStepSize;

            if (!std::isnan(uDiff))
            {
                return std::min(settings->EpsilonZStepSize, uDiff);
            }
            else
            {
                return settings->EpsilonZStepSize;
            }
        }

        double DirectionReliabilityForDirectionalSampling::findBetaBetweenBoundaries(std::shared_ptr<Models::ModelRunner> modelRunner, std::shared_ptr<DirectionReliabilitySettings> settings, std::shared_ptr<Sample> uDirection, bool invertZ, double uLow, double uHigh, double zLow, double zHigh, double& z)
        {
            if (std::isnan(zLow) || std::isnan(zHigh))
            {
                return this->findBetaBetweenBoundariesAllowNaN(modelRunner, settings, uDirection, invertZ, uLow, uHigh, zLow, zHigh, z);
            }
            else
            {
                auto directionCalculation = std::make_shared<DirectionCalculation>(modelRunner, uDirection, invertZ);

                const double zTolerance = GetZTolerance(settings, uLow, uHigh, zLow, zHigh);

                auto linearSearchCalculation = LinearRootFinder(zTolerance, settings->MaximumIterations);

                auto low = XValue(uLow, zLow);
                auto high = XValue(uHigh, zHigh);
                double uResult = linearSearchCalculation.CalculateValue(low, high, 0.0, [directionCalculation](double v) { return directionCalculation->GetZ(v); });

                z = std::isnan(uResult) ? nan("") : directionCalculation->GetZ(uResult);

                if (modelRunner->Settings->IsProxyModel())
                {
                    if (std::isnan(uResult))
                    {
                        auto bisectionCalculation = BisectionRootFinder(zTolerance);
                        uResult = bisectionCalculation.CalculateValue(uLow, uHigh, 0.0, [directionCalculation](double v) { return directionCalculation->GetZ(v); });
                    }

                    if (modelRunner->Settings->proxySettings->ShouldUpdateFinalSteps && !isProxyAllowed(modelRunner, uResult, this->Threshold))
                    {
                        uDirection->AllowProxy = false;

                        double z0 = directionCalculation->GetZProxy(0, false);
                        double zResult = directionCalculation->GetZProxy(uResult, false);

                        if (std::isnan(zResult))
                        {
                            z = zResult;
                            modelRunner->removeTask(uDirection->IterationIndex);
                            return settings->MaximumLengthU;
                        }
                        else if (NumericSupport::GetSign(z0) == NumericSupport::GetSign(zResult) && std::fabs(zResult) >= std::fabs(z0))
                        {
                            z = zResult;
                            modelRunner->removeTask(uDirection->IterationIndex);
                            return settings->MaximumLengthU;
                        }
                        else
                        {
                            double uNew = NumericSupport::interpolate(0, z0, 0, zResult, uResult, true);
                            if (isProxyAllowed(modelRunner, uNew, this->Threshold))
                            {
                                z = zResult;
                                return std::min(uNew, settings->MaximumLengthU);
                            }
                        }

                        auto lowProxy = XValue(0.0, z0);
                        auto highProxy = XValue(uResult, zResult);
                        uResult = linearSearchCalculation.CalculateValue(lowProxy, highProxy, 0.0, [directionCalculation](double v) { return directionCalculation->GetZNoProxy(v); });
                        if (std::isnan(uResult))
                        {
                            z = zResult;
                            uResult = settings->MaximumLengthU;
                        }
                        else
                        {
                            z = directionCalculation->GetZProxy(uResult, false);
                        }
                    }
                }

                return uResult;
            }
        };

        bool DirectionReliabilityForDirectionalSampling::isProxyAllowed(std::shared_ptr<ModelRunner> modelRunner, double u, double threshold)
        {
            return std::isnan(u) || u > threshold + modelRunner->Settings->proxySettings->ThresholdOffset;
        }
    }
}


