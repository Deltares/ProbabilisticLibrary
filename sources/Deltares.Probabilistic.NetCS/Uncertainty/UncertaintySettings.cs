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
using System.Collections.Generic;
using System.Linq;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Uncertainty;

public class UncertaintySettings : IDisposable
{
    private int id = 0;
    private List<StochastSettings> stochastSettings = null;

    public UncertaintySettings()
    {
        this.id = Interface.Create("uncertainty_settings");
    }

    internal UncertaintySettings(int id)
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

    public UncertaintyMethod UncertaintyMethod
    {
        get { return UncertaintyMethodConverter.ConvertFromString(Interface.GetStringValue(id, "uncertainty_method")); }
        set { Interface.SetStringValue(id, "uncertainty_method", UncertaintyMethodConverter.ConvertToString(value)); }
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

    public bool ReuseCalculations
    {
        get { return Interface.GetBoolValue(id, "reuse_calculations"); }
        set { Interface.SetBoolValue(id, "reuse_calculations", value); }
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

    public double GradientType
    {
        get { return Interface.GetValue(id, "gradient_type"); }
        set { Interface.SetValue(id, "gradient_type", value); }
    }

    public double RelaxationFactor
    {
        get { return Interface.GetValue(id, "relaxation_factor"); }
        set { Interface.SetValue(id, "relaxation_factor", value); }
    }

    public double VariationCoefficient
    {
        get { return Interface.GetValue(id, "variation_coefficient"); }
        set { Interface.SetValue(id, "variation_coefficient", value); }
    }

    public double FractionFailed
    {
        get { return Interface.GetValue(id, "fraction_failed"); }
        set { Interface.SetValue(id, "fraction_failed", value); }
    }

    public bool DeriveSamplesFromVariationCoefficient
    {
        get { return Interface.GetBoolValue(id, "derive_samples_from_variation_coefficient"); }
        set { Interface.SetBoolValue(id, "derive_samples_from_variation_coefficient", value); }
    }

    public bool CalculateCorrelations
    {
        get { return Interface.GetBoolValue(id, "calculate_correlations"); }
        set { Interface.SetBoolValue(id, "calculate_correlations", value); }
    }

    public bool CalculateInputCorrelations
    {
        get { return Interface.GetBoolValue(id, "calculate_input_correlations"); }
        set { Interface.SetBoolValue(id, "calculate_input_correlations", value); }
    }

    public int GetRequiredSamples()
    {
        return Interface.GetIntValue(id, "required_samples");
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