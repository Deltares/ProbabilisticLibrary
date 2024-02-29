#include "RandomProvider.h"

#include "../../Deltares.Probabilistic/Math/Randomizers/ModifiedKnuthSubtractiveRandomValueGenerator.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			void RandomProvider::initializeInitializeDelegate()
			{
				ManagedInitializeRandomDelegate^ fp = gcnew ManagedInitializeRandomDelegate(RandomProvider::initialize);
				System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				InitializeRandomDelegate functionPointer = static_cast<InitializeRandomDelegate>(callbackPtr.ToPointer());

				Numeric::ModifiedKnuthSubtractiveRandomValueGenerator::setInitializeDelegate(functionPointer);
			}

			void RandomProvider::initializeNextDelegate()
			{
				ManagedNextRandomDelegate^ fp = gcnew ManagedNextRandomDelegate(RandomProvider::next);
				System::Runtime::InteropServices::GCHandle gch = System::Runtime::InteropServices::GCHandle::Alloc(fp);

				System::IntPtr callbackPtr = System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(fp);
				NextRandomDelegate functionPointer = static_cast<NextRandomDelegate>(callbackPtr.ToPointer());

				Numeric::ModifiedKnuthSubtractiveRandomValueGenerator::setNextDelegate(functionPointer);
			}

			void RandomProvider::initialize()
			{
				if (!initialized)
				{
					initialized = true;
					initializeInitializeDelegate();
					initializeNextDelegate();
				}
			}
		}
	}
}

