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
#pragma once
#include <cstddef>

namespace Deltares
{
    namespace Utils
    {
        namespace Wrappers
        {
            enum class ListOperationType { Add, Remove, Clear };

            generic <typename U> delegate void ListCallBack(ListOperationType listOperationType, U item);

            generic <typename T> ref class CallBackList : System::Collections::Generic::IList<T>, System::Collections::IList
            {
            public:
                CallBackList()
                {
                }

                CallBackList(ListCallBack<T>^ callBack)
                {
                    this->callBack = callBack;
                }

                void SetCallBack(ListCallBack<T>^ callBack)
                {
                    this->callBack = callBack;
                }

                virtual System::Collections::IEnumerator^ GetEnumerator() = System::Collections::IEnumerable::GetEnumerator
                {
                    return list.GetEnumerator();
                }

                virtual System::Collections::Generic::IEnumerator<T>^ GetEnumeratorGeneric() = System::Collections::Generic::IEnumerable<T>::GetEnumerator
                {
                    return list.GetEnumerator();
                }

                virtual void Add(T item)
                {
                    list.Add(item);
                    callBack(ListOperationType::Add, item);
                };

                virtual void Clear()
                {
                    if (list.Count > 0)
                    {
                        callBack(ListOperationType::Clear, list[0]);
                        list.Clear();
                    }
                };

                virtual bool Contains(T item)
                {
                    return list.Contains(item);
                };

                virtual void CopyTo(array<T>^ array, int arrayIndex)
                {
                    list.CopyTo(array, arrayIndex);
                };

                virtual bool Remove(T item)
                {
                    callBack(ListOperationType::Remove, item);
                    return list.Remove(item);
                };

                property int Count
                {
                    virtual int get() { return list.Count; }
                }

                property bool IsReadOnly
                {
                    virtual bool get() { return false; }
                }

                virtual int IndexOf(T item)
                {
                    return list.IndexOf(item);
                };

                virtual void Insert(int index, T item)
                {
                    list.Insert(index, item);
                    callBack(ListOperationType::Add, item);
                };

                virtual void RemoveAt(int index)
                {
                    callBack(ListOperationType::Remove, list[index]);
                    list.RemoveAt(index);
                }

                property T default[int]
                {
                    virtual T get(int index)
                    {
                        return list[index];
                    };
                    virtual void set(int index, T item)
                    {
                        callBack(ListOperationType::Remove, list[index]);
                        list[index] = item;
                        callBack(ListOperationType::Add, item);
                    };
                }

                // IList implementation

                property Object^ IListItems[int]
                {
                    virtual Object ^ get(int index) = System::Collections::IList::default::get { return this[index]; };
                    virtual void set(int index, Object^ item) = System::Collections::IList::default::set{ this[index] = static_cast<T>(item); };
                }

                virtual void CopyTo(System::Array^ array, int index)
                {
                    System::Array::Copy(this->list.ToArray(), 0, array, index, this->Count);
                }

                property Object^ SyncRoot
                {
                    virtual Object^ get() { return nullptr; }
                }

                property bool IsSynchronized
                {
                    virtual bool get() { return false; }
                }

                virtual int Add(Object^ value)
                {
                    this->Add(static_cast<T>(value));
                    return this->Count;
                }

                virtual bool Contains(Object^ value)
                {
                    return this->Contains(static_cast<T>(value));
                }

                virtual int IndexOf(Object^ value)
                {
                    return this->IndexOf(static_cast<T>(value));
                }

                virtual void Insert(int index, Object^ value)
                {
                    this->Insert(index, static_cast<T>(value));
                }

                virtual void Remove(Object^ value)
                {
                    this->list.Remove(static_cast<T>(value));
                }

                property bool IsFixedSize
                {
                    virtual bool get() { return false; }
                }

            private:
                System::Collections::Generic::List<T> list = gcnew System::Collections::Generic::List<T>();
                ListCallBack<T>^ callBack;
            };
        }
    }
}

