#include "NativeSupport.h"
#include "../../Deltares.Probabilistic/Utils/TagSupport.h"

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

            array<double>^ NativeSupport::toManaged(const std::vector<double>& values)
            {
                array<double>^ mValues = gcnew array<double>(values.size());

                for (int i = 0; i < values.size(); i++)
                {
                    mValues[i] = values[i];
                }

                return mValues;
            }
        }
    }
}

