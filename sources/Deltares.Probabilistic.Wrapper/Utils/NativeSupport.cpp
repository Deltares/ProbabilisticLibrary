// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include "NativeSupport.h"

namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            std::string NativeSupport::toNative(System::String^ text)
            {
                text = escape(text);

                const char* chars = (const char*)(System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text)).ToPointer();
                std::string os = chars;
                System::Runtime::InteropServices::Marshal::FreeHGlobal(System::IntPtr((void*)chars));

                return os;
            }

            System::String^ NativeSupport::toManaged(std::string text)
            {
                System::String^ result = gcnew System::String(text.c_str());

                return unescape(result);
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

            std::vector<int> NativeSupport::toNative(array<int>^ values)
            {
                std::vector<int> nValues(values->Length);

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

            System::String^ NativeSupport::escape(System::String^ text)
            {
                System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder(16);
                for (int i = 0; i < text->Length; i++)
                {
                    if (text[i] < System::SByte::MaxValue)
                    {
                        sb->Append(text[i]);
                    }
                    else
                    {
                        sb->Append("\\u")->Append(((int)text[i]).ToString("x4"));
                    }
                }
                return sb->ToString();
            }

            System::String^ NativeSupport::unescape(System::String^ text)
            {
                const int hexaLength = 6;

                if (text->Length < hexaLength)
                {
                    return text;
                }
                else
                {
                    System::Text::StringBuilder^ sb = gcnew System::Text::StringBuilder();

                    int maxIndexHexaChar = text->Length - hexaLength;

                    for (int i = 0; i < text->Length; i++)
                    {
                        if (i <= maxIndexHexaChar && text[i] == '\\' && text[i + 1] == 'u')
                        {
                            System::String^ hexadecimalCode = text->Substring(i + 2, 4);
                            wchar_t c = System::Convert::ToChar(System::Convert::ToUInt32(hexadecimalCode, 16));
                            sb->Append(c);
                            i = i + hexaLength - 1;
                        }
                        else
                        {
                            sb->Append(text[i]);
                        }
                    }

                    return sb->ToString();
                }
            }
        }
    }
}

