// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of Streams.
//
// Streams is free software: you can redistribute it and/or modify
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
#include "DirectionalSamplingS.h"
#include <vector>
#include <cmath>
#include <memory>
#include <algorithm>

#include "../Math/NumericSupport.h"
#include "../Math/SpecialFunctions.h"
#include "../Math/RootFinders/BisectionRootFinder.h"
#include "../Model/Sample.h"
#include "../Model/GradientCalculator.h"
#include "../Model/RandomSampleGenerator.h"
#include "../Statistics/DistributionType.h"

using namespace Deltares::Models;

namespace Deltares
{
    namespace Sensitivity
    {
        std::shared_ptr<Statistics::Stochast> DirectionalSamplingS::getSensitivityStochast(std::shared_ptr<Models::ModelRunner> modelRunner)
        {
            //Step 0: Initialize the algorithm

            modelRunner->updateStochastSettings(this->Settings->StochastSet);

            int nStochasts = modelRunner->getVaryingStochastCount();

            // Step 1: Calculate Z0, the model result at u = 0.

            std::shared_ptr<Sample> u0 = std::make_shared<Sample>(nStochasts);
            u0->IterationIndex = -1;
            double Z0 = modelRunner->getZValue(u0);

            // Step 2: Calculate beta_q, the reliability index corresponding to the given quantile value / exceeding probability

            std::shared_ptr<Statistics::Stochast> stochast = std::make_shared<Statistics::Stochast>();
            stochast->setDistributionType(Statistics::CDFCurve);

            for (size_t i = 0; i < this->Settings->RequestedQuantiles.size(); i++)
            {
                std::shared_ptr<Statistics::FragilityValue> fragilityValue = std::make_shared<Statistics::FragilityValue>();
                fragilityValue->X = getZForRequiredQ(modelRunner, this->Settings->RequestedQuantiles[i]->Reliability, nStochasts, Z0);
                fragilityValue->Reliability = this->Settings->RequestedQuantiles[i]->Reliability;
                stochast->getProperties()->FragilityValues.push_back(fragilityValue);
            }

            std::sort(stochast->getProperties()->FragilityValues.begin(), stochast->getProperties()->FragilityValues.end(), [](std::shared_ptr<Statistics::FragilityValue> val1, std::shared_ptr<Statistics::FragilityValue> val2) {return val1->X < val2->X; });

            return stochast;
        }

