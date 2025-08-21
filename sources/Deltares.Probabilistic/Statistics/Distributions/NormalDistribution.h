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

namespace Deltares
{
    namespace Statistics
    {
        class NormalDistribution : public Distribution
        {
        public:
            void initialize(std::shared_ptr<StochastProperties> stochast, std::vector<double> values) override;
            double getXFromU(std::shared_ptr<StochastProperties> stochast, double u) override;
            double getUFromX(std::shared_ptr<StochastProperties> stochast, double x) override;
            bool isValid(std::shared_ptr<StochastProperties> stochast) override;
            bool isVarying(std::shared_ptr<StochastProperties> stochast) override;
            bool canTruncate() override { return true; }
            bool canFit() override { return true; }
            bool canFitPrior() override { return true; }
            double getMean(std::shared_ptr<StochastProperties> stochast) override;
            double getDeviation(std::shared_ptr<StochastProperties> stochast) override;
            void setMeanAndDeviation(std::shared_ptr<StochastProperties> stochast, double mean, double deviation) override;
            double getPDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            double getCDF(std::shared_ptr<StochastProperties> stochast, double x) override;
            void setXAtU(std::shared_ptr<StochastProperties> stochast, double x, double u, ConstantParameterType constantType) override;
            double getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x) override;
            void fit(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values) override;
            void fitWeighted(std::shared_ptr<StochastProperties> stochast, std::vector<double>& values, std::vector<double>& weights) override;
            void fitPrior(std::shared_ptr<StochastProperties> stochast, std::shared_ptr<StochastProperties> prior, std::vector<double>& values) override;
            std::vector<DistributionPropertyType> getParameters() override { return { Location, Scale }; }
        };
    }
}

