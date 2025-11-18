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
#include "../../Math/NumericSupport.h"

namespace Deltares
{
    namespace Statistics
    {
        struct multipleInExtremes
        {
            bool multipleAtMin;
            bool multipleAtMax;
            double min;
            double max;
        };

        class HistogramDistribution : public Distribution
        {
        public:
            void initializeForRun(StochastProperties& stochast) override;
            double getXFromU(StochastProperties& stochast, double u) override;
            double getUFromX(StochastProperties& stochast, double x) override;
            bool isVarying(StochastProperties& stochast) override;
            void validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject) override;
            bool canFit() override { return true; }
            double getMean(StochastProperties& stochast) override;
            double getDeviation(StochastProperties& stochast) override;
            void setMeanAndDeviation(StochastProperties& stochast, double mean, double deviation) override;
            double getPDF(StochastProperties& stochast, double x) override;
            double getCDF(StochastProperties& stochast, double x) override;
            void fit(StochastProperties& stochast, const std::vector<double>& values, const double shift) override;
            void fitWeighted(StochastProperties& stochast, const std::vector<double>& values, std::vector<double>& weights) override;
            std::vector<double> getSpecialPoints(StochastProperties& stochast) override;
            std::vector<double> getDiscontinuityPoints(StochastProperties&) override;
        private:
            static double getSizeForEmptySizedRange(const StochastProperties& stochast);
            static void splitRanges(StochastProperties& stochast, const std::vector<Numeric::WeightedValue>& values);
            static double getAmount(const std::shared_ptr<HistogramValue>& range, const std::vector<Numeric::WeightedValue>& values);
            static void mergeLowWeights(std::vector<Numeric::WeightedValue>& values);
            static size_t getDistinctCount(const std::vector<Numeric::WeightedValue>& values);
            static void determineRangesLoop(StochastProperties& stochast, const std::vector<Numeric::WeightedValue>& x,
                int requiredRanges, const multipleInExtremes& multiples);
        };
    }
}

