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
using Deltares.Probabilistic.Model;
using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using static Deltares.Probabilistic.Utils.NativeInterface;

namespace Deltares.Probabilistic.Utils
{
    public static class Interface
    {
        // Keep callbacks to prevent them garbage collected
        private static Callback keepCallBack = null;
        private static MultipleCallback keepMultipleCallBack = null;
        private static EmptyCallback keepEmptyCallBack = null;
        private static ModelSampleCallback keepModelSampleCallback = null;
        private static MultipleModelSampleCallback keepMultipleModelSampleCallback = null;
        private static ProgressCallBack keepProgressCallBack = null;
        private static DetailedProgressCallBack keepDetailedCallBack = null;
        private static TextualProgressCallBack keepTextualCallBack = null;

        private static byte[] Utf8(string s) => Encoding.UTF8.GetBytes(s);

        private static readonly Dictionary<string, bool> canHandleValues = new Dictionary<string, bool>();

        public static int GetNewId()
        {
            int id = NativeInterface.GetNewId();
            CheckException();
            return id;
        }

        public static bool CanHandle(string className)
        {
            if (!canHandleValues.ContainsKey(className))
            {
                canHandleValues[className] = NativeInterface.CanHandle(Utf8(className));
                CheckException();
            }

            return canHandleValues[className];
        }

        public static int Create(string className)
        {
            int id = NativeInterface.Create(Utf8(className));
            CheckException();
            return id;
        }

        public static void Destroy(int id)
        {
            NativeInterface.Destroy(id);
            CheckException();
        }

        public static double GetValue(int id, string property)
        {
            double value = NativeInterface.GetValue(id, Utf8(property));
            CheckException();
            return value;
        }

        public static void SetValue(int id, string property, double value)
        {
            NativeInterface.SetValue(id, Utf8(property), value);
            CheckException();
        }

        public static int GetIntValue(int id, string property)
        {
            int value = NativeInterface.GetIntValue(id, Utf8(property));
            CheckException();
            return value;
        }

        public static void SetIntValue(int id, string property, int value)
        {
            NativeInterface.SetIntValue(id, Utf8(property), value);
            CheckException();
        }

        public static int GetIdValue(int id, string property)
        {
            int value = NativeInterface.GetIdValue(id, Utf8(property));
            CheckException();
            return value;
        }

        public static double GetIntArgValue(int id, int arg, string property)
        {
            double value = NativeInterface.GetIntArgValue(id, arg, Utf8(property));
            CheckException();
            return value;
        }

        public static void SetIntArgValue(int id, int arg, string property, double value)
        {
            NativeInterface.SetIntArgValue(id, arg, Utf8(property), value);
            CheckException();
        }

        public static bool GetBoolValue(int id, string property)
        {
            bool value = NativeInterface.GetBoolValue(id, Utf8(property));
            CheckException();
            return value;
        }

        public static void SetBoolValue(int id, string property, bool value)
        {
            NativeInterface.SetBoolValue(id, Utf8(property), value);
            CheckException();
        }

        public static string GetStringValue(int id, string property)
        {
            byte[] prop = Utf8(property);
            int size = NativeInterface.GetStringLength(id, prop);
            CheckException();

            byte[] buffer = new byte[size + 1];
            NativeInterface.GetStringValue(id, prop, buffer, (UIntPtr)buffer.Length);
            CheckException();

            return Encoding.UTF8.GetString(buffer).TrimEnd('\0');
        }

        public static int GetIndexedIntValue(int id, string property, int index)
        {
            int value = NativeInterface.GetIndexedIntValue(id, Utf8(property), index);
            CheckException();
            return value;
        }

        public static int GetIndexedIdValue(int id, string property, int index)
        {
            int value = NativeInterface.GetIndexedIdValue(id, Utf8(property), index);
            CheckException();
            return value;
        }

        public static string GetIndexedStringValue(int id, string property, int index)
        {
            byte[] prop = Utf8(property);
            int size = NativeInterface.GetIndexedStringLength(id, prop, index);
            CheckException();

            byte[] buffer = new byte[size + 1];
            NativeInterface.GetIndexedStringValue(id, prop, index, buffer, (UIntPtr)buffer.Length);
            CheckException();

            return Encoding.UTF8.GetString(buffer).TrimEnd('\0');
        }

        public static void SetStringValue(int id, string property, string value)
        {
            NativeInterface.SetStringValue(id, Utf8(property), Utf8(value));
            CheckException();
        }


        public static void FillArrayValue(int id, string property, double[] values)
        {
            NativeInterface.FillArrayValue(id, Utf8(property), values, (uint)values.Length);
            CheckException();
        }

