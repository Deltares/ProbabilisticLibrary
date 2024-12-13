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

#include "../Math/Random.h"

namespace Deltares
{
    namespace Optimization
    {
        /**
         * \brief Types of initialization of a cluster
         * \remark Forgy, Random are other initialization methods, but not implemented yet
         */
        enum class ClusterInitializationMethod { PlusPlus};

        /**
         * \brief Settings for clustering algorithm
         */
        class ClusterSettings
        {
        public:
            /**
             * \brief Sample Weighting should be accounted for in clustering
             */
            bool SampleHasWeighting = false;

            /**
             * \brief Requested number of clusters
             */
            int NumberClusters = 1;

            /**
             * \brief Maximum iterations within the clustering algorithm
             */
            int MaxIterations = 100;

            /**
             * \brief Number of trials. Each trial leads to another initial clustering set
             */
            int Trials = 10;

            /**
             * \brief Indicates whether number of clusters is fixed (false) or determined by the <see cref="IClusterMethod"/> algorithm
             */
            bool OptimizeNumberOfClusters = false;

            /**
             * \brief Initialization method
             */
            ClusterInitializationMethod clusterInitializationMethod = ClusterInitializationMethod::PlusPlus;

            /**
             * \brief Maximum number of clusters when OptimizeNumberOfClusters is true or number of clusters to be generated when OptimizeNumberOfClusters is false
             */
            int MaxClusters = 100;

            /**
             * \brief Minimum required relative improvement to continue increasing the number of clusters when <see cref="optimizeNumberOfClusters"/> is true
             */
            double MinImprovement = 0.1;

            /**
             * \brief The maximum number of samples used to generate clusters
             */
            int MaxSamples = 500;

            /**
             * \brief Indicates whether these settings are valid
             * \return Indication
             */
            bool isValid()
            {
                return MaxClusters >= 1;
            }

            /**
             * \brief The type of random number generator to be used.
             */
            Numeric::RandomValueGeneratorType generatorType = Numeric::RandomValueGeneratorType::ModifiedKnuthSubtractive;
        };
    }
}


