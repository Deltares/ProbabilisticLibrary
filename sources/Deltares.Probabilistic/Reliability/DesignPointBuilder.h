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
#include "../Model/ModelRunner.h"

namespace Deltares
{
    namespace Reliability
    {
        class ModeFinder;

        enum DesignPointMethod
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
            double rmin = std::numeric_limits<double>::infinity();
            double sumWeights = 0;
            std::shared_ptr<Sample> defaultSample = nullptr;
            std::shared_ptr<Sample> meanSample = nullptr;
            std::shared_ptr<Sample> sinSample = nullptr;
            std::shared_ptr<Sample> cosSample = nullptr;
            bool sampleAdded = false;

            std::vector<int> qualitativeIndices;
            int qualitativeCount = 0;
            std::vector<std::shared_ptr<ModeFinder>> modeFinders;

            void initializeSamples(int count, DesignPointMethod method);

        public:
            DesignPointBuilder(int count, DesignPointMethod method, std::shared_ptr<StochastSettingsSet> stochastSet = nullptr);
            DesignPointBuilder(DesignPointMethod method, std::vector<std::shared_ptr<Statistics::Stochast>> stochasts);

            void initialize(double beta);
            void addSample(std::shared_ptr<Sample> sample);
            std::shared_ptr<Sample> getSample();

            static std::string getDesignPointMethodString(DesignPointMethod method);
            static DesignPointMethod getDesignPointMethod(std::string method);
        };
    }
}