        double DirectionalSamplingS::getZForRequiredQ(std::shared_ptr<Models::ModelRunner> modelRunner, double betaRequested, int nStochasts, double Z0)
        {
            int performedIterations = 0;

            // Step 3: Calculate n samples in random directions, all at the same distance d of the origin

            int maxIterations = this->Settings->MaximumIterations;
            int directionNumber = this->Settings->NumberDirections;
            double tolerance = this->Settings->VariationCoefficientFailure;

            std::shared_ptr<SampleProvider> sampleProvider = std::make_shared<SampleProvider>(this->Settings->StochastSet, false);
            modelRunner->setSampleProvider(sampleProvider);

            const std::shared_ptr<RandomSampleGenerator> randomSampleGenerator = std::make_shared<RandomSampleGenerator>();
            randomSampleGenerator->Settings = this->Settings->randomSettings;
            randomSampleGenerator->Settings->StochastSet = this->Settings->StochastSet;
            randomSampleGenerator->sampleProvider = sampleProvider;
            randomSampleGenerator->initialize();

            std::vector<std::shared_ptr<Sample>> samples;

            for (int i = 0; i < directionNumber; i++)
            {
                std::shared_ptr<Sample> randomSample = randomSampleGenerator->getRandomSample();
                samples.push_back(randomSample);
            }

            // Calculate the corresponding distance d of the origin using the length of the samples list

            double initialDistance = getBetaDistance(std::abs(betaRequested), nStochasts);
            initialDistance = std::max(0.1, initialDistance);

            // Normalize the value of Samples via d
            for (size_t i = 0; i < samples.size(); i++)
            {
                samples[i] = samples[i]->getSampleAtBeta(initialDistance);
                samples[i]->IterationIndex = static_cast<int>(i);
            }

            std::vector<double> zValues = modelRunner->getZValues(samples);

            modelRunner->reportProgress(++performedIterations, maxIterations + 1);

            int nZValuesGreaterZero = 0;
            for (double z : zValues)
            {
                if (z > Z0)
                {
                    nZValuesGreaterZero++;
                }
            }

            double probability0 = Numeric::NumericSupport::Divide(nZValuesGreaterZero, static_cast<int>(zValues.size()));
            double beta0 = Statistics::StandardNormal::getUFromQ(probability0);
            double maxBetaDirection = std::abs(betaRequested) + 5; // The maximum beta value for the direction is set to the requested beta value plus 5 to prevent the algorithm from running into extreme values for the distance d

            std::vector<std::shared_ptr<Direction>> directions;
            for (int i = 0; i < samples.size(); i++)
            {
                std::shared_ptr<Direction> direction = std::make_shared<Direction>(samples[i]);
                direction->AddResult(0, Z0);
                direction->AddResult(initialDistance, zValues[i]);
                direction->Valid = betaRequested > beta0 ? zValues[i] > Z0 : zValues[i] < Z0; // If the direction is invalid, the direction will not be used in the calculation
                directions.push_back(direction);
            }

            // Step 4: Calculate new distances di,2 for each direction so that the combined reliability remains beta_q
            double qRequired = Statistics::StandardNormal::getQFromU(std::abs(betaRequested));
            std::shared_ptr<Numeric::BisectionRootFinder> bisection = std::make_shared<Numeric::BisectionRootFinder>();

            //firstly the zPred is needed.
            double zPred = 0;

            double error = std::numeric_limits<double>::max();

            int j = 0; //iteration over N
            while (j < maxIterations && error > tolerance)
            {
                double zMin = Z0;
                double zMax = betaRequested > beta0 ? Numeric::NumericSupport::getMaximum(zValues) : Numeric::NumericSupport::getMinimum(zValues);

                zPred = bisection->CalculateValue(zMin, zMax, qRequired,
                    [&, directions, nStochasts, probability0](double predZi) { return predict(predZi, directions, probability0, nStochasts); });

                //for each new scale in direction i values, the new z values are calculated (zValues[i,j])
                std::vector<std::shared_ptr<Sample>> newSamples;
                std::vector<std::shared_ptr<Sample>> calculateSamples;

                for (size_t i = 0; i < directions.size(); i++)
                {
                    std::shared_ptr<Sample> newSample = directions[i]->CreateNewSampleAt(zPred, maxBetaDirection);
                    newSamples.push_back(newSample);
                    if (directions[i]->Valid)
                    {
                        newSample->IterationIndex = static_cast<int>(i);
                        calculateSamples.push_back(newSample); //calculateSamples are the samples that are used to calculate the new z values ( only for valid directions to prevent z values of NaN/infinity)
                    }
                }

                modelRunner->getZValues(calculateSamples);

                std::vector<double> newZValues = Sample::select(newSamples, [](std::shared_ptr<Sample> p) {return p->Z; });

                //add the newZvalues to the list of zValues
                for (size_t i = 0; i < directions.size(); i++)
                {
                    if (directions[i]->Valid)
                    {
                        directions[i]->AddResult(directions[i]->GetDistanceAtZ(zPred), newSamples[i]->Z);
                    }
                }

                //double Pf = CalculateProbabilityOfFailure(NewdValues, nstochasts);
                error = calculateError(zValues, newZValues);

                j++;
                zValues = newZValues;

                modelRunner->reportProgress(++performedIterations, maxIterations + 1);
            }

            return zPred;
        }

