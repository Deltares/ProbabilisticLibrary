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
#include "StartPointCalculator.h"

#include "DirectionReliability.h"
#include "../Utils/probLibException.h"
#include "../Model/GradientCalculator.h"
#include "../Math/NumericSupport.h"
#include "SphereTasks.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<Sample> StartPointCalculator::getStartPoint(Models::ModelRunner& modelRunner) const
        {
            switch (this->Settings->StartMethod)
            {
            case StartMethodType::FixedValue:
                return getNoneStartPoint();
            case StartMethodType::RaySearch:
                return getRayStartPoint(modelRunner);
            case StartMethodType::SensitivitySearch:
                return getSensitivityStartPoint(modelRunner);
            case StartMethodType::SphereSearch:
                return getSphereStartPoint(modelRunner);
            default:
                throw probLibException("Start method not supported: ", (int)this->Settings->StartMethod);
            }
        }

        std::shared_ptr<Sample> StartPointCalculator::getNoneStartPoint() const
        {
            std::shared_ptr<Sample> startPoint = Settings->StochastSet->getStartPoint();
            if ( ! Settings->startVector.empty())
            {
                for (int i = 0; i < startPoint->getSize(); i++)
                {
                    startPoint->Values[i] = Settings->startVector[i];
                }
            }
            return startPoint;
        }

        void StartPointCalculator::correctDefaultValues(Sample& startPoint) const
        {
            bool isDefaultStartValues = true;
            for (int i = 0; i < startPoint.getSize(); i++)
            {
                if (this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
                {
                    isDefaultStartValues &= startPoint.Values[i] == 0.0;
                }
            }

            if (isDefaultStartValues)
            {
                for (int i = 0; i < startPoint.getSize(); i++)
                {
                    startPoint.Values[i] = 1;
                }
            }
        }

        std::shared_ptr<Sample> StartPointCalculator::getRayStartPoint(Models::ModelRunner& modelRunner) const
        {
            std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getStartPoint();

            if ( ! Settings->startVector.empty())
            {
                for (int i = 0; i < startPoint->getSize(); i++)
                {
                    startPoint->Values[i] = this->Settings->startVector[i];
                }
            }
            else
            {
                correctDefaultValues(*startPoint);
            }

            std::shared_ptr<Sample> rayStartPoint = getDirectionStartPoint(modelRunner, *startPoint);

            return rayStartPoint;
        }

        std::shared_ptr<Sample> StartPointCalculator::getDirectionStartPoint(Models::ModelRunner& modelRunner, Sample& startPoint) const
        {
            int nStochasts = modelRunner.getVaryingStochastCount();

            for (int i = 0; i < nStochasts; i++)
            {
                if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed || 
                    this->Settings->StochastSet->VaryingStochastSettings[i]->IsQualitative)
                {
                    startPoint.Values[i] = 0;
                }
            }

            auto directionReliability = DirectionReliability();
            directionReliability.Settings->StochastSet = this->Settings->StochastSet;
            directionReliability.Settings->MaximumLengthU = this->Settings->MaximumLengthStartPoint;
            directionReliability.Settings->StochastSet = this->Settings->StochastSet;
            directionReliability.Settings->FindMinimalValue = true;
            directionReliability.Settings->UseInitialValues = true;
            directionReliability.Settings->modelVaryingType = ModelVaryingType::Varying;
            directionReliability.Settings->Dsdu = this->Settings->dsdu;

            double beta = directionReliability.getBeta(modelRunner, startPoint, 1);

            std::shared_ptr<Sample> directionPoint = std::make_shared<Sample>(startPoint.Values);

            directionPoint = directionPoint->getSampleAtBeta(std::min(beta, this->Settings->MaximumLengthStartPoint));

            for (int i = 0; i < nStochasts; i++)
            {
                if (!this->Settings->StochastSet->VaryingStochastSettings[i]->IsInitializationAllowed)
                {
                    directionPoint->Values[i] = this->Settings->StochastSet->VaryingStochastSettings[i]->StartValue;
                }
            }

            return directionPoint;
        }

        std::shared_ptr<Sample> StartPointCalculator::getSensitivityStartPoint(Models::ModelRunner& modelRunner) const
        {
            int nStochasts = modelRunner.getVaryingStochastCount();
            std::shared_ptr<Sample> startPoint = std::make_shared<Sample>(nStochasts);

            std::vector<double> gradient = getGradient(modelRunner, startPoint);

            auto gradientSample = Sample(gradient);

            std::shared_ptr<Sample> sensitivityStartPoint = getDirectionStartPoint(modelRunner, gradientSample);

            return sensitivityStartPoint;
        }

        std::vector<double> StartPointCalculator::getGradient(Models::ModelRunner& modelRunner, const std::shared_ptr<Sample>& sample)
        {
            int nstochasts = modelRunner.getVaryingStochastCount();

            std::vector<std::shared_ptr<Sample>> samples;
            std::vector<double> gradient(nstochasts);

            double stepSize = 2;

            // first sample is the sample itself
            samples.push_back(sample);

            for (int k = 0; k < nstochasts; k++)
            {
                std::shared_ptr<Sample> u1 = sample->clone();
                u1->Values[k] -= stepSize;
                samples.push_back(u1);

                std::shared_ptr<Sample> u2 = sample->clone();
                u2->Values[k] += stepSize;
                samples.push_back(u2);
            }

            std::vector<double> zValues = modelRunner.getZValues(samples);

            double z0Fac = Numeric::NumericSupport::GetSign(zValues[0]);
            double z0 = zValues[0] * z0Fac;

            for (int k = 0; k < nstochasts; k++)
            {
                const double zLow = zValues[2 * k + 1] * z0Fac;
                const double zHigh = zValues[2 * k + 2] * z0Fac;

                if (zLow < z0)
                {
                    gradient[k] = 1.0 / Numeric::NumericSupport::interpolate(0, z0, 0, zLow, -stepSize, true);
                }
                else if (zHigh < z0)
                {
                    gradient[k] = 1.0 / Numeric::NumericSupport::interpolate(0, z0, 0, zHigh, stepSize, true);
                }
                else
                {
                    gradient[k] = 0;
                }
            }

            return gradient;
        }

        std::shared_ptr<Sample> StartPointCalculator::getSphereStartPoint(Models::ModelRunner& modelRunner) const
        {
            constexpr int nRadiusFactors = 20;
            auto maxSteps = Settings->maxStepsSphereSearch;

            std::shared_ptr<Sample> zeroSample = std::make_shared<Sample>(modelRunner.getVaryingStochastCount());
            double z0 = modelRunner.getZValue(zeroSample);

            double z0Fac = z0 < 0 ? -1 : 1;

            std::shared_ptr<Sample> startPoint = this->Settings->StochastSet->getStartPoint();

            correctDefaultValues(*startPoint);

            double radiusFactor = this->Settings->RadiusSphereSearch / startPoint->getBeta();

            std::shared_ptr<Sample> uSphere = startPoint->getMultipliedSample(radiusFactor);

            if ( ! Settings->startVector.empty())
            {
                for (size_t i = 0; i < this->Settings->startVector.size(); i++)
                {
                    uSphere->Values[i] = this->Settings->startVector[i];
                }
            }

            auto st = sphereTasks(maxSteps, Settings->allQuadrants);
            auto uSphereValues = Numeric::vector1D(uSphere->Values.size());
            for (size_t i = 0; i < uSphere->Values.size(); i++)
            {
                uSphereValues(i) = uSphere->Values[i];
            }
            auto tasks = st.examineSurfaceForTasks(uSphereValues);

            std::shared_ptr<Sample> bestSample = nullptr;
            std::vector<std::shared_ptr<Sample>> previousSamples;

            for (int i = 0; i < nRadiusFactors; i++)
            {
                radiusFactor = Numeric::NumericSupport::Divide(i + 1, nRadiusFactors);

                std::vector<std::shared_ptr<Sample>> samples;
                for (const auto& task : tasks)
                {
                    std::shared_ptr<Sample> uRay = this->Settings->StochastSet->getStartPoint();
                    for (size_t k = 0; k < uSphere->Values.size(); k++)
                    {
                        uRay->Values[k] = task(k);
                    }
                    std::shared_ptr<Sample> u = uRay->getMultipliedSample(radiusFactor);
                    u->IterationIndex = i;
                    samples.push_back(u);
                }
                auto zValues = modelRunner.getZValues(samples);
                for (auto & z : zValues) {z *= z0Fac;}
                auto indexMinimal = Numeric::NumericSupport::getLocationMinimum(zValues);
                if (zValues[indexMinimal] < 0.0)
                {
                    auto previous = (i > 0 ? previousSamples[indexMinimal] : zeroSample);
                    bestSample = refineSpherePoint(*samples[indexMinimal], *previous);
                    break;
                }

                previousSamples = samples;

                for (auto& z : zValues) { z = std::fabs(z); }
                auto indexAbsMinimal = Numeric::NumericSupport::getLocationMinimum(zValues);
                setBestSample(bestSample, samples[indexAbsMinimal]);
            }

            return bestSample;
        }

        // Sets the best sample based on closest to Z == 0
        void StartPointCalculator::setBestSample(std::shared_ptr<Sample> & bestSample, const std::shared_ptr<Sample> sample)
        {
            if (bestSample == nullptr)
            {
                bestSample = sample;
            }
            else if (std::abs(bestSample->Z) > std::abs(sample->Z))
            {
                bestSample = sample;
            }
        }

        std::shared_ptr<Sample> StartPointCalculator::refineSpherePoint( Sample& u, Sample& previous)
        {
            // determine the u-vector for which the z-function is 0.0, assuming linear behaviour between the samples u and previous.

            auto betaZeqZero = Numeric::NumericSupport::interpolate(0.0, previous.Z, previous.getBeta(), u.Z, u.getBeta());

            std::shared_ptr<Sample> u3 = u.getSampleAtBeta(betaZeqZero);
            return u3;
        }
    }
}


