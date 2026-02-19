using System;
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Reliability;

public class ReliabilitySettings : IDisposable
{
    private int id = 0;
    private List<StochastSettings> stochastSettings = null;

    public ReliabilitySettings()
    {
        this.id = Interface.Create("reliability_settings");
    }

    internal ReliabilitySettings(int id)
    {
        this.id = id;
    }

    public void Dispose()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public ReliabilityMethod ReliabilityMethod
    {
        get { return ReliabilityMethodConverter.ConvertFromString(Interface.GetStringValue(id, "reliability_method")); }
        set { Interface.SetStringValue(id, "reliability_method", ReliabilityMethodConverter.ConvertToString(value)); }
    }

    public int MaxParallelProcesses
    {
        get { return Interface.GetIntValue(id, "max_parallel_processes"); }
        set { Interface.SetIntValue(id, "max_parallel_processes", value); }
    }

    public bool SaveRealizations
    {
        get { return Interface.GetBoolValue(id, "save_realizations"); }
        set { Interface.SetBoolValue(id, "save_realizations", value); }
    }

    public bool SaveConvergence
    {
        get { return Interface.GetBoolValue(id, "save_convergence"); }
        set { Interface.SetBoolValue(id, "save_convergence", value); }
    }

    public bool SaveMessages
    {
        get { return Interface.GetBoolValue(id, "save_messages"); }
        set { Interface.SetBoolValue(id, "save_messages", value); }
    }

    public int MaxMessages
    {
        get { return Interface.GetIntValue(id, "max_messages"); }
        set { Interface.SetIntValue(id, "max_messages", value); }
    }

    public MessageType LowestMessageType
    {
        get { return MessageTypeConverter.ConvertFromString(Interface.GetStringValue(id, "lowest_message_type")); }
        set { Interface.SetStringValue(id, "lowest_message_type", MessageTypeConverter.ConvertToString(value)); }
    }

    public bool ReuseCalculations
    {
        get { return Interface.GetBoolValue(id, "reuse_calculations"); }
        set { Interface.SetBoolValue(id, "reuse_calculations", value); }
    }

    public DesignPointMethod DesignPointMethod
    {
        get
        {
            return DesignPointMethodConverter.ConvertFromString(Interface.GetStringValue(id, "design_point_method"));
        }
        set { Interface.SetStringValue(id, "design_point_method", DesignPointMethodConverter.ConvertToString(value)); }
    }

    public StartPointMethod StartMethod
    {
        get { return StartPointMethodConverter.ConvertFromString(Interface.GetStringValue(id, "start_method")); }
        set { Interface.SetStringValue(id, "start_method", StartPointMethodConverter.ConvertToString(value)); }
    }

    public bool AllQuadrants
    {
        get { return Interface.GetBoolValue(id, "all_quadrants"); }
        set { Interface.SetBoolValue(id, "all_quadrants", value); }
    }

    public bool FilterAtNonConvergence
    {
        get { return Interface.GetBoolValue(id, "filter_at_non_convergence"); }
        set { Interface.SetBoolValue(id, "filter_at_non_convergence", value); }
    }

    public int MaxStepsSphereSearch
    {
        get { return Interface.GetIntValue(id, "max_steps_sphere_search"); }
        set { Interface.SetIntValue(id, "max_steps_sphere_search", value); }
    }

    public bool IsRepeatableRandom
    {
        get { return Interface.GetBoolValue(id, "is_repeatable_random"); }
        set { Interface.SetBoolValue(id, "is_repeatable_random", value); }
    }

    public int RandomSeed
    {
        get { return Interface.GetIntValue(id, "random_seed"); }
        set { Interface.SetIntValue(id, "random_seed", value); }
    }

    public SampleMethod SampleMethod
    {
        get { return SampleMethodConverter.ConvertFromString(Interface.GetStringValue(id, "sample_method")); }
        set { Interface.SetStringValue(id, "sample_method", SampleMethodConverter.ConvertToString(value)); }
    }

    public int MinimumSamples
    {
        get { return Interface.GetIntValue(id, "minimum_samples"); }
        set { Interface.SetIntValue(id, "minimum_samples", value); }
    }

    public int MaximumSamples
    {
        get { return Interface.GetIntValue(id, "maximum_samples"); }
        set { Interface.SetIntValue(id, "maximum_samples", value); }
    }

    public int MaximumSamplesNoResult
    {
        get { return Interface.GetIntValue(id, "maximum_samples_no_result"); }
        set { Interface.SetIntValue(id, "maximum_samples_no_result", value); }
    }

    public int MinimumIterations
    {
        get { return Interface.GetIntValue(id, "minimum_iterations"); }
        set { Interface.SetIntValue(id, "minimum_iterations", value); }
    }

