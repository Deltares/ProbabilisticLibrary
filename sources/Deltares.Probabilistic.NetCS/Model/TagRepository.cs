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
using System.Collections.Generic;

namespace Deltares.Probabilistic.Model;

public class TagRepository
{
    private Dictionary<int, object> tags = new Dictionary<int, object>();
    private int counter = 0;
    private readonly object lockObject = new object();

    public TagRepository()
    {
    }

    public int RegisterTag(object tag)
    {
        if (tag != null)
        {
            lock (lockObject)
            {
                tags[++counter] = tag;
                return counter;
            }
        }
        else
        {
            return 0;
        }
    }

    public object RetrieveTag(int index) 
    {
        if (index == 0)
        {
            return null;
        }
        else
        {
            lock (lockObject)
            {
                if (tags.ContainsKey(index))
                {
                    return tags[index];
                }
                else
                {
                    return null;
                }
            }
        }
    }
}