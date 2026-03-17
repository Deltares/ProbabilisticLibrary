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

namespace Deltares.Probabilistic.Utils;

public static class ObjectFactory
{
    private static Dictionary<int, WeakReference> objects = new Dictionary<int, WeakReference>();
    private static int lastId = 0;

    public static T GetObject<T>(int id)
    {
        if (objects.ContainsKey(id))
        {
            return (T) objects[id].Target;
        }
        else
        {
            object newObject = Activator.CreateInstance(typeof(T), id);
            return (T)newObject;
        }
    }

    public static void Register (object obj, int id)
    {
        objects[id] = new WeakReference(obj);
        lastId = id;
    }

    public static void Destroy(int id)
    {
        objects.Remove(id);
        Interface.Destroy(id);
    }

    // for testing
    public static int GetCount()
    {
        return objects.Count;
    }

    // for testing
    public static int GetLastId()
    {
        return lastId;
    }
}