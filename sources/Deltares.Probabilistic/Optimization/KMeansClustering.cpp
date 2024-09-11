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
#include "KMeansClustering.h"

#include <map>

#include "../Utils/probLibException.h"
#include "../Math/Random.h"
#include "../Math/NumericSupport.h"

namespace Deltares
{
    namespace Optimization
    {
        double KMeansClustering::Cluster::getSumSquared()
        {
            double sum = 0;

            for (std::shared_ptr<Models::Sample> sample : this->Samples)
            {
                sum += this->Center->getDistance2(sample);
            }

            return sum;
        }

        void KMeansClustering::Cluster::updateMean()
        {
            if (this->Samples.empty())
            {
                throw Reliability::probLibException("Cluster must have at least one sample");
            }

            this->Center = std::make_shared<Models::Sample>(this->Samples[0]->Values.size());

            for (int i = 0; i < this->Center->Values.size(); i++)
            {
                double sumValues = 0;
                for (std::shared_ptr<Models::Sample> sample : this->Samples)
                {
                    sumValues += sample->Values[i];
                }

                this->Center->Values[i] = sumValues / this->Samples.size();
            }
        }

        std::vector<std::shared_ptr<Models::Sample>> KMeansClustering::getCentersFromClusters(std::vector<std::shared_ptr<KMeansClustering::Cluster>> clusters)
        {
            std::vector<std::shared_ptr<Models::Sample>> centers;
            for (std::shared_ptr<Cluster> cluster : clusters)
            {
                centers.push_back(cluster->Center);
            }

            return centers;
        }

        std::vector<std::shared_ptr<Models::Sample>> KMeansClustering::getClusterCenters(std::vector<std::shared_ptr<Models::Sample>> samples)
        {
            if (this->Settings->OptimizeNumberOfClusters)
            {
                std::vector<std::shared_ptr<Cluster>> clusters;

                double score0 = std::numeric_limits<double>::lowest();
                double score1 = std::numeric_limits<double>::lowest();
                double bestScore = std::numeric_limits<double>::lowest();

                for (int k = 2; k <= this->Settings->MaxClusters; k++)
                {
                    std::shared_ptr<ClusterSettings> fixedClusterOptions = std::make_shared<ClusterSettings>();

                    fixedClusterOptions->NumberClusters = k;
                    fixedClusterOptions->clusterInitializationMethod = this->Settings->clusterInitializationMethod;
                    fixedClusterOptions->MaxIterations = this->Settings->MaxIterations;
                    fixedClusterOptions->Trials = this->Settings->MaxIterations;
                    fixedClusterOptions->SampleHasWeighting = true;

                    double sumSquared;

                    std::vector<std::shared_ptr<Cluster>> newClusters = FixedCluster(samples, fixedClusterOptions, sumSquared);

                    double score2 = SilhouetteCoefficient(newClusters);

                    if (!clusters.empty() && (score0 > score1 && score1 > score2))
                    {
                        break;
                    }

                    if (score2 > bestScore)
                    {
                        clusters = newClusters;
                        bestScore = score2;
                    }
                    score0 = score1;
                    score1 = score2;

                }

                if (bestScore < 0.5)
                {
                    std::shared_ptr<ClusterSettings> fixedClusterOptions = std::make_shared<ClusterSettings>();

                    fixedClusterOptions->NumberClusters = 1;
                    fixedClusterOptions->clusterInitializationMethod = this->Settings->clusterInitializationMethod;
                    fixedClusterOptions->MaxIterations = this->Settings->MaxIterations;
                    fixedClusterOptions->Trials = this->Settings->MaxIterations;
                    fixedClusterOptions->SampleHasWeighting = true;

                    double sumSquared;
                    std::vector<std::shared_ptr<Cluster>> newClusters = FixedCluster(samples, fixedClusterOptions, sumSquared);

                    return getCentersFromClusters(newClusters);
                }

                return getCentersFromClusters(clusters);
            }
            else
            {
                std::shared_ptr<ClusterSettings> fixedClusterOptions = std::make_shared<ClusterSettings>();

                fixedClusterOptions->NumberClusters = this->Settings->MaxClusters;
                fixedClusterOptions->clusterInitializationMethod = this->Settings->clusterInitializationMethod;
                fixedClusterOptions->MaxIterations = this->Settings->MaxIterations;
                fixedClusterOptions->SampleHasWeighting = true;

                double sumSquared;
                std::vector<std::shared_ptr<Cluster>> clusters = FixedCluster(samples, fixedClusterOptions, sumSquared);

                return getCentersFromClusters(clusters);
            }
        }

