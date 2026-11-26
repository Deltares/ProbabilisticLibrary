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
using Deltares.Probabilistic.Model;

// Shared test helpers. They may only depend on Deltares.ModelWrapper.Net,
// because they are linked into the Optimization and Reliability test projects as well.
namespace Deltares.Probabilistic.Test
{
    public class ProgressHolder
    {
        public int Step { get; private set; }
        public int Loop { get; private set; }
        public double Reliability { get; private set; }
        public double Convergence { get; private set; }

        public double Progress { get; private set; }

        public ProgressType Type { get; private set; }
        public string Text { get; private set; } = string.Empty;

        public int Invocations { get; private set; }

        public void SetProgress(double progress)
        {
            Progress = progress;
            lock (this)
            {
                Invocations++;
            }
        }

        public void SetTextualProgress(ProgressType type, string text)
        {
            Type = type;
            Text = text;
        }

        public void SetDetailedProgress(int step, int loop, double reliability, double convergence)
        {
            Step = step;
            Loop = loop;
            Reliability = reliability;
            Convergence = convergence;
        }
    }

    public class ZSampleOutput
    {
        private readonly ZDelegate function;

        public ZSampleOutput(ZDelegate function)
        {
            this.function = function;
        }

        public void CalculateSample(ModelSample sample)
        {
            ZFunctionOutput output = function.Invoke(sample.Values);
            sample.Z = output.Z;
            sample.Tag = output;
        }
    }

    public class SampleValuesOutput
    {
        private readonly ZDelegate function;

        public SampleValuesOutput(ZDelegate function)
        {
            this.function = function;
        }

        public void CalculateSample(ModelSample sample)
        {
            ZFunctionOutput output = function.Invoke(sample.Values);
            sample.Z = output.Z;
            sample.Tag = output;
        }
    }
}

