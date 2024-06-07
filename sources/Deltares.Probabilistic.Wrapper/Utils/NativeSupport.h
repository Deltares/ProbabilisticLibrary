#pragma once
#include <string>
#include <vector>

#include "../../Deltares.Probabilistic/Utils/TagSupport.h"

namespace Deltares
{
	namespace Utils
	{
		namespace Wrappers
		{
			ref class NativeSupport
			{
			public:
				static std::string toNative(System::String^ text);
				static System::String^ toManaged(std::string text);

				static std::vector<double> toNative(array<double>^ values);
				static array<double>^ toManaged(std::vector<double>& values, int count);
				static array<double>^ toManaged(const std::vector<double> & values);
			};
		}
	}
}