        std::vector<std::shared_ptr<KMeansClustering::Cluster>> KMeansClustering::FixedCluster(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, double& sumSquared)
        {
            std::vector<std::shared_ptr<Cluster>> clusters;
            sumSquared = std::numeric_limits<double>::max(); // smaller is better

            int unchangedClusters = 0;
            constexpr int maxUnchangedClusters = 10;

            for (int trial = 0; trial < options->Trials; trial++)
            {
                std::vector<std::shared_ptr<Cluster>> newClusters = DoClustering(samples, options, trial); // find a clustering and update bests

                double newSumSquared = 0;
                for (std::shared_ptr<Cluster> cluster : newClusters)
                {
                    newSumSquared += cluster->getSumSquared();
                }

                if (clusters.empty() || newSumSquared < sumSquared) // new best clustering found
                {
                    unchangedClusters = 0;
                    clusters = newClusters;
                    sumSquared = newSumSquared;
                }
                else
                {
                    unchangedClusters++;
                }

                if (unchangedClusters >= maxUnchangedClusters)
                {
                    break;
                }
            }

            return clusters;
        }

        std::vector<std::shared_ptr<KMeansClustering::Cluster>> KMeansClustering::DoClustering(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, int randomSeed)
        {
            std::vector<std::shared_ptr<Cluster>> newClusters = InitializeClusters(samples, options, randomSeed);

            bool modified = true; //  result from UpdateClustering (to exit loop)
            int iteration = 0;

            while (modified && iteration++ < options->MaxIterations)
            {
                modified = updateClustering(newClusters);
            }

            return newClusters;
        }

        std::vector<std::shared_ptr<KMeansClustering::Cluster>> KMeansClustering::InitializeClusters(std::vector<std::shared_ptr<Models::Sample>>& samples, std::shared_ptr<ClusterSettings> options, int randomSeed)
        {
            switch (options->clusterInitializationMethod)
            {
            case ClusterInitializationMethod::PlusPlus:
                return InitPlusPlus(options->NumberClusters, samples, randomSeed, options->SampleHasWeighting);
            default:
                throw Reliability::probLibException("Cluster initialization method");
            }
        }

        std::vector<std::shared_ptr<KMeansClustering::Cluster>> KMeansClustering::InitPlusPlus(int numberClusters, std::vector<std::shared_ptr<Models::Sample>>& samples, int randomSeed, bool sampleHasWeighting)
        {
            std::vector<std::shared_ptr<Cluster>> clusters;

            // select one data item index at random as 1st mean
            Numeric::Random::initialize(Settings->generatorType, true, randomSeed, 0);

            // TODO: PROBL-42 remove this line 
            double dummy = Numeric::Random::next();  //  random->Next(0, samples.size()); // [0, N)

            std::shared_ptr<Cluster> firstCluster = std::make_shared<Cluster>();

            firstCluster->Samples.reserve(samples.size());
            for (std::shared_ptr<Models::Sample> sample : samples)
            {
                firstCluster->Samples.push_back(sample);
            }

            firstCluster->updateMean();

            clusters.push_back(firstCluster);

            for (int k = 1; k < numberClusters; k++) // find each remaining mean
            {
                std::vector<double> squaredDistances(samples.size()); // from each item to its closest mean
                int n = 0;

                for (std::shared_ptr<Models::Sample> sample : samples)
                {
                    std::shared_ptr<Cluster> nearestCluster = getNearestCluster(sample, clusters);

                    double distance2 = nearestCluster->Center->getDistance2(sample);

                    if (sampleHasWeighting)
                    {
                        distance2 *= sample->Weight * sample->Weight;
                    }

                    squaredDistances[n++] = distance2;
                }

                // select an item far from its mean using roulette wheel
                // if a sample has been used as a mean its distance will become 0 so it will not be selected
                int newMeanIndex = ProporSelect(squaredDistances);

                std::shared_ptr<Cluster> nextCluster = std::make_shared<Cluster>(samples[newMeanIndex]->clone());

                clusters.push_back(nextCluster);
            }

            updateClustering(clusters);

            return clusters;
        }

