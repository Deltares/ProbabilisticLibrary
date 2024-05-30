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
				static System::Collections::Generic::List<Object^>^ managedObjects = gcnew System::Collections::Generic::List<Object^>();
			public:
				static std::string toNative(System::String^ text);
				static System::String^ toManaged(std::string text);

				static std::vector<double> toNative(array<double>^ values);
				static array<double>^ toManaged(std::vector<double>& values, int count);
				static array<double>^ toManaged(const std::vector<double> & values);

				static intptr_t toNativeObject(System::Object^ object);
				static System::Object^ toManagedObject(intptr_t pointer);

                static void releaseManagedObjects();
                static void releaseManagedObject(System::Object^ object);
                static int getManagedObjectsCount();
			private:
                static bool initialized = false;
                static void initialize();
                static void releaseTag(intptr_t tag);
                static Deltares::Utils::ReleaseTagLambda getReleaseTagLambda();
			};
		}
	}
}

