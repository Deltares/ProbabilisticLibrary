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

			intptr_t NativeSupport::toNativeObject(System::Object^ object)
			{
				if (object == nullptr)
				{
					return 0;
				}
				else 
				{
					System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(object);
					System::IntPtr parameter = (System::IntPtr)handle;

					// call WinAPi and pass the parameter here
					// then free the handle when not needed:
					//handle.Free();

					void* p = parameter.ToPointer();

					intptr_t t = (intptr_t) p;
					return t;
				}
			}

			System::Object^ NativeSupport::toManagedObject(intptr_t pointer)
			{
				if (pointer == 0)
				{
					return nullptr;
				}
				else 
				{
					void* address = (void*) pointer;

					System::IntPtr^ parameter = gcnew System::IntPtr(address);
					System::Runtime::InteropServices::GCHandle handle = safe_cast<System::Runtime::InteropServices::GCHandle>(*parameter);

					System::Object^ object = handle.Target;
					return object;
				}
			}

		}
	}
}

