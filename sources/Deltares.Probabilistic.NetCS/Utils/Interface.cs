using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using static Deltares.Probabilistic.NetCS.Utils.NativeInterface;

namespace Deltares.Probabilistic.NetCS.Utils
{
    public static class Interface
    {
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


        // ------------------ Array operations ------------------

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


        // ------------------ Array getters ------------------

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

        public static void SetCallback(int id, string property, Callback cb)
        {
            try
            {
                NativeInterface.SetCallBack(id, property, cb);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetMultipleCallback(int id, string property, NativeInterface.MultipleCallback cb)
        {
            try
            {
                NativeInterface.SetMultipleCallBack(id, property, cb);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                throw;
            }
        }

        public static void SetEmptyCallback(int id, string property, EmptyCallback cb)
        {
            try
            {
                NativeInterface.SetEmptyCallBack(id, property, cb);
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

        public static void ExecuteMethod(int id, string method)
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

    public static class NativeInterface
    {
        // void* callback(double* in1, int n, double* out1)
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr Callback(
            IntPtr inputArray,
            int count,
            IntPtr outputArray
        );

        // void* callback(int n1, double** in2, int n2, double** out2)
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr MultipleCallback(
            int count1,
            IntPtr inputMatrix,
            int count2,
            IntPtr outputMatrix
        );

        // void* callback()
        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate IntPtr EmptyCallback();

        // Import the native function from your library
        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int Create(byte[] className);

        // Import the native function from your library
        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Destroy(int id);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetValue(int id, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetValue(int id, byte[] property, double value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetIntValue(int id, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetIntValue(int id, byte[] property, int value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetIdValue(int id, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetIntArgValue(int id, int arg, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetIntArgValue(int id, int arg, byte[] property, double value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern bool GetBoolValue(int id, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetBoolValue(int id, byte[] property, bool value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetStringLength(int id, byte[] property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetStringValue(int id, byte[] property, byte[] buffer, UIntPtr bufferSize);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetIndexedStringLength(int id, byte[] property, int index);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetIndexedStringValue(int id, byte[] property, int index, byte[] buffer,
            UIntPtr bufferSize);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetStringValue(int id, byte[] property, byte[] value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void FillArrayValue(int id, byte[] property, double[] values, uint size);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetArrayValue(int id, byte[] property, IntPtr values, uint size);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetArgValues(int id, byte[] property, IntPtr inputValues, uint size,
            double[] outputValues);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetIndexedValue(int id, byte[] property, int index);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetIndexedValue(int id, byte[] property, int index, double value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetIndexedIndexedValue(int id, byte[] property, int index1, int index2);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetIndexedIndexedValue(int id, byte[] property, int index1, int index2, double value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetIndexedIndexedIntValue(int id, byte[] property, int index1, int index2, int value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern double GetArgValue(int id, byte[] property, double arg);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetArgValue(int id, byte[] property, double arg, double value);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetIndexedIntValue(int id, byte[] property, int index);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetIndexedIdValue(int id, byte[] property, int index);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetArrayIntValue(int id, byte[] property, IntPtr values, uint size);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCallBack(int id, string property, Callback cb);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetMultipleCallBack(int id, string property, MultipleCallback cb);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetEmptyCallBack(int id, string property, EmptyCallback cb);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr GetCallBack(int id, string property);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void Execute(int id, string method);

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetExceptionLength();

        [DllImport("CWrapper.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetException(IntPtr buffer, UIntPtr size);
    }
}