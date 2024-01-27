#pragma once
#include <limits>
#include <vector>
#include <functional>

#include "Sample.h"

namespace Deltares
{
	namespace Models
	{
#ifdef _WIN32
		typedef double(__stdcall* ZDelegate) (Sample*);
		typedef double(__stdcall* ZMultipleDelegate) (Sample**, int count);
#else
		typedef double(* ZDelegate) (Sample*);
		typedef double(* ZMultipleDelegate) (Sample**, int count);
#endif // _WIN32

		typedef std::function<double(Sample*)> ZLambda;

		class ZModel
		{
		private:
			ZDelegate zDelegate = nullptr;
			ZMultipleDelegate zMultipleDelegate = nullptr;
			ZLambda zLambda = nullptr;
			int maxProcesses = 1;

		public:
			ZModel(ZDelegate zDelegate, ZMultipleDelegate zMultipleDelegate = nullptr)
			{
				this->zDelegate = zDelegate;
				this->zMultipleDelegate = zMultipleDelegate;
			}

			ZModel(ZLambda zLambda)
			{
				this->zLambda = zLambda;
			}

			void setMaxProcesses(int maxProcesses);

			void invoke(Sample* sample);

			void invoke(std::vector<Sample*> samples);
		};
	}
}

