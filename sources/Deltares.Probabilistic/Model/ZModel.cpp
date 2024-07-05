#include "ZModel.h"
#include <omp.h>

#include "ModelSample.h"
#include "../Utils/probLibException.h"


namespace Deltares
{
	namespace Models
	{
		void ZModel::setMaxProcesses(int maxProcesses)
		{
			this->maxProcesses = maxProcesses;

			if (maxProcesses > 0)
			{
				omp_set_num_threads(maxProcesses);
			}
		}

        void ZModel::releaseCallBacks()
		{
            this->zLambda = nullptr;
            this->zMultipleLambda = nullptr;
		}

		void ZModel::invoke(std::shared_ptr<ModelSample> sample)
		{
            if (this->zLambda == nullptr)
            {
                throw Reliability::probLibException("callback function not set or released");
            }

            sample->threadId = omp_get_thread_num();
            this->zLambda(sample);
		}

		void ZModel::invoke(std::vector<std::shared_ptr<ModelSample>> samples)
		{
			if (zMultipleLambda == nullptr)
			{
				#pragma omp parallel for
				for (int i = 0; i < (int)samples.size(); i++)
				{
                    invoke(samples[i]);
				}
			}
			else
			{
				this->zMultipleLambda(samples);
			}
		}

		double ZModel::getBeta(std::shared_ptr<ModelSample> sample, double beta)
		{
			return this->zBetaLambda(sample, beta);
		}
	}
}



