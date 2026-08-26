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
#pragma once

#include "../Model/Sample.h"
#include "../Math/RandomValueGenerator.h"
#include "ClusterSettings.h"

namespace Deltares::Optimization
{
    /**
     * \brief K-Means Clustering algorithm
     * \remark Code based on https://visualstudiomagazine.com/articles/2020/05/06/data-clustering-k-means.aspx
     */
    class KMeansClustering
    {

    public:
        /**
         * \brief Settings for k-Means clustering algorithm
         */
        std::shared_ptr<ClusterSettings> Settings = std::make_shared<ClusterSettings>();

        /**
         * \brief Generates clusters
         * \param samples Samples for which the clustering is generated
         * \return Centers of the clusters
         */
        std::vector<Models::Sample> getClusterCenters(std::vector<Models::Sample> samples);
    private:
        /**
         * \brief Internal class for cluster administration
         */
        class Cluster
        {
        public:
            Models::Sample Center = Models::Sample();
            std::vector<Models::Sample> Samples;
            std::vector<Models::Sample> PreviousSamples;

            double getSumSquared() const;
            void updateMean();
        };

        std::vector<std::shared_ptr<Cluster>> FixedCluster(std::vector<Models::Sample>& samples, const ClusterSettings& options);
        std::vector<std::shared_ptr<Cluster>> DoClustering(std::vector<Models::Sample>& samples, const ClusterSettings& options, Numeric::RandomValueGenerator& randomGenerator);
        std::vector<std::shared_ptr<Cluster>> InitializeClusters(std::vector<Models::Sample>& samples, const ClusterSettings& options, Numeric::RandomValueGenerator& randomGenerator);
        std::vector<std::shared_ptr<Cluster>> InitPlusPlus(int numberClusters, std::vector<Models::Sample>& samples, Numeric::RandomValueGenerator& randomGenerator, bool sampleHasWeighting);
        int ProporSelect(std::vector<double>& values, Numeric::RandomValueGenerator& randomGenerator);
        bool updateClustering(const std::vector<std::shared_ptr<Cluster>>& clusters);
        static std::shared_ptr<Cluster> getNearestCluster(Models::Sample& sample, const std::vector<std::shared_ptr<Cluster>>& clusters);
        static double SilhouetteCoefficient(const std::vector<std::shared_ptr<Cluster>>& clusters);
        static double IntraClusterDistance(const std::shared_ptr<Cluster>& cluster, Models::Sample& sample);
        static double InterClusterDistance(const std::shared_ptr<Cluster>& cluster,
            Models::Sample& sample, const std::vector<std::shared_ptr<Cluster>>& clusters);
        static std::vector<Models::Sample> getCentersFromClusters(const std::vector<std::shared_ptr<Cluster>>& clusters);

        const double margin = 1E-8;
    };
}