        int KMeansClustering::ProporSelect(std::vector<double>& values)
        {
            // on the fly technique
            // values[] can't be all 0.0s
            int n = values.size();

            double sum = Numeric::NumericSupport::sum(values, [](double p) {return p; });

            double cumP = 0.0; // cumulative prob

            double p = Numeric::Random::next();

            for (int i = 0; i < n; ++i)
            {
                cumP += values[i] / sum;
                if (cumP > p)
                {
                    return i;
                }
            }

            return n - 1; // last index
        }

        bool KMeansClustering::updateClustering(std::vector<std::shared_ptr<Cluster>>& clusters)
        {
            // proposed clustering would have an empty cluster: return false - no change to clustering
            // proposed clustering would be no change: return false, no change to clustering
            // proposed clustering is different and has no empty clusters: return true, clustering is changed

            // clear the current clustering
            for (const std::shared_ptr<Cluster>& cluster : clusters)
            {
                cluster->PreviousSamples.clear();

                cluster->PreviousSamples.reserve(cluster->Samples.size());
                for (const std::shared_ptr<Models::Sample>& sample : cluster->Samples)
                {
                    cluster->PreviousSamples.push_back(sample);
                }

                cluster->Samples.clear();
            }

            bool modified = false; // is there a change to the existing clustering?

            for (const std::shared_ptr<Cluster>& cluster : clusters)
            {
                for (const std::shared_ptr<Models::Sample>& sample : cluster->PreviousSamples)
                {
                    std::shared_ptr<Cluster> nearestCluster = getNearestCluster(sample, clusters);
                    nearestCluster->Samples.push_back(sample);

                    modified |= nearestCluster != cluster;
                }
            }

            // no change to clustering because would have an empty cluster
            for (const std::shared_ptr<Cluster>& cluster : clusters)
            {
                if (cluster->Samples.empty())
                {
                    return false;
                }
            }

            if (modified)
            {
                for (const std::shared_ptr<Cluster>& cluster : clusters)
                {
                    cluster->updateMean();
                }
            }

            return modified;
        }

        std::shared_ptr<KMeansClustering::Cluster> KMeansClustering::getNearestCluster(std::shared_ptr<Models::Sample> sample, std::vector<std::shared_ptr<Cluster>>& clusters)
        {
            std::shared_ptr<Cluster> nearestCluster = nullptr;
            double minDistance = std::numeric_limits<double>::max();

            for (auto cluster : clusters)
            {
                double distance = cluster->Center->getDistance2(sample);
                if (nearestCluster == nullptr || distance < minDistance)
                {
                    nearestCluster = cluster;
                    minDistance = distance;
                }
            }

            return nearestCluster;
        }

        double KMeansClustering::SilhouetteCoefficient(std::vector<std::shared_ptr<Cluster>>& clusters)
        {
            double score = 0.0;
            double nSamples = 0;
            for (auto cluster : clusters)
            {
                for (std::shared_ptr<Models::Sample> sample : cluster->Samples)
                {
                    ++nSamples;
                    double a_i = IntraClusterDistance(cluster, sample);
                    double b_i = InterClusterDistance(cluster, sample, clusters);
                    score += ((b_i - a_i) / std::max(b_i, a_i));
                }
            }
            return score / nSamples;
        }

        double KMeansClustering::IntraClusterDistance(std::shared_ptr<Cluster> cluster, std::shared_ptr<Models::Sample> sample)
        {
            double intraClusterDistance = 0.0;

            for (std::shared_ptr<Models::Sample> otherSample : cluster->Samples)
            {
                if (sample != otherSample)
                {
                    intraClusterDistance += sample->getDistance(otherSample);
                }
            }

            return intraClusterDistance / (cluster->Samples.size() - 1);
        }

        double KMeansClustering::InterClusterDistance(std::shared_ptr<Cluster> cluster, std::shared_ptr<Models::Sample> sample, std::vector<std::shared_ptr<Cluster>>& clusters)
        {
            double interClusterDistance = std::numeric_limits<double>::max();

            for (std::shared_ptr<Cluster> otherCluster : clusters)
            {
                if (cluster != otherCluster)
                {
                    double clusterDissimilarity = 0.0;

                    for (const std::shared_ptr<Models::Sample> otherSample : otherCluster->Samples)
                    {
                        clusterDissimilarity += sample->getDistance(otherSample);
                    }

                    clusterDissimilarity /= otherCluster->Samples.size();

                    if (clusterDissimilarity < interClusterDistance)
                    {
                        interClusterDistance = clusterDissimilarity;
                    }
                }
            }

            return interClusterDistance;
        }

    }
}