        public static void SetArrayValue(int id, string property, double[] values)
        {
            IntPtr ptr = Marshal.AllocHGlobal(sizeof(double) * values.Length);
            try
            {
                Marshal.Copy(values, 0, ptr, values.Length);
                NativeInterface.SetArrayValue(id, Utf8(property), ptr, (uint)values.Length);
                CheckException();
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static void GetArgValues(int id, string property, double[] inputValues, double[] outputValues)
        {
            IntPtr ptr = Marshal.AllocHGlobal(sizeof(double) * inputValues.Length);
            try
            {
                Marshal.Copy(inputValues, 0, ptr, inputValues.Length);
                NativeInterface.GetArgValues(id, Utf8(property), ptr, (uint)inputValues.Length, outputValues);
                CheckException();
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }


        public static double[] GetArrayValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            CheckException();

            double[] values = new double[count];

            for (int i = 0; i < count; i++)
            {
                values[i] = NativeInterface.GetIndexedValue(id, Utf8(property), i);
                CheckException();
            }

            return values;
        }

        public static int[] GetArrayIntValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            CheckException();

            int[] values = new int[count];

            for (int i = 0; i < count; i++)
            {
                values[i] = NativeInterface.GetIndexedIntValue(id, Utf8(property), i);
                CheckException();
            }

            return values;
        }

        public static int[] GetArrayIdValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            CheckException();

            int[] values = new int[count];

            for (int i = 0; i < count; i++)
            {
                values[i] = NativeInterface.GetIndexedIdValue(id, Utf8(property), i);
                CheckException();
            }

            return values;
        }

        public static string[] GetArrayStringValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            CheckException();

            string[] values = new string[count];

            for (int i = 0; i < count; i++)
            {
                values[i] = GetIndexedStringValue(id, property, i);
                CheckException();
            }

            return values;
        }

        public static void SetArrayIntValue(int id, string property, int[] values)
        {
            IntPtr ptr = Marshal.AllocHGlobal(sizeof(int) * values.Length);
            try
            {
                Marshal.Copy(values, 0, ptr, values.Length);
                NativeInterface.SetArrayIntValue(id, Utf8(property), ptr, (uint)values.Length);
                CheckException();
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static double GetArgValue(int id, string property, double arg)
        {
            double value = NativeInterface.GetArgValue(id, Utf8(property), arg);
            CheckException();
            return value;
        }

        public static void SetArgValue(int id, string property, double arg, double value)
        {
            NativeInterface.SetArgValue(id, Utf8(property), arg, value);
            CheckException();
        }

        public static double GetIndexedValue(int id, string property, int index)
        {
            double value = NativeInterface.GetIndexedValue(id, Utf8(property), index);
            CheckException();
            return value;
        }

        public static void SetIndexedValue(int id, string property, int index, double value)
        {
            NativeInterface.SetIndexedValue(id, Utf8(property), index, value);
            CheckException();
        }

        public static double GetIndexedIndexedValue(int id, string property, int index1, int index2)
        {
            double value = NativeInterface.GetIndexedIndexedValue(id, Utf8(property), index1, index2);
            CheckException();
            return value;
        }

        public static void SetIndexedIndexedValue(int id, string property, int index1, int index2, double value)
        {
            NativeInterface.SetIndexedIndexedValue(id, Utf8(property), index1, index2, value);
            CheckException();
        }

        public static void SetIndexedIndexedIntValue(int id, string property, int index1, int index2, int value)
        {
            NativeInterface.SetIndexedIndexedIntValue(id, Utf8(property), index1, index2, value);
            CheckException();
        }

        public static void SetCallback(int id, string property, Callback callBack)
        {
            keepCallBack = callBack;
            NativeInterface.SetCallBack(id, property, callBack);
            CheckException();
        }

        public static void SetMultipleCallback(int id, string property, NativeInterface.MultipleCallback multipleCallBack)
        {
            keepMultipleCallBack = multipleCallBack;
            NativeInterface.SetMultipleCallBack(id, property, multipleCallBack);
            CheckException();
        }

        public static void SetEmptyCallback(int id, string property, EmptyCallback emptyCallBack)
        {
            keepEmptyCallBack = emptyCallBack;
            NativeInterface.SetEmptyCallBack(id, property, emptyCallBack);
            CheckException();
        }

        public static void SetModelSampleCallback(int id, string property, ModelSampleCallback modelSampleCallBack)
        {
            keepModelSampleCallback = modelSampleCallBack;
            NativeInterface.SetModelSampleCallback(id, property, modelSampleCallBack);
            CheckException();
        }

        public static void SetMultipleModelSampleCallback(int id, string property, MultipleModelSampleCallback multipleModelSampleCallBack)
        {
            keepMultipleModelSampleCallback = multipleModelSampleCallBack;
            NativeInterface.SetMultipleModelSampleCallback(id, property, multipleModelSampleCallBack);
            CheckException();
        }

        public static void SetProgressCallbacks(int id, ProgressCallBack progressCallBack, DetailedProgressCallBack detailedCallBack, TextualProgressCallBack textualCallBack)
        {
            keepProgressCallBack = progressCallBack;
            keepDetailedCallBack = detailedCallBack;
            keepTextualCallBack = textualCallBack;
            NativeInterface.SetProgressCallBacks(id, progressCallBack, detailedCallBack, textualCallBack);
            CheckException();
        }

        public static void Execute(int id, string method)
        {
            NativeInterface.Execute(id, method);
            CheckException();
        }


        // ------------------------------------------------------------
        // Exception handling
        // ------------------------------------------------------------

        private static void CheckException()
        {
            int size = NativeInterface.GetExceptionLength();
            if (size > 0)
            {
                IntPtr buffer = Marshal.AllocHGlobal(size + 1);

                try
                {
                    NativeInterface.GetException(buffer, (UIntPtr)(size + 1));
                    string message = Marshal.PtrToStringAnsi(buffer);
                    throw new Exception(message);
                }
                finally
                {
                    Marshal.FreeHGlobal(buffer);
                }
            }
        }

    }
}