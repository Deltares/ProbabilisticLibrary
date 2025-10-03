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

#include "Distribution.h"

namespace Deltares::Numeric
{
    class WeightedValue;
}

namespace Deltares
{
    namespace Statistics
    {
        class HistogramDistribution : public Distribution
        {
        public:
            void initializeForRun(std::shared_ptr<StochastProperties> stochast) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            bool canFit() override { return true; }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, double shift = nan("")) override;
            void fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<double> getDiscontinuityPoints(const StochastProperties&) override;
        private:
            double getSizeForEmptySizedRange(std::shared_ptr<StochastProperties> stochast);
            void splitRanges(std::shared_ptr<StochastProperties> stochast, std::vector<std::shared_ptr<Numeric::WeightedValue>>& values);
            double getAmount(std::shared_ptr<HistogramValue> range, std::vector<std::shared_ptr<Numeric::WeightedValue>>& values);
            void mergeLowWeights(std::vector<std::shared_ptr<Numeric::WeightedValue>>& values);
            size_t getDistinctCount(std::vector<std::shared_ptr<Numeric::WeightedValue>>& values);
        };
    }
}

