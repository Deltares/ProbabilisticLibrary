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
#include "NumericalIntegration.h"
#include "DesignPoint.h"
#include "DesignPointBuilder.h"
#include "../Statistics/StandardNormal.h"
#include "../Statistics/ProbabilityIterator.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> NumericalIntegration::getDesignPoint(std::shared_ptr<Models::ModelRunner> runner)
        {
            modelRunner = runner;
            modelRunner->updateStochastSettings(Settings.StochastSet);

            int nStochasts = modelRunner->getVaryingStochastCount();

            // local variables
            const auto u = std::make_shared<Sample>(nStochasts); //local vector with values in u-space

            // Examine the position of the origin: if the origin has a negative z-value the design point must be searched
            // with positive values of the z-function. The explanation for searching with positive z-values as the origin
            // has a negative z-value: if searching with negative z-values, the origin is found as the design point, because
            // that is the point who's closest to the origin (distance is zero). For the computation of the probability of
            // failure all elements of negative z-value has to be used.
            const double z = modelRunner->getZValue(u);

            // parameter for the position of the origin; necessary to determine the design point
            z0Fac = NumericSupport::GetSign(z);

            // Numerical integration isn't possible with a large set of stochastic parameters
            // warnings and errors are presented.
            if (nStochasts > 11)
            {
                modelRunner->reportMessage(Models::MessageType::Error, "Numerical integration with more than 11 stochastic parameters. This is practically impossible.");

                std::shared_ptr<ReliabilityReport> reportInvalid = std::make_shared<ReliabilityReport>();
                reportInvalid->Step = 1;
                reportInvalid->MaxSteps = 1;
                modelRunner->reportResult(reportInvalid);

                return modelRunner->getDesignPoint(u, nan(""), nullptr);
            }
            else if (nStochasts > 4)
            {
                modelRunner->reportMessage(Models::MessageType::Warning, "Numerical integration with more than 4 stochastic parameters. Large computation time is possible.");
            }

            // Initialize parameters for the recursive part of the numerical integration computation.
            constexpr double density = 1.0; // joint probability density
            constexpr int stochastIndex = 0; // number of the stochastic parameter

            designPointBuilder = std::make_shared<DesignPointBuilder>(nStochasts, Settings.designPointMethod, Settings.StochastSet);
            double totalDensity = 0;

            // Call to the recursive part of the numerical integration computation
            // the result of this computation is the probability of failure and the
            // direction of the design point (alpha values).

            auto convergenceReport = std::make_shared<ConvergenceReport>();

            double probFailure = getStochastProbability(stochastIndex, *u, density, totalDensity, 1);

            probFailure /= totalDensity;

            // Compute beta from the probability failure. Beta is because of this no longer per definition the
            // distance from the design point to the origin.
            const double beta = Statistics::StandardNormal::getUFromQ(probFailure);

            const auto designPoint = designPointBuilder->getSample();

            return modelRunner->getDesignPoint(designPoint, beta, convergenceReport);
        }

        double NumericalIntegration::getStochastProbability(int stochastIndex, Sample& parentSample, double density, double& totalDensity, int nSamples)
        {
            const double uDelta = 0.01;
            const int nStochasts = Settings.StochastSet->getVaryingStochastCount();

            // Initialize parameters for stochastic parameter u.
            const int nrIntervals = Settings.StochastSet->VaryingStochastSettings[stochastIndex]->Intervals; // number of intervals in u-space as integer
            const double uMin = Settings.StochastSet->VaryingStochastSettings[stochastIndex]->MinValue; // lower bound for u
            const double uMax = Settings.StochastSet->VaryingStochastSettings[stochastIndex]->MaxValue; // upper bound for u
            const double rangeU = uMax - uMin;

            // Build up list of u values to be computed
            std::vector<double> uValues = { - Statistics::StandardNormal::UMax };

            for (int i = 0; i <= nrIntervals; i++)
            {
                double uValue = uMin + i * rangeU / nrIntervals;
                if (uValue > -Statistics::StandardNormal::UMax + uDelta && uValue < Statistics::StandardNormal::UMax - uDelta)
                {
                    uValues.push_back(uValue);
                }
            }
            uValues.push_back(Statistics::StandardNormal::UMax);

            // Initialize first probabilities
            auto pq = Statistics::ProbabilityIterator(uValues[0]);

            nSamples *= static_cast<int>(uValues.size()) - 1;

            if (stochastIndex < nStochasts - 1)
            {
                double probFailure = 0;

                for (size_t j = 0; j < uValues.size() - 1; j++)
                {
                    parentSample.Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    const double contribution = pq.getDifference(uValues[j + 1]);

                    probFailure += getStochastProbability(stochastIndex + 1, parentSample, density * contribution, totalDensity, nSamples);

                    if (stochastIndex == 0)
                    {
                        const double beta = -Statistics::StandardNormal::getUFromP(probFailure);

                        auto report = std::make_shared<ReliabilityReport>();
                        report->Step = static_cast<int>(j);
                        report->MaxSteps = nSamples;
                        report->Reliability = beta;

                        modelRunner->reportResult(report);
                    }
                }

                return probFailure;
            }
            else
            {
                std::vector<std::shared_ptr<Sample>> samples;

                for (size_t j = 0; j < uValues.size() - 1; j++)
                {
                    std::shared_ptr<Sample> sample = parentSample.clone();
                    sample->Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    const double contribution = pq.getDifference(uValues[j + 1]);
                    sample->Weight = density * contribution * nSamples;

                    samples.push_back(sample);
                }

                // compute the z-value(s)
                const auto zValues = modelRunner->getZValues(samples); // z-value (failure if z < 0)

                double probFailure = 0;

                for (size_t j = 0; j < samples.size(); j++)
                {
                    const auto sample = samples[j];

                    if (!std::isnan(zValues[j]))
                    {
                        const double sampleProbability = sample->Weight / nSamples;

                        totalDensity += sampleProbability;

                        // if the z-value is negative add the probability density to the probability of failure
                        if (zValues[j] < 0.0)
                        {
                            probFailure += sampleProbability;
                        }

                        // If the combination of the z-value and the parameter for the position of the origin is negative
                        // it is possible that a new design point is computed. Then also the alpha values are computed.
                        if (zValues[j] * z0Fac < 0)
                        {
                            designPointBuilder->addSample(sample);
                        }
                    }
                }

                return probFailure;
            }
        }
    }
}

