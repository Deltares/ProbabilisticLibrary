#pragma once
#include <limits>
#include <vector>

#include "Sample.h"

namespace Deltares
{
	namespace Models
	{
		typedef double(__stdcall* ZDelegate) (Sample*);

		typedef double(__stdcall* ZMultipleDelegate) (Sample**, int count);

		class ZModel
		{
		private:
			ZDelegate zDelegate = nullptr;
			ZMultipleDelegate zMultipleDelegate = nullptr;
			int maxProcesses = 1;

		public:
			ZModel(ZDelegate zDelegate, ZMultipleDelegate zMultipleDelegate = nullptr)
			{
				this->zDelegate = zDelegate;
				this->zMultipleDelegate = zMultipleDelegate;
			}

			void setMaxProcesses(int maxProcesses);

			void invoke(Sample* sample);

			void invoke(std::vector<Sample*> samples);
		};
	}
}

