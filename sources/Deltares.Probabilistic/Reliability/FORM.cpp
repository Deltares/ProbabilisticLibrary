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
#include "FORM.h"
#include "StartPointCalculator.h"
#include "DesignPoint.h"
#include "../Model/GradientCalculator.h"
#include "../Math/NumericSupport.h"

#include <tuple>
#include <format>

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> FORM::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            std::vector<std::shared_ptr<DesignPoint>> previousDesignPoints;

            // initialize
            auto startPointCalculator = StartPointCalculator();
            startPointCalculator.Settings = this->Settings->StartPointSettings;
            startPointCalculator.Settings->StochastSet = this->Settings->StochastSet;

            const std::shared_ptr<Models::Sample> startPoint = startPointCalculator.getStartPoint(*modelRunner);

            if (Settings->StartPointSettings->StartMethod != StartMethodType::FixedValue)
            {
                const std::shared_ptr<DesignPoint> startDesignPoint =
                    modelRunner->getDesignPoint(startPoint, startPoint->getBeta(), std::make_shared<ConvergenceReport>());
                startDesignPoint->Identifier = "Start point";
                previousDesignPoints.push_back(startDesignPoint);
            }

            std::shared_ptr<DesignPoint> designPoint = nullptr;

            double relaxationFactor = this->Settings->RelaxationFactor;

            // perform iterations
            for (int relaxationIndex = 0; relaxationIndex < Settings->RelaxationLoops; relaxationIndex++)
            {
                modelRunner->clearLists();

                designPoint = getDesignPoint(modelRunner, startPoint->clone(), relaxationFactor, relaxationIndex);

                if (designPoint->convergenceReport->IsConverged)
                {
                    break;
                }
                else
                {
                    relaxationFactor /= 2;
                    Settings->MaximumIterations *= Settings->MaxIterationsGrowthFactor;

                    int modifiedRelaxationIndex = relaxationIndex + 1;

                    if (modifiedRelaxationIndex < Settings->RelaxationLoops)
                    {
                        designPoint->Identifier = std::format("Relaxation loop {0:}", modifiedRelaxationIndex);
                        previousDesignPoints.push_back(designPoint);
                    }
                }
            }

            for (const std::shared_ptr<DesignPoint>& previousDesignPoint : previousDesignPoints)
            {
                designPoint->ContributingDesignPoints.push_back(previousDesignPoint);
            }

            return designPoint;
        }

        std::shared_ptr<DesignPoint> FORM::getDesignPoint(const std::shared_ptr<Models::ModelRunner>& modelRunner, std::shared_ptr<Models::Sample> startPoint,
            const double relaxationFactor, const int relaxationIndex)
        {
            constexpr double minGradientLength = 1E-08;

            const int nStochasts = modelRunner->getVaryingStochastCount();

            std::vector<double> zGradient(nStochasts);

            // initialization
            const std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            bool isLastRelaxation = (relaxationIndex + 1 == Settings->RelaxationLoops);
            if (this->Settings->RelaxationLoops > 1)
            {
                convergenceReport->RelaxationFactor = relaxationFactor;
            }

            int iteration = 0;
            double beta = nan("");

            std::shared_ptr<Models::Sample> sample = startPoint->clone();
            std::shared_ptr<Models::Sample> resultSample = startPoint->clone();

            auto gradientCalculator = Models::GradientCalculator();
            gradientCalculator.Settings = this->Settings->GradientSettings;

            auto lastBetas = std::vector<double>();
            auto lastSamples = std::vector<std::shared_ptr<Models::Sample>>();

            while (!convergenceReport->IsConverged && iteration < this->Settings->MaximumIterations && !this->isStopped())
            {
                sample->IterationIndex = iteration;
                zGradient = gradientCalculator.getGradient(*modelRunner, sample);

                // check whether there are valid results

                if (!areAllResultsValid(zGradient))
                {
                    // return the result so far
                    modelRunner->reportMessage(Logging::MessageType::Error,
                        std::format("Model did not provide valid results, limit state value = {0:.5G}", sample->Z));

                    std::shared_ptr<ReliabilityReport> reportInvalid = getReport(iteration, nan(""));

                    modelRunner->reportResult(reportInvalid);

                    return modelRunner->getDesignPoint(sample, nan(""), convergenceReport);
                }

                // Mean value Z at u = 0
                double zSum = 0;
                for (int i = 0; i < nStochasts; i++)
                {
                    zSum += sample->Values[i] * zGradient[i];
                }

                const double z0 = sample->Z - zSum;

                // Standard deviation Z 
                double zGradientLength = NumericSupport::GetLength(zGradient);

                if (zGradientLength < minGradientLength && iteration == 0)
                {
                    modelRunner->reportMessage(Logging::MessageType::Error, "No variation in model results found at start point");

                    const std::shared_ptr<ReliabilityReport> reportTooSmall = getReport(iteration, beta);
                    modelRunner->reportResult(reportTooSmall);

                    const double betaNoVariation = ReliabilityMethod::getZFactor(sample->Z) * Statistics::StandardNormal::BetaMax;
                    return modelRunner->getDesignPoint(sample, betaNoVariation, convergenceReport);
                }

                //   compute beta
                if (zGradientLength == 0)
                {
                    beta = ReliabilityMethod::getZFactor(z0) * Statistics::StandardNormal::BetaMax;
                }
                else
                {
                    beta = z0 / zGradientLength;
                }

                if (isLastRelaxation && Settings->FilterAtNonConvergence)
                {
                    if (iteration < histU)
                    {
                        lastSamples.push_back(sample);
                        lastBetas.push_back(beta);
                    }
                    else
                    {
                        int j = iteration % histU;
                        lastSamples[j] = sample;
                        lastBetas[j] = beta;
                    }
                }

                if (std::fabs(beta) >= Statistics::StandardNormal::BetaMax)
                {
                    modelRunner->reportMessage(Logging::MessageType::Error, "No convergence found");

                    const std::shared_ptr<ReliabilityReport> reportTooHigh = std::make_shared<ReliabilityReport>();

                    reportTooHigh->Step = iteration;
                    reportTooHigh->MaxSteps = this->Settings->MaximumIterations;
                    reportTooHigh->Reliability = beta;
                    reportTooHigh->ReportMatchesEvaluation = false;

                    modelRunner->reportResult(reportTooHigh);

                    return modelRunner->getDesignPoint(sample, beta, convergenceReport);
                }

                convergenceReport->IsConverged = isConverged(*modelRunner, *sample, *convergenceReport, beta, zGradientLength);
                convergenceReport->ZMargin = zGradientLength * this->Settings->EpsilonBeta;
                convergenceReport->TotalIterations = iteration + 1;

                // no convergence, next iteration
                if (!convergenceReport->IsConverged)
                {
                    const std::shared_ptr<Models::Sample> newSample = std::make_shared<Models::Sample>(nStochasts);

                    for (int k = 0; k < nStochasts; k++)
                    {
                        const double alpha = zGradient[k] / zGradientLength;
                        const double uNewValue = -alpha * beta;

                        newSample->Values[k] = relaxationFactor * uNewValue + (1 - relaxationFactor) * sample->Values[k];
                    }

                    sample = newSample;
                }

                double betaFac = getZFactor(beta);

                // compute design values
                std::vector<double> uValues = NumericSupport::select(zGradient, [betaFac, zGradientLength](double p) { return - p * betaFac / zGradientLength; });

                resultSample = std::make_shared<Models::Sample>(uValues);

                iteration++;
            }

            if (isLastRelaxation && Settings->FilterAtNonConvergence && !convergenceReport->IsConverged)
            {
                std::tie(beta, resultSample) = estimateBetaNonConv(lastBetas, lastSamples);
            }
            return modelRunner->getDesignPoint(resultSample, beta, convergenceReport, "FORM");
        }

        bool FORM::areAllResultsValid(const std::vector<double>& values)
        {
            bool foundNaN = std::ranges::any_of(values, [](const double x) {return std::isnan(x); });
            return !foundNaN;
        }

        bool FORM::isConverged(Models::ModelRunner& modelRunner, const Models::Sample& sample, ConvergenceReport& convergenceReport, double beta, double zGradientLength) const
        {
            const double uSquared = NumericSupport::GetSquaredSum(sample.Values);

            const double fromZeroDiff = uSquared > 0 ? std::fabs(beta * beta - uSquared) / uSquared : 0;
            const double localDiff = std::fabs(sample.Z / zGradientLength);
            const double betaDiff = std::max(fromZeroDiff, localDiff);

            convergenceReport.Convergence = betaDiff;
            convergenceReport.IsConverged = betaDiff <= this->Settings->EpsilonBeta;

            std::shared_ptr<ReliabilityReport> report = getReport(sample.IterationIndex, beta);
            report->ConvBeta = betaDiff;

            modelRunner.reportResult(report);

            //   check for convergence
            return convergenceReport.IsConverged;
        }

        std::shared_ptr<ReliabilityReport> FORM::getReport(int iteration, double reliability) const
        {
            std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();

            report->Step = iteration;
            report->Reliability = reliability;

            report->MaxSteps = this->Settings->MaximumIterations;
            report->ReportMatchesEvaluation = false;

            return report;
        }

        std::pair<double, std::shared_ptr<Models::Sample>> FORM::estimateBetaNonConv(const std::vector<double>& lastBetas, const std::vector<std::shared_ptr<Models::Sample>>& last10u)
        {
            const size_t nStochasts = last10u[0]->getSize();
            const size_t nIter = last10u.size();
            double rNIter = 1.0 / (double)nIter;
            double sumUk = 0.0;
            auto uk = std::vector<double>(nStochasts);
            for (size_t k = 0; k < nStochasts; k++)
            {
                uk[k] = 0.0;
                for (size_t iter = 0; iter < nIter; iter++)
                {
                    uk[k] += last10u[iter]->Values[k];
                }
                uk[k] *= rNIter;
                sumUk += pow(uk[k], 2);
            }

            double meanBeta = 0.0;
            for (size_t iter = 0; iter < nIter; iter++)
            {
                meanBeta += lastBetas[iter];
            }
            meanBeta *= rNIter;

            double signBeta = (meanBeta > 0.0 ? 1.0 : -1.0);
            double beta = signBeta * sqrt(sumUk);

            auto alpha = std::make_shared<Models::Sample>(nStochasts);

            for (size_t k = 0; k < nStochasts; k++)
            {
                alpha->Values[k] = uk[k];
            }
            return { beta, alpha };
        }

    }
}


