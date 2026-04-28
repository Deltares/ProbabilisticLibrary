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

using Deltares.Probabilistic.Utils;
using Deltares.Probabilistic.Statistics;
using System;

namespace Deltares.Probabilistic.Server;

/// <summary>
/// The project handler serves as a base class for communication between python classes and their shadow classes in C++ or C#
/// </summary>
/// <remarks>
/// This class is useful when communication occurs via a socket connection, which is required when some shadow classes are defined in C#. This class
/// is the base class for handling the .net classes defined in the probabilistic library, such as <see cref="Stochast"/>. Usually it is overridden
/// by a class handling .net classes as a shadow class.
/// </remarks>
public class ProjectHandler
{
    public ProjectHandler()
    {
    }

    /// <summary>
    /// Indicates whether the handler can handle a class of a certain type
    /// </summary>
    /// <param name="type">The type of interest</param>
    /// <returns>Indication</returns>
    public virtual bool CanHandle(string type)
    {
        return Interface.CanHandle(type);
    }

    /// <summary>
    /// Gets a new id when a new class instance is created.
    /// </summary>
    /// <returns>New id</returns>
    /// <remarks>
    /// The issuance and registration of ids is centralized in order to prevent confusion when same ids are used for different instances 
    /// </remarks>
    public int GetNewId()
    {
        return Interface.GetNewId();
    }

    public virtual int Create(string type)
    {
        return Interface.Create(type);
    }

    public virtual void Destroy(int id)
    {
        Interface.Destroy(id);
    }

    public virtual bool ShouldClose()
    {
        return false;
    }

    public virtual double GetValue(int id, string property)
    {
        return Interface.GetValue(id, property);
    }

    public virtual void SetValue(int id, string property, double value)
    {
        Interface.SetValue(id, property, value);
    }

    public virtual double GetArgValue(int id, string property, double argument)
    {
        return Interface.GetArgValue(id, property, argument);
    }

    public virtual int GetIntValue(int id, string property)
    {
        return Interface.GetIntValue(id, property);
    }

    public virtual void SetIntValue(int id, string property, int value)
    {
        Interface.SetIntValue(id, property, value);
    }

    public virtual bool GetBoolValue(int id, string property)
    {
        return Interface.GetBoolValue(id, property);
    }

    public virtual void SetBoolValue(int id, string property, bool value)
    {
        Interface.SetBoolValue(id, property, value);
    }

    public virtual string GetStringValue(int id, string property)
    {
        return Interface.GetStringValue(id, property);
    }

    public virtual void SetStringValue(int id, string property, string value)
    {
        Interface.SetStringValue(id, property, value);
    }

    public virtual int GetIndexedIntValue(int id, string property, int index)
    {
        return Interface.GetIndexedIntValue(id, property, index);
    }

    public virtual int GetIndexedIdValue(int id, string property, int index)
    {
        return Interface.GetIndexedIdValue(id, property, index);
    }

    public virtual string GetIndexedStringValue(int id, string property, int index)
    {
        return Interface.GetIndexedStringValue(id, property, index);
    }

    public virtual void SetArrayIntValue(int id, string property, int[] values)
    {
        Interface.SetArrayIntValue(id, property, values);
    }

    public virtual void Execute(int id, string method)
    {
        Interface.Execute(id, method);
    }

    public virtual double GetIndexedValue(int id, string property, int index)
    {
        return Interface.GetIndexedValue(id, property, index);
    }

    public virtual int GetIdValue(int id, string property)
    {
        return Interface.GetIdValue(id, property);
    }

    public virtual double GetIndexedIndexedValue(int id, string property, int index1, int index2)
    {
        return Interface.GetIndexedIndexedValue(id, property, index1, index2);
    }

    public virtual void SetIndexedIndexedValue(int id, string property, int index1, int index2, double value)
    {
        Interface.SetIndexedIndexedValue(id, property, index1, index2, value);
    }
}