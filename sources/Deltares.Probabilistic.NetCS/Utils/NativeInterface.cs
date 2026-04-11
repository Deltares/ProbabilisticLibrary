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
using System.Reflection;
using System.Runtime.InteropServices;
using Deltares.Probabilistic.Model;

namespace Deltares.Probabilistic.Utils;

public static class NativeInterface
{
    static NativeInterface()
    {
        NativeLibrary.SetDllImportResolver(typeof(NativeInterface).Assembly, ResolveLibrary);
    }

    private static IntPtr ResolveLibrary(string libraryName, Assembly assembly, DllImportSearchPath? path)
    {
        if (libraryName == "CWrapper")
        {
            if (RuntimeInformation.IsOSPlatform(OSPlatform.Windows))
                return NativeLibrary.Load("Deltares.Probabilistic.CWrapper.dll", assembly, path);

            if (RuntimeInformation.IsOSPlatform(OSPlatform.Linux))
                return NativeLibrary.Load("Deltares.Probabilistic.CWrapper.so", assembly, path);
        }

        return IntPtr.Zero;
    }

    // void* callback(double* in1, int n, double* out1)
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate IntPtr Callback(
        IntPtr inputArray,
        int count,
        IntPtr outputArray
    );

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate IntPtr MultipleCallback(
        int count1,
        IntPtr inputMatrix,
        int count2,
        IntPtr outputMatrix
    );

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate IntPtr EmptyCallback();

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ProgressCallBack(double progress);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void DetailedProgressCallBack(int step, int loop, double reliability, double convergence);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void TextualProgressCallBack(int type, [MarshalAs(UnmanagedType.LPStr)] string text);

    // Import the native function from your library
    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int Create(byte[] className);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int CreateWithId(byte[] className, int id);

    // Import the native function from your library
    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Destroy(int id);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern double GetValue(int id, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetValue(int id, byte[] property, double value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetIntValue(int id, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetIntValue(int id, byte[] property, int value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetIdValue(int id, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern double GetIntArgValue(int id, int arg, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetIntArgValue(int id, int arg, byte[] property, double value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern bool GetBoolValue(int id, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetBoolValue(int id, byte[] property, bool value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetStringLength(int id, byte[] property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetStringValue(int id, byte[] property, byte[] buffer, UIntPtr bufferSize);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetIndexedStringLength(int id, byte[] property, int index);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetIndexedStringValue(int id, byte[] property, int index, byte[] buffer,
        UIntPtr bufferSize);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetStringValue(int id, byte[] property, byte[] value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void FillArrayValue(int id, byte[] property, double[] values, uint size);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetArrayValue(int id, byte[] property, IntPtr values, uint size);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetArgValues(int id, byte[] property, IntPtr inputValues, uint size,
        double[] outputValues);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern double GetIndexedValue(int id, byte[] property, int index);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetIndexedValue(int id, byte[] property, int index, double value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern double GetIndexedIndexedValue(int id, byte[] property, int index1, int index2);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetIndexedIndexedValue(int id, byte[] property, int index1, int index2, double value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetIndexedIndexedIntValue(int id, byte[] property, int index1, int index2, int value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern double GetArgValue(int id, byte[] property, double arg);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetArgValue(int id, byte[] property, double arg, double value);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetIndexedIntValue(int id, byte[] property, int index);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetIndexedIdValue(int id, byte[] property, int index);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetArrayIntValue(int id, byte[] property, IntPtr values, uint size);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetCallBack(int id, string property, Callback cb);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetMultipleCallBack(int id, string property, MultipleCallback cb);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetModelSampleCallback(int id, string property, ModelSampleCallback cb);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetMultipleModelSampleCallback(int id, string property, MultipleModelSampleCallback cb);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetProgressCallBacks(int id, ProgressCallBack progress, DetailedProgressCallBack detailed, TextualProgressCallBack textual);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void SetEmptyCallBack(int id, string property, EmptyCallback cb);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern IntPtr GetCallBack(int id, string property);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void Execute(int id, string method);

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern int GetExceptionLength();

    [DllImport("CWrapper", CallingConvention = CallingConvention.Cdecl)]
    public static extern void GetException(IntPtr buffer, UIntPtr size);
}