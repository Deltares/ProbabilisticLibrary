// Copyright (C) Stichting Deltares and State of the Netherlands 2023. All rights reserved.
//
// This file is part of the Hydra Ring Application.
//
// The Hydra Ring Application is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.

using System;
using System.Collections.Generic;

namespace Deltares.ProbabilisticCPP.Wrapper
{
    public class Correlations
    {

        /// <summary>
        /// go through all correlations given
        /// if (a,b) == 1 and (b,c) == 1, make sure that also (a,c) = 1
        /// special care for fully anti correlated
        /// </summary>
        public void ExtendFullyCorrelated(ref List<corrStruct> correlations)
        {
            var extraCorrelation = new List<corrStruct>();
            for (int i = 0; i < correlations.Count; i++)
            {
                if (!isFullyCorrelated(correlations[i])) continue;
                for (int j = i + 1; j < correlations.Count; j++)
                {
                    if (!isFullyCorrelated(correlations[j])) continue;
                    int a; int c;
                    if (!CheckAb(correlations[i], correlations[j], out a, out c)) continue;
                    var found = LoopCheckAc(correlations, a, c);
                    if (!found) found = LoopCheckAc(extraCorrelation, a, c);

                    if (!found)
                    {
                        extraCorrelation.Add(new corrStruct
                        {
                            idx1 = Math.Min(a, c),
                            idx2 = Math.Max(a, c),
                            correlation = correlations[i].correlation * correlations[j].correlation
                        }
                        );
                    }
                }
            }

            if (extraCorrelation.Count > 0)
            {
                correlations.AddRange(extraCorrelation);
                ExtendFullyCorrelated(ref correlations);
            }
        }

        private bool isFullyCorrelated(corrStruct c)
        {
            return Math.Abs(c.correlation) == 1.0;
        }

        private bool CheckAb(corrStruct c1, corrStruct c2, out int a, out int c)
        {
            var i = 0;
            c = -999;
            a = -999;
            if (c1.idx1 == c2.idx1) { i++; a = c1.idx2; c = c2.idx2; }
            if (c1.idx1 == c2.idx2) { i++; a = c1.idx2; c = c2.idx1; }
            if (c1.idx2 == c2.idx1) { i++; a = c1.idx1; c = c2.idx2; }
            if (c1.idx2 == c2.idx2) { i++; a = c1.idx1; c = c2.idx1; }

            return i == 1;
        }

        private bool LoopCheckAc(IEnumerable<corrStruct> corr, int a, int c)
        {
            var found = false;
            foreach (var t in corr)
            {
                if (!CheckAc(t, a, c)) continue;
                if (!isFullyCorrelated(t)) throw new Exception("inconsistent list of fully correlated stochasts");
                found = true;
                break;
            }

            return found;
        }

        private bool CheckAc(corrStruct correlation, int a, int c)
        {
            if (correlation.idx1 == a && correlation.idx2 == c) { return true; }
            if (correlation.idx2 == a && correlation.idx1 == c) { return true; }

            return false;
        }

    }
}
