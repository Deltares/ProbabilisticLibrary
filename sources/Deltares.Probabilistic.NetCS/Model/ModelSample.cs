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
using System.Linq;
using System.Runtime.InteropServices;

namespace Deltares.Probabilistic.Model;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void ModelSampleCallback(ref ModelSampleStruct sample);

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void MultipleModelSampleCallback(IntPtr samples, int count);

public class ModelSample
{
    private double[] values = null;
    private double[] outputValues = null;

    public ModelSample()
    {
    }

    public ModelSample(double[] values)
    {
        // copy values
        this.values = values.Select(p => p).ToArray();
    }

    public ModelSample(ModelSampleStruct sample)
    {
        // Copy from unmanaged → managed
        values = new double[sample.ValuesCount];
        if (sample.ValuesCount > 0)
        {
            Marshal.Copy(sample.Values, values, 0, sample.ValuesCount);
        }

        outputValues = new double[sample.OutputValuesCount];
        if (sample.OutputValuesCount > 0)
        {
            Marshal.Copy(sample.OutputValues, outputValues, 0, sample.OutputValuesCount);
        }

        this.Iteration = sample.IterationIndex;
        this.Beta = sample.Beta;
        this.Z = sample.Z;
        this.Weight = sample.Weight;
        this.AllowProxy = sample.AllowProxy;
        this.UsedProxy = sample.UsedProxy;
        this.IsRestartRequired = sample.IsRestartRequired;
    }

    public void PrepareForReturn(ref ModelSampleStruct sample, TagRepository tagRepository)
    {
        // Copy back to unmanaged memory
        if (Values.Length > 0)
        {
            Marshal.Copy(Values, 0, sample.Values, Values.Length);
        }
        sample.ValuesCount = Values.Length;

        if (OutputValues.Length > 0)
        {
            Marshal.Copy(OutputValues, 0, sample.OutputValues, OutputValues.Length);
        }
        sample.OutputValuesCount = OutputValues.Length;

        sample.IterationIndex = Iteration;
        sample.Beta = Beta;
        sample.Z = Z;
        sample.Weight = Weight;
        sample.AllowProxy = AllowProxy;
        sample.UsedProxy = UsedProxy;
        sample.IsRestartRequired = IsRestartRequired;
        sample.Tag = tagRepository?.RegisterTag(Tag) ?? 0;
    }

    public double[] Values
    {
        get { return values; }
    }

    public double[] OutputValues
    {
        get { return outputValues; }
        set { outputValues = value; }
    }

    public int Iteration { get; set; }

    public double Beta { get; set; }

    public double Z { get; set; }

    public double Weight { get; set; }

    public bool AllowProxy { get; set; }

    public bool UsedProxy { get; set; }

    public bool IsRestartRequired { get; set; }

    public object Tag { get; set; } = null;

    public bool AreValuesEqual(ModelSample sample)
    {
        const double tolerance = 1E10;

        if (sample.Values.Length != Values.Length)
        {
            return false;
        }

        for (int i = 0; i < Values.Length; i++)
        {
            if (Math.Abs(Values[i] - sample.Values[i]) > tolerance)
            {
                return false;
            }
        }

        return true;
    }
}

[StructLayout(LayoutKind.Sequential)]
public struct ModelSampleStruct
{
    public IntPtr Values;
    public int ValuesCount;

    public IntPtr OutputValues;
    public int OutputValuesCount;

    public int IterationIndex;
    public int threadId;
    public double Weight;
    [MarshalAs(UnmanagedType.I1)] public bool AllowProxy;
    [MarshalAs(UnmanagedType.I1)] public bool UsedProxy;
    [MarshalAs(UnmanagedType.I1)] public bool IsRestartRequired;
    public double Beta;
    public double Z;
    [MarshalAs(UnmanagedType.I1)] public bool ExtendedLogging;
    public int LoggingCounter;
    public int Tag;
}
