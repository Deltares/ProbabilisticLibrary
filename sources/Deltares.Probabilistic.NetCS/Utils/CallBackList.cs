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
using System;
using System.Collections;
using System.Collections.Generic;

namespace Deltares.Probabilistic.Utils
{
    public enum ListOperationType
    {
        Add,
        Remove,
        Clear
    }

    public delegate void ListCallBack<U>(ListOperationType operation, U item);

    public class CallBackList<T> : IList<T>, IList
    {
        private readonly List<T> list = new List<T>();
        private ListCallBack<T> callBack = null;

        public CallBackList()
        {
        }

        public CallBackList(ListCallBack<T> callBack)
        {
            this.callBack = callBack;
        }

        public void SetCallBack(ListCallBack<T> callBack)
        {
            this.callBack = callBack;
        }

        public IEnumerator<T> GetEnumerator()
        {
            return list.GetEnumerator();
        }

        IEnumerator IEnumerable.GetEnumerator()
        {
            return list.GetEnumerator();
        }

        public void Add(T item)
        {
            list.Add(item);
            callBack?.Invoke(ListOperationType.Add, item);
        }

        public void AddWithoutCallBack(T item)
        {
            list.Add(item);
        }

        public void Clear()
        {
            if (list.Count > 0)
            {
                callBack?.Invoke(ListOperationType.Clear, list[0]);
                list.Clear();
            }
        }

        public bool Contains(T item)
        {
            return list.Contains(item);
        }

        public void CopyTo(T[] array, int arrayIndex)
        {
            list.CopyTo(array, arrayIndex);
        }

        public bool Remove(T item)
        {
            callBack?.Invoke(ListOperationType.Remove, item);
            return list.Remove(item);
        }

        public int Count => list.Count;

        public bool IsReadOnly => false;

        public int IndexOf(T item)
        {
            return list.IndexOf(item);
        }

        public void Insert(int index, T item)
        {
            list.Insert(index, item);
            callBack?.Invoke(ListOperationType.Add, item);
        }

        public void RemoveAt(int index)
        {
            var oldItem = list[index];
            callBack?.Invoke(ListOperationType.Remove, oldItem);
            list.RemoveAt(index);
        }

        public T this[int index]
        {
            get => list[index];
            set
            {
                var oldItem = list[index];
                callBack?.Invoke(ListOperationType.Remove, oldItem);

                list[index] = value;

                callBack?.Invoke(ListOperationType.Add, value);
            }
        }

        // -----------------------------
        // Non-generic IList implementation
        // -----------------------------

        bool IList.IsFixedSize => false;

        bool IList.IsReadOnly => false;

        object ICollection.SyncRoot => null;

        bool ICollection.IsSynchronized => false;

        int IList.Add(object value)
        {
            Add((T)value);
            return Count - 1;
        }

        bool IList.Contains(object value)
        {
            return Contains((T)value);
        }

        int IList.IndexOf(object value)
        {
            return IndexOf((T)value);
        }

        void IList.Insert(int index, object value)
        {
            Insert(index, (T)value);
        }

        void IList.Remove(object value)
        {
            Remove((T)value);
        }

        object IList.this[int index]
        {
            get => this[index];
            set => this[index] = (T)value;
        }

        void ICollection.CopyTo(Array array, int index)
        {
            ((ICollection)list).CopyTo(array, index);
        }
    }
}