    public int MaximumIterations
    {
        get { return Interface.GetIntValue(id, "maximum_iterations"); }
        set { Interface.SetIntValue(id, "maximum_iterations", value); }
    }

    public int MinimumDirections
    {
        get { return Interface.GetIntValue(id, "minimum_directions"); }
        set { Interface.SetIntValue(id, "minimum_directions", value); }
    }

    public int MaximumDirections
    {
        get { return Interface.GetIntValue(id, "maximum_directions"); }
        set { Interface.SetIntValue(id, "maximum_directions", value); }
    }

    public ModelVaryingType ModelVaryingType
    {
        get { return ModelVaryingTypeConverter.ConvertFromString(Interface.GetStringValue(id, "model_varying_type")); }
        set { Interface.SetStringValue(id, "model_varying_type", ModelVaryingTypeConverter.ConvertToString(value)); }
    }

    public double EpsilonBeta
    {
        get { return Interface.GetValue(id, "epsilon_beta"); }
        set { Interface.SetValue(id, "epsilon_beta", value); }
    }

    public double StepSize
    {
        get { return Interface.GetValue(id, "step_size"); }
        set { Interface.SetValue(id, "step_size", value); }
    }

    public GradientMethod GradientType
    {
        get { return GradientMethodConverter.ConvertFromString(Interface.GetStringValue(id, "gradient_type")); }
        set { Interface.SetStringValue(id, "gradient_type", GradientMethodConverter.ConvertToString(value)); }
    }

    public double RelaxationFactor
    {
        get { return Interface.GetValue(id, "relaxation_factor"); }
        set { Interface.SetValue(id, "relaxation_factor", value); }
    }

    public int RelaxationLoops
    {
        get { return Interface.GetIntValue(id, "relaxation_loops"); }
        set { Interface.SetIntValue(id, "relaxation_loops", value); }
    }

    public int MinimumVarianceLoops
    {
        get { return Interface.GetIntValue(id, "minimum_variance_loops"); }
        set { Interface.SetIntValue(id, "minimum_variance_loops", value); }
    }

    public int MaximumVarianceLoops
    {
        get { return Interface.GetIntValue(id, "maximum_variance_loops"); }
        set { Interface.SetIntValue(id, "maximum_variance_loops", value); }
    }

    public double VariationCoefficient
    {
        get { return Interface.GetValue(id, "variation_coefficient"); }
        set { Interface.SetValue(id, "variation_coefficient", value); }
    }

    public double MarkovChainDeviation
    {
        get { return Interface.GetValue(id, "markov_chain_deviation"); }
        set { Interface.SetValue(id, "markov_chain_deviation", value); }
    }

    public double FractionFailed
    {
        get { return Interface.GetValue(id, "fraction_failed"); }
        set { Interface.SetValue(id, "fraction_failed", value); }
    }

    public bool Clustering
    {
        get { return Interface.GetBoolValue(id, "clustering"); }
        set { Interface.SetBoolValue(id, "clustering", value); }
    }

    public int MaxClusters
    {
        get { return Interface.GetIntValue(id, "max_clusters"); }
        set { Interface.SetIntValue(id, "max_clusters", value); }
    }

    public bool OptimizeNumberClusters
    {
        get { return Interface.GetBoolValue(id, "optimize_number_clusters"); }
        set { Interface.SetBoolValue(id, "optimize_number_clusters", value); }
    }

    internal void SetVariables(IList<Stochast> stochasts)
    {
        List<StochastSettings> newStochastSettings = new List<StochastSettings>();

        foreach (Stochast stochast in stochasts)
        {
            StochastSettings stochastSettings = StochastSettings.FirstOrDefault(p => p.Stochast == stochast);
            if (stochastSettings == null)
            {
                stochastSettings = new StochastSettings();
                stochastSettings.Stochast = stochast;
            }

            newStochastSettings.Add(stochastSettings);
        }

        StochastSettings.Clear();
        stochastSettings.AddRange(newStochastSettings);

        Interface.SetArrayIntValue(id, "stochast_settings", StochastSettings.Select(p => p.GetId()).ToArray());
    }

    public void SetStartPoint(StochastPoint stochastPoint)
    {
        Interface.SetIntValue(id, "start_point", stochastPoint.GetId());
    }

    public IList<StochastSettings> StochastSettings
    {
        get
        {
            if (stochastSettings == null)
            {
                stochastSettings = new List<StochastSettings>();

                int[] stochastSettingsIds = Interface.GetArrayIdValue(id, "stochast_settings");
                foreach (int stochastSettingsId in stochastSettingsIds)
                {
                    stochastSettings.Add(new StochastSettings(stochastSettingsId));
                }
            }

            return stochastSettings;
        }
    }
}