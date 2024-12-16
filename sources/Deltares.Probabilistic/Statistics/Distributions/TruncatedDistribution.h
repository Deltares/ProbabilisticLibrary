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
#pragma once
#include <memory>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        struct Truncated;

        class TruncatedDistribution : public Distribution
        {
        public:
            TruncatedDistribution(std::shared_ptr<Distribution> innerDistribution)
            {
                this->innerDistribution = innerDistribution;
            }

            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool canTruncate() override { return true; }
            bool canInvert() override { return this->innerDistribution->canInvert(); }
            bool canFit() override { return this->innerDistribution->canFit(); }
            bool isShiftUsed() override { return this->innerDistribution->isShiftUsed(); }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            void setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override;
        private:
            std::shared_ptr<Distribution> innerDistribution = nullptr;

            Truncated getTruncatedValue(std::shared_ptr<StochastProperties> stochast);
            double getProbability(std::shared_ptr<StochastProperties> stochast, bool isMinimum);
            double getUntruncatedU(double u, std::shared_ptr<StochastProperties> stochast);
        };
    }
}

