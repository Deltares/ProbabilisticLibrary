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
				static System::Collections::Generic::List<Object^>^ managedObjects = gcnew System::Collections::Generic::List<Object^>();
			public:
				static std::string toNative(System::String^ text);
				static System::String^ toManaged(std::string text);

				static double* toNative(array<double>^ values);
				static array<double>^ toManaged(double* values, int count);

				static intptr_t toNativeObject(System::Object^ object);
				static System::Object^ toManagedObject(intptr_t pointer);

				static void releaseManagedObjects();
			};
		}
	}
}

