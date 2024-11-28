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
#include "StochastPoint.h"

#include <unordered_set>

#include "ModelSample.h"

namespace Deltares
{
    namespace Models
    {
        std::shared_ptr<Sample> StochastPoint::getSample()
        {
            std::shared_ptr<Sample> sample = std::make_shared<Sample>(Alphas.size());

            for (size_t i = 0; i < Alphas.size(); i++)
            {
                sample->Values[i] = this->Alphas[i]->U;
            }

            return sample;
        }

        std::shared_ptr<Sample> StochastPoint::getSampleForStochasts(std::vector<std::shared_ptr<Statistics::Stochast>> stochasts)
        {
            std::unordered_set<size_t> usedIndices;

            std::shared_ptr<Sample> sample = std::make_shared<Sample>(stochasts.size());

            for (size_t i = 0; i < stochasts.size(); i++)
            {
                sample->Values[i] = 0;

                for (size_t j = 0; j < this->Alphas.size(); j++)
                {
                    if (this->Alphas[j]->Stochast == stochasts[i] && !usedIndices.contains(j))
                    {
                        sample->Values[i] = this->Alphas[j]->U;
                        usedIndices.insert(j);
                        break;
                    }
                }
            }

            return sample;
        }

        std::shared_ptr<ModelSample> StochastPoint::getModelSample()
        {
            std::vector<double> values(Alphas.size());
            for (size_t i = 0; i < Alphas.size(); i++)
            {
                values[i] = this->Alphas[i]->X;
            }

            return std::make_shared<ModelSample>(values);
        }

        std::vector<std::shared_ptr<StochastPointAlpha>> StochastPoint::getAlphas(const std::vector<std::shared_ptr<Statistics::Stochast>>& stochasts) const
        {
            std::vector<std::shared_ptr<StochastPointAlpha>> alphas;
            std::unordered_set<std::shared_ptr<StochastPointAlpha>> usedAlphas;

            for (std::shared_ptr<Statistics::Stochast> stochast : stochasts)
            {
                bool found = false;

                for (const auto& alpha : this->Alphas)
                {
                    if (alpha->Stochast == stochast && !usedAlphas.contains(alpha))
                    {
                        alphas.push_back(alpha);
                        usedAlphas.insert(alpha);
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    auto empty = std::make_shared<StochastPointAlpha>();
                    empty->Stochast = stochast;
                    alphas.push_back(empty);
                }
            }

            return alphas;
        }


        std::shared_ptr<StochastPointAlpha> StochastPoint::getAlpha(std::shared_ptr<Statistics::Stochast> stochast)
        {
            for (std::shared_ptr<StochastPointAlpha> alpha : this->Alphas)
            {
                if (alpha->Stochast == stochast)
                {
                    return alpha;
                }
            }

            return nullptr;
        }

        std::shared_ptr<StochastPoint> StochastPoint::getCopy(double beta, double alphaMargin)
        {
            std::shared_ptr<StochastPoint> copy = std::make_shared<StochastPoint>();

            copy->Beta = beta;

            for (std::shared_ptr<StochastPointAlpha> alpha : this->Alphas)
            {
                if (std::fabs(alpha->Alpha) > alphaMargin)
                {
                    copy->Alphas.push_back(alpha->clone());
                }
            }

            return copy;
        }

        void StochastPoint::updateInfluenceFactors()
        {
            double sum = 0;
            for (size_t i = 0; i < Alphas.size(); i++)
            {
                sum += this->Alphas[i]->Alpha * this->Alphas[i]->Alpha;
            }

            if (sum > 0)
            {
                for (size_t j = 0; j < this->Alphas.size(); j++)
                {
                    Alphas[j]->InfluenceFactor = Alphas[j]->Alpha * Alphas[j]->Alpha / sum;
                }
            }
        }
    }
}

