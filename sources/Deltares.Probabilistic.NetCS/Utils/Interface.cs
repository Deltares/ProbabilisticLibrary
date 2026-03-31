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
using System.Runtime.InteropServices;
using System.Text;
using Deltares.Probabilistic.Model;
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

        public static int Create(string className)
        {
            try
            {
                return NativeInterface.Create(Utf8(className));
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine($"Error: {ex.Message}");
                throw;
            }
        }

        public static void Destroy(int id)
        {
            NativeInterface.Destroy(id);
        }

        public static double GetValue(int id, string property)
        {
            return NativeInterface.GetValue(id, Utf8(property));
        }

        public static void SetValue(int id, string property, double value)
        {
            NativeInterface.SetValue(id, Utf8(property), value);
        }

        public static int GetIntValue(int id, string property)
        {
            return NativeInterface.GetIntValue(id, Utf8(property));
        }

        public static void SetIntValue(int id, string property, int value)
        {
            NativeInterface.SetIntValue(id, Utf8(property), value);
        }

        public static int GetIdValue(int id, string property)
            => NativeInterface.GetIdValue(id, Utf8(property));

        public static double GetIntArgValue(int id, int arg, string property)
            => NativeInterface.GetIntArgValue(id, arg, Utf8(property));

        public static void SetIntArgValue(int id, int arg, string property, double value)
            => NativeInterface.SetIntArgValue(id, arg, Utf8(property), value);

        public static bool GetBoolValue(int id, string property)
            => NativeInterface.GetBoolValue(id, Utf8(property));

        public static void SetBoolValue(int id, string property, bool value)
            => NativeInterface.SetBoolValue(id, Utf8(property), value);

        public static string GetStringValue(int id, string property)
        {
            byte[] prop = Utf8(property);
            int size = NativeInterface.GetStringLength(id, prop);

            byte[] buffer = new byte[size + 1];
            NativeInterface.GetStringValue(id, prop, buffer, (UIntPtr)buffer.Length);

            return Encoding.UTF8.GetString(buffer).TrimEnd('\0');
        }

        public static string GetIndexedStringValue(int id, string property, int index)
        {
            byte[] prop = Utf8(property);
            int size = NativeInterface.GetIndexedStringLength(id, prop, index);

            byte[] buffer = new byte[size + 1];
            NativeInterface.GetIndexedStringValue(id, prop, index, buffer, (UIntPtr)buffer.Length);

            return Encoding.UTF8.GetString(buffer).TrimEnd('\0');
        }

        public static void SetStringValue(int id, string property, string value)
            => NativeInterface.SetStringValue(id, Utf8(property), Utf8(value));


        public static void FillArrayValue(int id, string property, double[] values)
            => NativeInterface.FillArrayValue(id, Utf8(property), values, (uint)values.Length);

        public static void SetArrayValue(int id, string property, double[] values)
        {
            IntPtr ptr = Marshal.AllocHGlobal(sizeof(double) * values.Length);
            try
            {
                Marshal.Copy(values, 0, ptr, values.Length);
                NativeInterface.SetArrayValue(id, Utf8(property), ptr, (uint)values.Length);
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
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }


        public static double[] GetArrayValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            double[] values = new double[count];

            for (int i = 0; i < count; i++)
                values[i] = NativeInterface.GetIndexedValue(id, Utf8(property), i);

            return values;
        }

        public static int[] GetArrayIntValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            int[] values = new int[count];

            for (int i = 0; i < count; i++)
                values[i] = NativeInterface.GetIndexedIntValue(id, Utf8(property), i);

            return values;
        }

        public static int[] GetArrayIdValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            int[] values = new int[count];

            for (int i = 0; i < count; i++)
                values[i] = NativeInterface.GetIndexedIdValue(id, Utf8(property), i);

            return values;
        }

        public static string[] GetArrayStringValue(int id, string property)
        {
            int count = GetIntValue(id, property + "_count");
            string[] values = new string[count];

            for (int i = 0; i < count; i++)
                values[i] = GetIndexedStringValue(id, property, i);

            return values;
        }

        public static void SetArrayIntValue(int id, string property, int[] values)
        {
            IntPtr ptr = Marshal.AllocHGlobal(sizeof(int) * values.Length);
            try
            {
                Marshal.Copy(values, 0, ptr, values.Length);
                NativeInterface.SetArrayIntValue(id, Utf8(property), ptr, (uint)values.Length);
            }
            finally
            {
                Marshal.FreeHGlobal(ptr);
            }
        }

        public static double GetArgValue(int id, string property, double arg)
            => NativeInterface.GetArgValue(id, Utf8(property), arg);

        public static void SetArgValue(int id, string property, double arg, double value)
            => NativeInterface.SetArgValue(id, Utf8(property), arg, value);

        public static double GetIndexedValue(int id, string property, int index)
            => NativeInterface.GetIndexedValue(id, Utf8(property), index);

        public static void SetIndexedValue(int id, string property, int index, double value)
            => NativeInterface.SetIndexedValue(id, Utf8(property), index, value);

        public static double GetIndexedIndexedValue(int id, string property, int index1, int index2)
            => NativeInterface.GetIndexedIndexedValue(id, Utf8(property), index1, index2);

        public static void SetIndexedIndexedValue(int id, string property, int index1, int index2, double value)
            => NativeInterface.SetIndexedIndexedValue(id, Utf8(property), index1, index2, value);

        public static void SetIndexedIndexedIntValue(int id, string property, int index1, int index2, int value)
            => NativeInterface.SetIndexedIndexedIntValue(id, Utf8(property), index1, index2, value);

        public static void SetCallback(int id, string property, Callback callBack)
        {
            try
            {
                keepCallBack = callBack;
                NativeInterface.SetCallBack(id, property, callBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetMultipleCallback(int id, string property, NativeInterface.MultipleCallback multipleCallBack)
        {
            try
            {
                keepMultipleCallBack = multipleCallBack;
                NativeInterface.SetMultipleCallBack(id, property, multipleCallBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetEmptyCallback(int id, string property, EmptyCallback emptyCallBack)
        {
            try
            {
                keepEmptyCallBack = emptyCallBack;
                NativeInterface.SetEmptyCallBack(id, property, emptyCallBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetModelSampleCallback(int id, string property, ModelSampleCallback modelSampleCallBack)
        {
            try
            {
                keepModelSampleCallback = modelSampleCallBack;
                NativeInterface.SetModelSampleCallback(id, property, modelSampleCallBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetMultipleModelSampleCallback(int id, string property, MultipleModelSampleCallback multipleModelSampleCallBack)
        {
            try
            {
                keepMultipleModelSampleCallback = multipleModelSampleCallBack;
                NativeInterface.SetMultipleModelSampleCallback(id, property, multipleModelSampleCallBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetProgressCallbacks(int id, ProgressCallBack progressCallBack, DetailedProgressCallBack detailedCallBack, TextualProgressCallBack textualCallBack)
        {
            try
            {
                keepProgressCallBack = progressCallBack;
                keepDetailedCallBack = detailedCallBack;
                keepTextualCallBack = textualCallBack;
                NativeInterface.SetProgressCallBacks(id, progressCallBack, detailedCallBack, textualCallBack);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static IntPtr GetCallback(int id, string property)
        {
            try
            {
                return NativeInterface.GetCallBack(id, property);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
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