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
namespace Deltares.Probabilistic.Wrapper.Test
{
    public delegate ZFunctionOutput ZDelegate(params double[] x);

    /// <summary>
    /// Class containing the output of a Z function call.
    /// </summary>
    public class ZFunctionOutput
    {
        /// <summary>
        /// Creates a new instance of <see cref="ZFunctionOutput"/>.
        /// </summary>
        /// <param name="z">The z value.</param>
        public ZFunctionOutput(double z)
        {
            Z = z;
        }

        /// <summary>
        /// Gets or sets the Z value returned by the Z function
        /// </summary>
        public double Z { get; }
    }
}
