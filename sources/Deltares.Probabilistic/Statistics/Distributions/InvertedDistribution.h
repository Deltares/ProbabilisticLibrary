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
#include <memory>

#include "Distribution.h"

namespace Deltares
{
    namespace Statistics
    {
        class InvertedDistribution : public Distribution
        {
        public:

            InvertedDistribution(std::shared_ptr<Distribution> innerDistribution)
            {
                this->innerDistribution = innerDistribution;
            }

            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool canTruncate() override { return this->innerDistribution->canTruncate(); }
            bool canInvert() override { return true; }
            bool canFit() override { return this->innerDistribution->canFit(); }
            bool canFitPrior() override { return this->innerDistribution->canFitPrior(); }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            void setShift(std::shared_ptr<StochastProperties> stochast, double shift, bool inverted) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, const double shift = nan("")) override;
            void fitPrior(const std::shared_ptr<StochastProperties>& stochast, std::vector<double>& values, const std::shared_ptr<StochastProperties>& prior, const double shift = nan("")) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x) override;
            std::vector<double> getSpecialPoints(std::shared_ptr<StochastProperties> stochast) override;
            std::vector<DistributionPropertyType> getParameters() override;
        private:
            std::shared_ptr<Distribution> innerDistribution = nullptr;
            double getInvertedValue(std::shared_ptr<StochastProperties> stochast, double value);
            std::shared_ptr<StochastProperties> getInvertedStochast(std::shared_ptr<StochastProperties> stochast);
            void copyFromInverted(std::shared_ptr<StochastProperties> target, std::shared_ptr<StochastProperties> source);
        };
    }
}
