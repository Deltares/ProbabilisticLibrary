#include "NativeSupport.h"

namespace Deltares
{
	namespace Probabilistic
	{
		namespace Kernels
		{
			std::string NativeSupport::toNative(System::String^ text)
			{
				const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text)).ToPointer();
				std::string os = chars;
				System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));

				return os;
			}

			System::String^ NativeSupport::toManaged(std::string text)
			{
				return gcnew System::String(text.c_str());
			}

			double* NativeSupport::toNative(array<double>^ values)
			{
				double* nValues = new double[values->Length];

				for (int i = 0; i < values->Length; i++)
				{
					nValues[i] = values[i];
				}

				return nValues;
			}

			array<double>^ NativeSupport::toManaged(double* values, int size)
			{
				array<double>^ mValues = gcnew array<double>(size);

				for (int i = 0; i < size; i++)
				{
					mValues[i] = values[i];
				}

				return mValues;
			}
		}
	}
}

