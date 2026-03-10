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
using System;

namespace Deltares.Probabilistic.Server;

public class ProjectHandler
{
    public ProjectHandler()
    {
    }

    public virtual bool CanHandle(string type)
    {
        return true;
    }

    public virtual void Create(string type, int id)
    {
        Interface.Create(type);
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
        throw new NotImplementedException("GetIndexedIntValue");
        //return Interface.GetIndexedIntValue(id, property, index);
    }

    public virtual int GetIndexedIdValue(int id, string property, int index, int newId)
    {
        throw new NotImplementedException("GetIndexedIdValue");
        //return Interface.GetIndexedIdValue(id, property, index, newId);
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

    public virtual int GetIdValue(int id, string property, int newId)
    {
        return Interface.GetIdValue(id, property);
    }
}