using System;
using System.Linq;
using System.Runtime.InteropServices;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void ModelSampleCallback(ref ModelSampleStruct sample);

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
        this.Tag = sample.Tag;
    }

    public void PrepareForReturn(ref ModelSampleStruct sample)
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
        sample.Tag = Tag;
    }

    public double[] Values
    {
        get { return values; }
    }

    public double[] OutputValues
    {
        get { return outputValues; }
    }

    public int Iteration { get; set; }

    public double Beta { get; set; }

    public double Z { get; set; }

    public double Weight { get; set; }

    public bool AllowProxy { get; set; }

    public bool UsedProxy { get; set; }

    public bool IsRestartRequired { get; set; }

    public int Tag { get; set; }
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
