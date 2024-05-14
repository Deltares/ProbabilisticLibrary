#include "StochastPoint.h"

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

        std::shared_ptr<ModelSample> StochastPoint::getModelSample()
        {
            std::vector<double> values(Alphas.size());
            for (size_t i = 0; i < Alphas.size(); i++)
            {
                values[i] = this->Alphas[i]->X;
            }

            return std::make_shared<ModelSample>(values);
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

