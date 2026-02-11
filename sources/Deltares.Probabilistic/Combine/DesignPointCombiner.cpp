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
#include "DesignPointCombiner.h"

#include "HohenbichlerExcludingCombiner.h"
#include "WeightedSumCombiner.h"
#include "ImportanceSamplingCombiner.h"
#include "HohenbichlerNumIntCombiner.h"
#include "DirectionalSamplingCombiner.h"
#include "HohenbichlerFormCombiner.h"

namespace Deltares
{
    namespace Reliability
    {
        std::shared_ptr<Combiner> DesignPointCombiner::getCombiner() const
        {
            switch (combinerType)
            {
            case CombinerType::ImportanceSampling:
            {
                auto impSamplingCombiner = std::make_shared<ImportanceSamplingCombiner>();
                return impSamplingCombiner;
            }
            case CombinerType::Hohenbichler:
                return std::make_shared<HohenbichlerNumIntCombiner>();
            case CombinerType::DirectionalSampling:
            {
                auto directionalSamplingCombiner = std::make_shared<DirectionalSamplingCombiner>();
                return directionalSamplingCombiner;
            }
            case CombinerType::HohenbichlerForm:
            {
                auto hhFormCmb = std::make_shared<HohenbichlerFormCombiner>();
                return hhFormCmb;
            }
            default: throw probLibException("Combiner type");
            }
        }

        std::shared_ptr<DesignPoint> DesignPointCombiner::combineDesignPoints(combineAndOr combineMethodType,
            std::vector<std::shared_ptr<DesignPoint>>& designPoints,
            const std::shared_ptr<Statistics::SelfCorrelationMatrix>& selfCorrelationMatrix,
            const std::shared_ptr<Statistics::BaseCorrelation>& correlationMatrix,
            const std::shared_ptr<Models::ProgressIndicator>& progress) const
        {
            const std::shared_ptr<Combiner> combiner = getCombiner();
            std::shared_ptr<DesignPoint> combinedDesignPoint = combiner->combineDesignPoints(combineMethodType, designPoints, selfCorrelationMatrix, progress);

            applyCorrelation(designPoints, correlationMatrix, combinedDesignPoint.get());

            return combinedDesignPoint;
        }

        std::shared_ptr<DesignPoint> DesignPointCombiner::combineDesignPointsExcluding(
            std::vector<std::shared_ptr<Statistics::Scenario>>& scenarios,
            std::vector<std::shared_ptr<DesignPoint>>& designPoints,
            const std::shared_ptr<Statistics::CorrelationMatrix>& correlationMatrix) const
        {
            const std::unique_ptr<ExcludingCombiner> combiner = getExcludingCombiner();
            std::shared_ptr<DesignPoint> combinedDesignPoint = combiner->combineExcludingDesignPoints(scenarios, designPoints);

            applyCorrelation(designPoints, correlationMatrix, combinedDesignPoint.get());

            return combinedDesignPoint;
        }

        void DesignPointCombiner::applyCorrelation(std::vector<std::shared_ptr<DesignPoint>>& designPoints,
                                                   std::shared_ptr<Statistics::BaseCorrelation> correlationMatrix,
                                                   DesignPoint* combinedDesignPoint)
        {
            if (correlationMatrix != nullptr && !correlationMatrix->IsIdentity())
            {
                std::vector<std::shared_ptr<Statistics::Stochast>> stochasts = DesignPoint::getUniqueStochasts(designPoints);
                Models::UConverter uConverter = Models::UConverter(stochasts, correlationMatrix);
                uConverter.initializeForRun();

                std::shared_ptr<Models::Sample> sample = combinedDesignPoint->getSample();
                std::shared_ptr<Models::StochastPoint> stochastPoint = uConverter.GetStochastPoint(sample, combinedDesignPoint->Beta);

                for (size_t i = 0; i < combinedDesignPoint->Alphas.size(); i++)
                {
                    combinedDesignPoint->Alphas[i]->AlphaCorrelated = stochastPoint->Alphas[i]->AlphaCorrelated;
                    combinedDesignPoint->Alphas[i]->X = stochastPoint->Alphas[i]->X;
                }
            }
        }

        std::unique_ptr<ExcludingCombiner> DesignPointCombiner::getExcludingCombiner() const
        {
            switch (excludingCombinerType)
            {
            case ExcludingCombinerType::WeightedSum: return std::make_unique<WeightedSumCombiner>();
            case ExcludingCombinerType::HohenbichlerExcluding: return std::make_unique<HohenbichlerExcludingCombiner>();
            default: throw probLibException("Excluding combiner type");
            }
        }

        std::string DesignPointCombiner::getCombineTypeString(combineAndOr type)
        {
            switch (type)
            {
            case combineAndOr::combOr: return "series";
            case combineAndOr::combAnd: return "parallel";
            default: throw probLibException("Combine type");
            }
        }

        combineAndOr DesignPointCombiner::getCombineType(const std::string& method)
        {
            if (method == "series") return combineAndOr::combOr;
            else if (method == "parallel") return combineAndOr::combAnd;
            else throw probLibException("Combine type");
        }

        std::string DesignPointCombiner::getCombinerMethodString(CombinerType type)
        {
            switch (type)
            {
            case CombinerType::Hohenbichler: return "hohenbichler";
            case CombinerType::ImportanceSampling: return "importance_sampling";
            case CombinerType::DirectionalSampling: return "directional_sampling";
            case CombinerType::HohenbichlerForm: return "hohenbichler_form";
            default: throw probLibException("Combiner method");
            }
        }

        CombinerType DesignPointCombiner::getCombinerMethod(const std::string& method)
        {
            if (method == "hohenbichler") return CombinerType::Hohenbichler;
            else if (method == "importance_sampling") return CombinerType::ImportanceSampling;
            else if (method == "directional_sampling") return CombinerType::DirectionalSampling;
            else if (method == "hohenbichler_form") return CombinerType::HohenbichlerForm;
            else throw probLibException("Combiner method type");
        }

        std::string DesignPointCombiner::getExcludingCombinerMethodString(ExcludingCombinerType type)
        {
            switch (type)
            {
            case ExcludingCombinerType::HohenbichlerExcluding: return "hohenbichler";
            case ExcludingCombinerType::WeightedSum: return "weighted_sum";
            default: throw probLibException("Excluding combiner method");
            }
        }

        ExcludingCombinerType DesignPointCombiner::getExcludingCombinerMethod(const std::string& method)
        {
            if (method == "hohenbichler_excluding") return ExcludingCombinerType::HohenbichlerExcluding;
            else if (method == "weighted_sum") return ExcludingCombinerType::WeightedSum;
            else throw probLibException("Excluding combiner method type");
        }

    }
}

