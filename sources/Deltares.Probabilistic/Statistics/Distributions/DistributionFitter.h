// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
#pragma once
#include <memory>
#include <vector>

#include "Distribution.h"
#include "../../Model/ModelSample.h"

namespace Deltares
{
    namespace Statistics
    {
        class DistributionFitter
        {
        public:
            std::vector<double> fitByLogLikelihood(std::vector<double>& values, Distribution* distribution, std::shared_ptr<StochastProperties> stochast, std::vector<double>& minimum, std::vector<double>& maximum, std::vector<double>& initial, std::vector<DistributionPropertyType>& properties);
        private:
            std::shared_ptr<StochastProperties> stochast = nullptr;
            std::vector<double> values;
            std::vector<DistributionPropertyType> properties;
            Distribution* distribution = nullptr;
            void getLogLikelihood(std::shared_ptr<Models::ModelSample> sample);
            double getSumLogLikelihood();
        };
    }
}

