using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

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

