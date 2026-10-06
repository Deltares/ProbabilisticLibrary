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
using Deltares.Probabilistic.Utils;
using System.Collections.Generic;
using System.Linq;

namespace Deltares.Probabilistic.Optimization;

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
        set { Interface.SetStringValue(id, "uncertainty_method", OptimizationMethodConverter.ConvertToString(value)); }
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
        Interface.SetArrayIntValue(id, "search_parameter_settings", this.searchParameterSettings.Select(p => p.GetId()).ToArray());
    }
}