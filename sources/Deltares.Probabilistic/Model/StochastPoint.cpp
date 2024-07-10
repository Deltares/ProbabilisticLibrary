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

