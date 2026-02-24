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
using Deltares.Probabilistic.Utils;

namespace Deltares.Probabilistic.Logging
{
    public delegate void ProgressDelegate(double progress);
    public delegate void DetailedProgressDelegate(int step, int loop, double reliability, double convergence);
    public delegate void TextualProgressDelegate(ProgressType progressType, string text);

    public class ProgressIndicator // : IDisposable
    {
        private ProgressDelegate progressDelegate = null;
        private DetailedProgressDelegate detailedProgressDelegate = null;
        private TextualProgressDelegate textualProgressDelegate = null;

        //private int id = 0;

        public ProgressIndicator(ProgressDelegate progressDelegate, DetailedProgressDelegate detailedProgressDelegate, TextualProgressDelegate textualProgressDelegate)
        {
            this.progressDelegate = progressDelegate;
            this.detailedProgressDelegate = detailedProgressDelegate;
            this.textualProgressDelegate = textualProgressDelegate;
        }

        //public void Dispose()
        //{
        //    Interface.Destroy(id);
        //}

        //internal int GetId()
        //{
        //    return id;
        //}

        public void DoProgress(double progress)
        {
            progressDelegate?.Invoke(progress);
        }

        public void DoDetailedProgress(int step, int loop, double reliability, double convergence)
        {
            detailedProgressDelegate?.Invoke(step, loop, reliability, convergence);
        }

        public void DoTextualProgress(int progressIndicator, string text)
        {
            ProgressType progressType = progressIndicator == 0 ? ProgressType.Global : ProgressType.Detailed;

            textualProgressDelegate?.Invoke(progressType, text);
        }
    }
}
