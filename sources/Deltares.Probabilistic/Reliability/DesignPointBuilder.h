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

#include <stack>

#include "../Model/Sample.h"
#include "../Model/ModelRunner.h"

namespace Deltares::Reliability
{
    class ModeFinder;

    enum class DesignPointMethod
    {
        CenterOfGravity,
        CenterOfAngles,
        NearestToMean
    };

    class DesignPointBuilder
    {
    private:
        int count = 0;
        DesignPointMethod method = DesignPointMethod::NearestToMean;
        bool addProbability = false;
        double minimumBeta = std::numeric_limits<double>::infinity();
        double sumWeights = 0;
        Models::Sample defaultSample = Models::Sample();
        Models::Sample meanSample = Models::Sample();
        Models::Sample sinSample = Models::Sample();
        Models::Sample cosSample = Models::Sample();
        bool sampleAdded = false;
        bool weightedSampleAdded = false;
        std::vector<Models::Sample> nearestSamples;

        std::vector<int> qualitativeIndices;
        int qualitativeCount = 0;
        std::vector<std::shared_ptr<ModeFinder>> modeFinders;

        void handleSample(Models::Sample& sample, double weight);
        static Models::Sample getSampleWithProbability(Models::Sample& sample, double probability);
        void initializeTotals();

    public:
        DesignPointBuilder() = default;
        explicit DesignPointBuilder(int count, DesignPointMethod method, const std::shared_ptr<StochastSettingsSet>& stochastSet = nullptr, bool addProbability = false);
        explicit DesignPointBuilder(DesignPointMethod method, const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts);

        void initialize(double beta);
        void addSample(Models::Sample& sample, double probability = 1.0);
        void removeSample(Models::Sample& sample);
        Models::Sample getSample() const;

        static std::string getDesignPointMethodString(DesignPointMethod method);
        static DesignPointMethod getDesignPointMethod(const std::string& method);
    };
}

