#include "NumericalIntegration.h"
#include "DesignPoint.h"
#include "DesignPointBuilder.h"
#include "../Statistics/StandardNormal.h"

namespace Deltares
{
    namespace Reliability
    {
        using namespace Deltares::Numeric;

        std::shared_ptr<DesignPoint> NumericalIntegration::getDesignPoint(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            modelRunner->updateStochastSettings(Settings.StochastSet);

            int nStochasts = modelRunner->getVaryingStochastCount();

            // local variables
            std::shared_ptr<Sample> u = std::make_shared<Sample>(nStochasts); //local vector with values in u-space

            // Examine the position of the origin: if the origin has a negative z-value the design point must be searched
            // with positive values of the z-function. The explanation for searching with positive z-values as the origin
            // has a negative z-value: if searching with negative z-values, the origin is found as the design point, because
            // that is the point who's closest to the origin (distance is zero). For the computation of the probability of
            // failure all elements of negative z-value has te be used.
            double z = modelRunner->getZValue(u);

            // parameter for the position of the origin; necessary to determine the design point
            double z0Fac = z > 0.0 ? 1.0 : -1.0; 

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

            std::shared_ptr<DesignPointBuilder> designPointBuilder = std::make_shared<DesignPointBuilder>(nStochasts, Settings.designPointMethod, Settings.StochastSet);
            double totalDensity = 0;

            // Call to the recursive part of the numerical integration computation
            // the result of this computation is the probability of failure and the
            // direction of the design point (alpha values).

            std::shared_ptr<ConvergenceReport> convergenceReport = std::make_shared<ConvergenceReport>();

            double probFailure = getStochastProbability(modelRunner, stochastIndex, u, designPointBuilder, density, z0Fac, totalDensity);

            probFailure = probFailure / totalDensity;

            // Compute beta from the probability failure. Beta is because of this no longer per definition the
            // distance from the design point to the origin.
            const double beta = Statistics::StandardNormal::getUFromQ(probFailure);

            std::shared_ptr<Sample> designPoint = designPointBuilder->getSample();

            return modelRunner->getDesignPoint(designPoint, beta, convergenceReport);
        }

        double NumericalIntegration::getStochastProbability(std::shared_ptr<ModelRunner> modelRunner, int stochastIndex, std::shared_ptr<Sample> parentSample, std::shared_ptr<DesignPointBuilder> designPointBuilder, double density, double z0Fac, double& totalDensity)
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
            Statistics::PQ pq = Statistics::StandardNormal::getPQFromU(uValues[0]);

            if (stochastIndex < nStochasts - 1)
            {
                double probFailure = 0;

                for (int j = 0; j < uValues.size() - 1; j++)
                {
                    parentSample->Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    const Statistics::PQ pqPrev = pq;
                    pq = Statistics::StandardNormal::getPQFromU(uValues[j + 1]);

                    // depending on the value of u(i) use the probabilities of exceeding or the probabilities of non-exceeding
                    const double contribution = parentSample->Values[stochastIndex] < 0 ? pq.p - pqPrev.p : pqPrev.q - pq.q;

                    probFailure += getStochastProbability(modelRunner, stochastIndex + 1, parentSample, designPointBuilder, density * contribution, z0Fac, totalDensity);

                    if (stochastIndex == 0)
                    {
                        double beta = -Statistics::StandardNormal::getUFromP(probFailure);

                        std::shared_ptr<ReliabilityReport> report = std::make_shared<ReliabilityReport>();
                        report->Step = j;
                        report->MaxSteps = (int)uValues.size();
                        report->Reliability = beta;

                        modelRunner->reportResult(report);
                    }
                }

                return probFailure;
            }
            else
            {
                std::vector<std::shared_ptr<Sample>> samples;

                for (int j = 0; j < uValues.size() - 1; j++)
                {
                    std::shared_ptr<Sample> sample = parentSample->clone();
                    sample->Values[stochastIndex] = (uValues[j] + uValues[j + 1]) / 2;

                    samples.push_back(sample);
                }

                // compute the z-value(s)
                const std::vector<double> zValues = modelRunner->getZValues(samples); // z-value (failure if z < 0)

                double probFailure = 0;

                for (int j = 0; j < samples.size(); j++)
                {
                    const std::shared_ptr<Sample> sample = samples[j];

                    if (!std::isnan(zValues[j]))
                    {
                        Statistics::PQ pqPrev = pq;
                        pq = Statistics::StandardNormal::getPQFromU(uValues[j + 1]);

                        // depending on the value of u(i) use the probabilities of exceeding or the probabilities of non-exceeding
                        const double contribution = sample->Values[stochastIndex] < 0 ? pq.p - pqPrev.p : pqPrev.q - pq.q;

                        sample->Weight = density * contribution;
                        totalDensity += sample->Weight;

                        // if the z-value is negative add the probability density to the probability of failure
                        if (zValues[j] < 0.0)
                        {
                            probFailure += density * contribution;
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

