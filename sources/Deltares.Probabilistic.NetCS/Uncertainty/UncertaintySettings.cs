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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Reliability;
using Deltares.Probabilistic.Statistics;
using Deltares.Probabilistic.Utils;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Uncertainty;

public class UncertaintySettings
{
    private int id = 0;
    private CallBackList<ProbabilityValue> quantiles = null;
    private CallBackList<StochastSettings> stochastSettings = null;

    public UncertaintySettings()
    {
        this.id = Interface.Create("uncertainty_settings");
    }

    internal UncertaintySettings(int id)
    {
        this.id = id;
    }

    ~UncertaintySettings()
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

    public MessageType LowestMessageType
    {
        get { return MessageTypeConverter.ConvertFromString(Interface.GetStringValue(id, "lowest_message_type")); }
        set { Interface.SetStringValue(id, "lowest_message_type", MessageTypeConverter.ConvertToString(value)); }
    }

    public int MaxChunkSize
    {
        get { return Interface.GetIntValue(id, "max_chunk_size"); }
        set { Interface.SetIntValue(id, "max_chunk_size", value); }
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

    public double StepSize
    {
        get { return Interface.GetValue(id, "step_size"); }
        set { Interface.SetValue(id, "step_size", value); }
    }

    public double VariationCoefficient
    {
        get { return Interface.GetValue(id, "variation_coefficient"); }
        set { Interface.SetValue(id, "variation_coefficient", value); }
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

    public double ProbabilityForConvergence
    {
        get { return Interface.GetValue(id, "probability_for_convergence"); }
        set { Interface.SetValue(id, "probability_for_convergence", value); }
    }

    public GradientMethod GradientType
    {
        get { return GradientMethodConverter.ConvertFromString(Interface.GetStringValue(id, "gradient_type")); }
        set { Interface.SetStringValue(id, "gradient_type", GradientMethodConverter.ConvertToString(value)); }
    }

    public double MinimumU
    {
        get { return Interface.GetValue(id, "minimum_u"); }
        set { Interface.SetValue(id, "minimum_u", value); }
    }

    public double MaximumU
    {
        get { return Interface.GetValue(id, "maximum_u"); }
        set { Interface.SetValue(id, "maximum_u", value); }
    }

    public double GlobalStepSize
    {
        get { return Interface.GetValue(id, "global_step_size"); }
        set { Interface.SetValue(id, "global_step_size", value); }
    }

    public int GetRequiredSamples()
    {
        return Interface.GetIntValue(id, "required_samples");
    }

    public IList<ProbabilityValue> Quantiles
    {
        get
        {
            if (quantiles == null)
            {
                quantiles = new CallBackList<ProbabilityValue>(RequestedQuantilesChanged);

                int[] quantilesIds = Interface.GetArrayIdValue(id, "quantiles");
                foreach (int quantilesId in quantilesIds)
                {
                    quantiles.Add(new ProbabilityValue(quantilesId));
                }
            }

            return quantiles;
        }
    }

    private void RequestedQuantilesChanged(ListOperationType listOperation, ProbabilityValue item)
    {
        Interface.SetArrayIntValue(id, "quantiles", this.quantiles.Select(p => p.GetId()).ToArray());
    }


    public IList<StochastSettings> StochastSettings
    {
        get
        {
            if (stochastSettings == null)
            {
                stochastSettings = new CallBackList<StochastSettings>(StochastSettingsChanged);

                int[] stochastSettingsIds = Interface.GetArrayIdValue(id, "stochast_settings");
                foreach (int stochastSettingsId in stochastSettingsIds)
                {
                    stochastSettings.Add(new StochastSettings(stochastSettingsId));
                }
            }

            return stochastSettings;
        }
    }

    private void StochastSettingsChanged(ListOperationType listOperation, StochastSettings item)
    {
        Interface.SetArrayIntValue(id, "stochast_settings", this.stochastSettings.Select(p => p.GetId()).ToArray());
    }
}