        double DirectionalSamplingS::predict(double predZi, std::vector<std::shared_ptr<Direction>> directions, double probability0, int nStochasts)
        {
            std::vector<double> dValues(directions.size());

            for (size_t i = 0; i < directions.size(); i++)
            {
                dValues[i] = directions[i]->GetDistanceAtZ(predZi); // Record dValues for each direction (i) and for each iteration (j)
            }

            // Check if all dValues are zero
            bool allZero = true;
            for (double p : dValues)
            {
                if (p != 0.0)
                {
                    allZero = false;
                    break;
                }
            }

            if (allZero)
            {
                // If all dValues are zero, set probFailure to 0.5
                return probability0;
            }
            else
            {
                // Otherwise, calculate the probability of failure using the existing method
                return calculateProbabilityOfFailure(dValues, nStochasts);
            }
        }

        double DirectionalSamplingS::calculateProbabilityOfFailure(std::vector<double>& dValues, double nstochasts)
        {
            double qTotal = 0;
            for (size_t i = 0; i < dValues.size(); i++)
            {
                double beta = dValues[i]; // Here, dValues are directly used as beta values
                if (beta > 0)
                {
                    double weight = Numeric::SpecialFunctions::getGammaUpperRegularized(0.5 * nstochasts, 0.5 * beta * beta); //Here the dValues length is used as nStochasts
                    qTotal += weight;
                }
            }

            return qTotal / dValues.size(); // Here, the number of directions is used in the calculations (valid+invalid directions)
        }

        double DirectionalSamplingS::getBetaDistance(double betaRequired, int nStochasts)
        {
            if (betaRequired == 0.0)
            {
                return 0;
            }
            else
            {
                Numeric::RootFinderMethod method = [nStochasts](double beta)
                {
                    return Numeric::SpecialFunctions::getGammaUpperRegularized(0.5 * nStochasts, 0.5 * beta * beta); //here no division by number of direction is needed as the same d applied for every direction.
                };

                double qRequired = Statistics::StandardNormal::getQFromU(betaRequired);

                std::shared_ptr<Numeric::BisectionRootFinder> bisection = std::make_shared<Numeric::BisectionRootFinder>();

                double distance = bisection->CalculateValue(0, 2 * betaRequired, qRequired, method);
                return distance;
            }
        }

        double DirectionalSamplingS::calculateError(std::vector<double>& zValues, std::vector<double>& newZValues)
        {
            if (zValues.size() != newZValues.size())
            {
                throw Reliability::probLibException("Arrays must be of the same length.");
            }

            double error = 0;

            for (size_t i = 0; i < zValues.size(); i++)
            {
                error += std::fabs(zValues[i] - newZValues[i]);
            }
            return error / zValues.size();
        }

        void DirectionalSamplingS::Direction::AddResult(double distance, double z)
        {
            distances.push_back(distance);
            zValues.push_back(z);

            // Create a list of Result objects
            std::vector<std::shared_ptr<Result>> results;
            for (size_t i = 0; i < distances.size(); i++)
            {
                std::shared_ptr<Result> tempVar = std::make_shared<Result>();
                tempVar->Distance = distances[i];
                tempVar->ZValue = zValues[i];
                results.push_back(tempVar);
            }

            // Sort the results based on the Distance property
            std::sort(results.begin(), results.end(), [](std::shared_ptr<Result> p, std::shared_ptr<Result> q) {return p->Distance < q->Distance; });

            // Clear the original lists and repopulate them with the sorted values
            distances.clear();
            zValues.clear();

            for (auto result : results)
            {
                distances.push_back(result->Distance);
                zValues.push_back(result->ZValue);
            }
        }

        double DirectionalSamplingS::Direction::GetDistanceAtZ(double z)
        {
            if (Valid)
            {
                return Numeric::NumericSupport::interpolate(z, zValues, distances, true);
            }
            else
            {
                return 0;
            }
        }

        std::shared_ptr<Sample> DirectionalSamplingS::Direction::CreateNewSampleAt(double z, double maxBeta)
        {
            double distance = Numeric::NumericSupport::interpolate(z, zValues, distances, true); //interpolates the distance (d) for the given z value in the direction
            //double distance = GetDistanceAtZ(z);

            if (!Valid || distance < 0.0 || distance > maxBeta)
            {
                if (Valid)
                {
                    Valid = false;
                }
                return sample;
            }
            else
            {
                return newSample = sample->getSampleAtBeta(distance);
            }
        }

    }
}



