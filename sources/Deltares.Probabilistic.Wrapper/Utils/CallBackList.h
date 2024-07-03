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

            generic <typename T> ref class CallBackList : System::Collections::Generic::IList<T>
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
                };

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
            private:
                System::Collections::Generic::List<T> list = gcnew System::Collections::Generic::List<T>();
                ListCallBack<T>^ callBack;
            };
        }
    }
}

