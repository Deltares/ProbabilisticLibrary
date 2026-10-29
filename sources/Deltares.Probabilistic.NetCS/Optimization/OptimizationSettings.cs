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
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Optimization;

public enum InitializePopulationMethod
{
    Uniform,
    Parabolic
}

public enum RandomFrequency
{
    Never,
    Generation,
    Child
}

public enum RandomEffect
{
    Unity,
    Free
}

public class OptimizationSettings
{
    private int id = 0;
    private CallBackList<SearchParameterSettings> searchParameterSettings = null;

    public OptimizationSettings()
    {
        this.id = Interface.Create("optimization_settings");
    }

    internal OptimizationSettings(int id)
    {
        this.id = id;
    }

    ~OptimizationSettings()
    {
        Interface.Destroy(id);
    }

    internal int GetId()
    {
        return id;
    }

    public OptimizationMethod OptimizationMethod
    {
        get { return OptimizationMethodConverter.ConvertFromString(Interface.GetStringValue(id, "optimization_method")); }
        set { Interface.SetStringValue(id, "optimization_method", OptimizationMethodConverter.ConvertToString(value)); }
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

    public bool AllowRepository
    {
        get { return Interface.GetBoolValue(id, "allow_repository"); }
        set { Interface.SetBoolValue(id, "allow_repository", value); }
    }

    public bool UseZFromSample
    {
        get { return Interface.GetBoolValue(id, "use_z_from_sample"); }
        set { Interface.SetBoolValue(id, "use_z_from_sample", value); }
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

    public int MaximumIterations
    {
        get { return Interface.GetIntValue(id, "maximum_iterations"); }
        set { Interface.SetIntValue(id, "maximum_iterations", value); }
    }

    public int MaxGridMoves
    {
        get { return Interface.GetIntValue(id, "max_grid_moves"); }
        set { Interface.SetIntValue(id, "max_grid_moves", value); }
    }

    public double EpsilonBeta
    {
        get { return Interface.GetValue(id, "epsilon_beta"); }
        set { Interface.SetValue(id, "epsilon_beta", value); }
    }

    public double DifferentialWeight
    {
        get { return Interface.GetValue(id, "differential_weight"); }
        set { Interface.SetValue(id, "differential_weight", value); }
    }

    public double CrossOver
    {
        get { return Interface.GetValue(id, "cross_over"); }
        set { Interface.SetValue(id, "cross_over", value); }
    }

    public double Beta
    {
        get { return Interface.GetValue(id, "beta"); }
        set { Interface.SetValue(id, "beta", value); }
    }

    public double Delta
    {
        get { return Interface.GetValue(id, "delta"); }
        set { Interface.SetValue(id, "delta", value); }
    }

    public double MutationRate
    {
        get { return Interface.GetValue(id, "mutation_rate"); }
        set { Interface.SetValue(id, "mutation_rate", value); }
    }

    public double[] CrossOverFractions
    {
        get { return Interface.GetArrayValue(id, "cross_over_fractions"); }
        set { Interface.SetArrayValue(id, "cross_over_fractions", value); }
    }

    public double[] MutationFractions
    {
        get { return Interface.GetArrayValue(id, "mutation_fractions"); }
        set { Interface.SetArrayValue(id, "mutation_fractions", value); }
    }

    public double CreepReduction
    {
        get { return Interface.GetValue(id, "creep_reduction"); }
        set { Interface.SetValue(id, "creep_reduction", value); }
    }

    public InitializePopulationMethod InitializePopulationMethod
    {
        get
        {
            var s = Interface.GetStringValue(id, "initialize_population_method");
            return s == "uniform" ? InitializePopulationMethod.Uniform : InitializePopulationMethod.Parabolic;
        }
        set
        {
            var s = value == InitializePopulationMethod.Uniform ? "uniform" : "parabolic";
            Interface.SetStringValue(id, "initialize_population_method", s);
        }
    }

    private string RandomFrequencyToString(RandomFrequency freq)
    {
        switch (freq)
        {
            case RandomFrequency.Never: return "never";
            case RandomFrequency.Generation: return "generation";
            case RandomFrequency.Child: return "child";
            default: throw new ArgumentException("unknown random frequency");
        }
    }

    private RandomFrequency StringToRandomFrequency(string s)
    {
        switch (s)
        {
            case "never": return RandomFrequency.Never;
            case "generation": return RandomFrequency.Generation;
            case "child": return RandomFrequency.Child;
            default: throw new ArgumentException("unknown random frequency");
        }
    }

    public RandomFrequency CrossOverRandomFrequency
    {
        get
        {
            var s = Interface.GetStringValue(id, "cross_over_random_frequency");
            return StringToRandomFrequency(s);
        }
        set
        {
            var s = RandomFrequencyToString(value);
            Interface.SetStringValue(id, "cross_over_random_frequency", s);
        }
    }

    public RandomFrequency MutationRandomFrequency
    {
        get
        {
            var s = Interface.GetStringValue(id, "mutation_random_frequency");
            return StringToRandomFrequency(s);
        }
        set
        {
            var s = RandomFrequencyToString(value);
            Interface.SetStringValue(id, "mutation_random_frequency", s);
        }
    }

    private RandomEffect StringToRandomEffect(string s)
    {
        return s == "unity" ? RandomEffect.Unity : RandomEffect.Free;
    }

    private string RandomEffectToString(RandomEffect randomEffect)
    {
        return randomEffect == RandomEffect.Unity ? "unity" : "free";
    }

    public RandomEffect CrossOverRandomEffect
    {
        get
        {
            var s = Interface.GetStringValue(id, "cross_over_random_effect");
            return StringToRandomEffect(s);
        }
        set
        {
            var s = RandomEffectToString(value);
            Interface.SetStringValue(id, "cross_over_random_effect", s);
        }
    }

    public RandomEffect MutationRandomEffect
    {
        get
        {
            var s = Interface.GetStringValue(id, "mutation_random_effect");
            return StringToRandomEffect(s);
        }
        set
        {
            var s = RandomEffectToString(value);
            Interface.SetStringValue(id, "mutation_random_effect", s);
        }
    }

    public int GenerationCount
    {
        get { return Interface.GetIntValue(id, "generation_count"); }
        set { Interface.SetIntValue(id, "generation_count", value); }
    }

    public int EliteCount
    {
        get { return Interface.GetIntValue(id, "elite_count"); }
        set { Interface.SetIntValue(id, "elite_count", value); }
    }

    public int PopulationCount
    {
        get { return Interface.GetIntValue(id, "population_count"); }
        set { Interface.SetIntValue(id, "population_count", value); }
    }

    public int StopAfterNonImprovingGenerations
    {
        get { return Interface.GetIntValue(id, "stop_after_non_improving_generations"); }
        set { Interface.SetIntValue(id, "stop_after_non_improving_generations", value); }
    }

    public int Seed
    {
        get { return Interface.GetIntValue(id, "seed"); }
        set { Interface.SetIntValue(id, "seed", value); }
    }

    public IList<SearchParameterSettings> SearchParameterSettings
    {
        get
        {
            if (searchParameterSettings == null)
            {
                searchParameterSettings = new CallBackList<SearchParameterSettings>(SearchParameterSettingsChanged);

                int[] searchParameterSettingsIds = Interface.GetArrayIdValue(id, "search_parameter_settings");
                foreach (int searchParameterSettingsId in searchParameterSettingsIds)
                {
                    searchParameterSettings.Add(new SearchParameterSettings(searchParameterSettingsId));
                }
            }

            return searchParameterSettings;
        }
    }

    private void SearchParameterSettingsChanged(ListOperationType listOperation, SearchParameterSettings item)
    {
        if (listOperation == ListOperationType.Add)
        {
            Interface.SetIntValue(id, "add_search_parameter_settings", item.GetId());
        }
        else
        {
            Interface.SetArrayIntValue(id, "search_parameter_settings", this.searchParameterSettings.Select(p => p.GetId()).ToArray());
        }
    }
}