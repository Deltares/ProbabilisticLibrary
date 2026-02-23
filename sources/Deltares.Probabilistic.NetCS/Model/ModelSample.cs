using System;
using System.Linq;
using System.Runtime.InteropServices;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Model;

[UnmanagedFunctionPointer(CallingConvention.Cdecl)]
public delegate void ModelSampleCallback(ref ModelSampleStruct sample);

public class ModelSample
{
    private ModelSampleStruct sample;
    private double[] values = null;
    private double[] outputValues = null;

    public ModelSample()
    {
        this.sample = new ModelSampleStruct();
    }

    public ModelSample(double[] values)
    {
        // copy values
        this.values = values.Select(p => p).ToArray();
    }

    public ModelSample(ModelSampleStruct sample)
    {
        this.sample = sample;

        // Copy from unmanaged → managed
        values = new double[sample.ValuesCount];
        Marshal.Copy(sample.Values, values, 0, sample.ValuesCount);

        outputValues = new double[sample.OutputValuesCount];
        Marshal.Copy(sample.OutputValues, outputValues, 0, sample.OutputValuesCount);
    }

    public void PrepareForReturn()
    {
        // Copy back to unmanaged memory
        Marshal.Copy(OutputValues, 0, sample.OutputValues, OutputValues.Length);
    }

    public double[] Values
    {
        get { return values; }
    }

    public double[] OutputValues
    {
        get { return outputValues; }
    }

    public int Iteration
    {
        get { return sample.IterationIndex; }
        set { sample.IterationIndex = value; }
    }

    public double Beta
    {
        get { return sample.Beta; }
        set { sample.Beta = value; }
    }

    public double Z
    {
        get { return sample.Z; }
        set { sample.Z = value; }
    }

    public double Weight
    {
        get { return sample.Weight; }
        set { sample.Weight = value; }
    }

    public bool AllowProxy
    {
        get { return sample.AllowProxy; }
        set { sample.AllowProxy = value; }
    }

    public bool UsedProxy
    {
        get { return sample.UsedProxy; }
        set { sample.UsedProxy = value; }
    }

    public bool IsRestartRequired
    {
        get { return sample.IsRestartRequired; }
        set { sample.IsRestartRequired = value; }
    }

    public int Tag
    {
        get { return sample.Tag; }
        set { sample.Tag = value; }
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
