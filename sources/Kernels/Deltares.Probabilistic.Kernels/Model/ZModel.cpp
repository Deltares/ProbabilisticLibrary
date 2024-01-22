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

		void ZModel::invoke(Sample* sample)
		{
			this->zDelegate(sample);
		}

		void ZModel::invoke(std::vector<Sample*> samples)
		{
			if (zMultipleDelegate == nullptr)
			{
				#pragma omp parallel for
				for (int i = 0; i < samples.size(); i++)
				{
					invoke(samples[i]);
				}
			}
			else
			{
				Sample** sampleList = new Sample * [samples.size()];

				for (int i = 0; i < samples.size(); i++)
				{
					sampleList[i] = samples[i];
				}

				this->zMultipleDelegate(sampleList, samples.size());
			}
		}
	}
}



