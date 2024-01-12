#pragma once
#include <string>

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			ref class NativeSupport
			{
			public:
				static std::string toNative(System::String^ text);
				static System::String^ toManaged(std::string text);

				static double* toNative(array<double>^ values);
				static array<double>^ toManaged(double* values, int count);
			};
		}
	}
}

