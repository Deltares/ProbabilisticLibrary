#include "ZModel.h"
#include <omp.h>


namespace Deltares
{
	namespace Models
	{
		void ZModel::setMaxProcesses(int maxProcesses)
		{
			this->maxProcesses = maxProcesses;
			omp_set_num_threads(maxProcesses);
		}

		void ZModel::invoke(std::shared_ptr<Sample> sample)
		{
			this->zLambda(sample);
		}

		void ZModel::invoke(std::vector<std::shared_ptr<Sample>> samples)
		{
			if (zMultipleLambda == nullptr)
			{
				#pragma omp parallel for
				for (int i = 0; i < samples.size(); i++)
				{
					invoke(samples[i]);
				}
			}
			else
			{
				this->zMultipleLambda(samples);
			}
		}
	}
}



