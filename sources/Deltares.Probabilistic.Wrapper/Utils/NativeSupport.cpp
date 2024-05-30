#include "NativeSupport.h"

namespace Deltares
{
	namespace Utils
	{
		namespace Wrappers
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

			std::vector<double> NativeSupport::toNative(array<double>^ values)
			{
				std::vector<double> nValues(values->Length);

				for (int i = 0; i < values->Length; i++)
				{
					nValues[i] = values[i];
				}

				return nValues;
			}

			array<double>^ NativeSupport::toManaged(std::vector<double>& values, int size)
			{
				array<double>^ mValues = gcnew array<double>(size);

				for (int i = 0; i < size; i++)
				{
					mValues[i] = values[i];
				}

				return mValues;
			}

			array<double>^ NativeSupport::toManaged(const std::vector<double> & values)
			{
				array<double>^ mValues = gcnew array<double>(values.size());

				for (int i = 0; i < values.size(); i++)
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
					// keep a reference to the managed object, so that ut is not cleared by the garbage collector
					managedObjects->Add(object);

					System::Runtime::InteropServices::GCHandle handle = System::Runtime::InteropServices::GCHandle::Alloc(object);
					System::IntPtr parameter = (System::IntPtr)handle;
					void* p = parameter.ToPointer();
					intptr_t t = (intptr_t) p;

					return t;

					// do not free the handle, it is used to store the managed object
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

			void NativeSupport::releaseManagedObjects()
			{
				managedObjects->Clear();
			}

            void NativeSupport::releaseManagedObject(System::Object^ object)
			{
                if (object != nullptr)
                {
                    managedObjects->Remove(object);
                }
			}

            int NativeSupport::getManagedObjectsCount()
			{
			    return managedObjects->Count;
			}
		}
	}
}

