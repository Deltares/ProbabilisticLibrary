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
#include <cmath>
#include "FragilityCurveIntegration.h"

#include "DesignPoint.h"
#include "../Model/GradientCalculator.h"
#include "../Math/NumericSupport.h"

#if __has_include(<format>)
#include <format>
#else
#include "../Utils/probLibString.h"
#endif


namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> FragilityCurveIntegration::getDesignPoint(std::shared_ptr<Statistics::Stochast> parameter, std::shared_ptr<Statistics::Stochast> fragilityCurve, std::shared_ptr<Statistics::Stochast> fragilityCurveNormalized)
        {
            double probFailure = 0;
            double probFailureExcluded = 0;

            if (parameter == nullptr)
            {
                parameter = std::make_shared<Statistics::Stochast>();
                parameter->setDistributionType(Statistics::DistributionType::Normal);
                parameter->setMeanAndDeviation(1, 1);
            }

            std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = std::vector{ parameter, fragilityCurve };
            std::shared_ptr<DesignPointBuilder> designPointBuilder = std::make_shared<DesignPointBuilder>(Settings->designPointMethod, stochasts);
            designPointBuilder->initialize(Statistics::StandardNormal::BetaMax);

            // Perform numerical integration over the fragility curve stochast

            std::vector<std::shared_ptr<UStep>> steps = getSteps(parameter, Settings->StepSize);

            int count = 0;

            for (std::shared_ptr<UStep> step : steps)
            {
                // Do one step in the numerical integration

                double x = parameter->getXFromU(step->U);

                double uFrag = fragilityCurve->getUFromX(x);
                double prob = Statistics::StandardNormal::getQFromU(uFrag);

                if (fragilityCurveNormalized != nullptr)
                {
                    double uFragNormalized = fragilityCurveNormalized->getUFromX(x);
                    double probNormalized = Statistics::StandardNormal::getQFromU(uFragNormalized);

                    prob = std::max(0.0, prob - (1 - probNormalized));
                    uFrag = Statistics::StandardNormal::getUFromQ(prob);

                    double additionExcluded = step->Weight * probNormalized;
                    probFailureExcluded += additionExcluded;
                }

                // Multiply this probability with the probability of the fragility curve stochast value
                double addition = step->Weight * prob;

                std::shared_ptr<Sample> sample = std::make_shared<Sample>(std::vector<double> { step->U, uFrag });

                double beta = std::sqrt(step->U * step->U + uFrag * uFrag);
                sample->Weight = Statistics::StandardNormal::getQFromU(beta);

                designPointBuilder->addSample(sample);

                probFailure += addition;

                count++;
            }

            if (fragilityCurveNormalized != nullptr)
            {
                probFailure /= probFailureExcluded;
            }

            std::shared_ptr<DesignPoint> designPoint = std::make_shared<DesignPoint>();
            designPoint->Beta = Statistics::StandardNormal::getUFromQ(probFailure);

            std::shared_ptr<Sample> designPointSample = designPointBuilder->getSample();

            // Set the contribution of the conditional stochast
            double alphaParameter = -designPointSample->Values[0] / designPoint->Beta; // u = - beta * alpha
            std::shared_ptr<StochastPointAlpha> alpha = std::make_shared<StochastPointAlpha>();
            alpha->Stochast = parameter;
            alpha->Alpha = alphaParameter;
            alpha->AlphaCorrelated = alphaParameter;
            alpha->U = designPointSample->Values[0];
            alpha->X = parameter->getXFromU(alpha->U);
            designPoint->Alphas.push_back(alpha);

            // Set the contribution of the fragility curve(s)
            for (std::shared_ptr<Statistics::Stochast> fCurve : { fragilityCurve, fragilityCurveNormalized })
            {
                if (fCurve != nullptr)
                {
                    std::shared_ptr<StochastPointAlpha> alphaCurve = std::make_shared<StochastPointAlpha>();
                    alphaCurve->Stochast = fCurve;
                    alphaCurve->U = designPointSample->Values[1];
                    alphaCurve->X = fCurve->getXFromU(alphaCurve->U);
                    alphaCurve->Alpha = -alphaCurve->U / designPoint->Beta;
                    alphaCurve->AlphaCorrelated = alphaCurve->Alpha;
                    designPoint->Alphas.push_back(alphaCurve);
                }
            }

            designPoint->correctFragilityCurves();
            designPoint->expandContributions();

            designPoint->convergenceReport->Convergence = 0;
            designPoint->convergenceReport->IsConverged = true;
            designPoint->convergenceReport->TotalModelRuns = count;

            return designPoint;
        }

        std::shared_ptr<DesignPoint> FragilityCurveIntegration::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            double stepSize = Settings->StepSize;
            double uMin = - Statistics::StandardNormal::UMax - stepSize / 2;
            double uMax = Statistics::StandardNormal::UMax + stepSize / 2;

            double probFailure = 0;

            double u = uMin;

            double cdfLow = Statistics::StandardNormal::getPFromU(u);

            std::shared_ptr<DesignPointBuilder> designPointBuilder = std::make_shared<DesignPointBuilder>(2, Settings->designPointMethod, this->Settings->StochastSet);
            designPointBuilder->initialize(Statistics::StandardNormal::BetaMax);

            // Perform numerical integration over the fragility curve stochast

            int count = 0;

            while (u < uMax)
            {
                // Do one step in the numerical integration

                double cdfHigh = Statistics::StandardNormal::getPFromU(u + stepSize);

                double pdf = cdfHigh - cdfLow;
                double parU = u + stepSize / 2;

                std::shared_ptr<Sample> sample = std::make_shared<Sample>(std::vector<double>{ parU, 0 });

                double prob = modelRunner->getZValue(sample);
                double uFrag = Statistics::StandardNormal::getUFromQ(prob);

                // Multiply this probability with the probability of the fragility curve stochast value
                double addition = pdf * prob;

                std::shared_ptr<Sample> fragilitySample = std::make_shared<Sample>(std::vector<double>{ parU, uFrag });

                double betaSample = std::sqrt(parU * parU + uFrag * uFrag);
                fragilitySample->Weight = Statistics::StandardNormal::getQFromU(betaSample);

                designPointBuilder->addSample(fragilitySample);

                probFailure += addition;

                u += stepSize;
                cdfLow = cdfHigh;
                
                count++;
            }

            std::shared_ptr<Sample> designPointSample = designPointBuilder->getSample();

            double beta = Statistics::StandardNormal::getUFromQ(probFailure);

            if (modelRunner->getVaryingStochastCount() == 1)
            {
                double alphaFragilityCurve = - designPointSample->Values[1] / beta; // u = - beta * alpha
                std::vector<double> alphas = std::vector{ alphaFragilityCurve };
                designPointSample = std::make_shared<Sample>(alphas);
            }

            std::shared_ptr<DesignPoint> designPoint = modelRunner->getDesignPoint(designPointSample, beta, std::make_shared<ConvergenceReport>());

            designPoint->correctFragilityCurves();
            designPoint->expandContributions();

            designPoint->convergenceReport->Convergence = 0;
            designPoint->convergenceReport->IsConverged = true;
            designPoint->convergenceReport->TotalModelRuns = count;

            return designPoint;
        }

        std::vector<std::shared_ptr<FragilityCurveIntegration::UStep>> FragilityCurveIntegration::getSteps(std::shared_ptr<Statistics::Stochast> stochast, double stepSize)
        {
            std::vector<std::shared_ptr<UStep>> steps;

            if (stochast->getDistributionType() == Statistics::DistributionType::Deterministic)
            {
                steps.push_back(std::make_shared<UStep>(0, 1));
            }
            else if (stochast->getDistributionType() == Statistics::DistributionType::Discrete || stochast->getDistributionType() == Statistics::DistributionType::Qualitative)
            {
                stochast->initializeForRun();
                for (std::shared_ptr<Statistics::DiscreteValue> discreteValue : stochast->getProperties()->DiscreteValues)
                {
                    steps.push_back(std::make_shared<UStep>(stochast->getUFromX(discreteValue->X), discreteValue->NormalizedAmount));
                }
            }
            else if (stochast->getDistributionType() == Statistics::DistributionType::Table)
            {
                stochast->initializeForRun();
                for (std::shared_ptr<Statistics::HistogramValue> histogramValue : stochast->getProperties()->HistogramValues)
                {
                    steps.push_back(std::make_shared<UStep>(stochast->getUFromX(histogramValue->getCenter()), histogramValue->NormalizedAmount));
                }
            }
            else
            {
                double uMin = -Statistics::StandardNormal::UMax - stepSize / 2;
                double uMax = Statistics::StandardNormal::UMax + stepSize / 2;

                double u = uMin;

                double cdfLow = Statistics::StandardNormal::getPFromU(u);

                while (u < uMax)
                {
                    double parU = u + stepSize / 2;
                    double cdfHigh = Statistics::StandardNormal::getPFromU(u + stepSize);
                    double weight = cdfHigh - cdfLow;

                    steps.push_back(std::make_shared<UStep>(parU, weight));

                    u += stepSize;
                    cdfLow = cdfHigh;
                }
            }

            return steps;
        }
    }
}


