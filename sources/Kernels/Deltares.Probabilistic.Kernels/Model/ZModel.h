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
		typedef double(__stdcall* ZDelegate) (std::shared_ptr<Sample>);
		typedef double(__stdcall* ZMultipleDelegate) (std::vector<std::shared_ptr<Sample>>);
#else
		typedef double(* ZDelegate) (std::shared_ptr<Sample>);
		typedef double(* ZMultipleDelegate) (std::shared_ptr<Sample>*, int count);
#endif // _WIN32

		typedef std::function<double(std::shared_ptr<Sample>)> ZLambda;

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

			void invoke(std::shared_ptr<Sample> sample);

			void invoke(std::vector<std::shared_ptr<Sample>> samples);
		};
	}
}

