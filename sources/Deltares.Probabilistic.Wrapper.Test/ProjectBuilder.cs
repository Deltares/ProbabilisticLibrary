// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
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
//
﻿using System;
using System.Collections.Generic;
using System.Diagnostics.Tracing;
using System.Linq;
using System.Threading.Tasks;
using Deltares.Statistics.Wrappers;
using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using Deltares.Utils.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
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
        private readonly TagRepository tagRepository = new TagRepository();

        public ZSampleOutput(ZDelegate function)
        {
            this.function = function;
        }

        public void CalculateSample(ModelSample sample)
        {
            ZFunctionOutput output = function.Invoke(sample.Values);
            sample.Z = output.Z;
            sample.Tag = tagRepository.RegisterTag(output);
        }

        public TagRepository GeTagRepository()
        {
            return tagRepository;
        }
    }

    public static class ProjectBuilder
    {
        public static ZSampleDelegate SampleDelegate(ZDelegate xDelegate)
        {
            return new ZSampleOutput(xDelegate).CalculateSample;
        }

        public static ZSampleOutput GetSampleOutput(ZDelegate xDelegate)
        {
            return new ZSampleOutput(xDelegate);
        }

        public static Project GetAddOneProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetDeterministicStochast(1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            ZSampleOutput zSampleOutput = GetSampleOutput(Sum);

            project.ZFunction = zSampleOutput.CalculateSample;

            return project;
        }

        public static Project GetLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            ZSampleOutput zSampleOutput = GetSampleOutput(Linear);

            project.ZFunction = zSampleOutput.CalculateSample;
            project.TagRepository = zSampleOutput.GeTagRepository();

            return project;
        }

        public static Project GetUnbalancedLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            ZSampleOutput zSampleOutput = GetSampleOutput(UnbalancedLinear);

            project.ZFunction = zSampleOutput.CalculateSample;
            project.TagRepository = zSampleOutput.GeTagRepository();

            return project;
        }

        public static SensitivityProject GetSensitivityProject(Project project)
        {
            var sensitivityProject = new SensitivityProject();

            sensitivityProject.Stochasts.AddRange(project.Stochasts);
            sensitivityProject.CorrelationMatrix = project.CorrelationMatrix;
            sensitivityProject.ZFunction = project.ZFunction;
            sensitivityProject.TagRepository = project.TagRepository;

            return sensitivityProject;
        }

        public static Project GetInverseLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(InverseLinear);

            return project;
        }

        public static Project GetLinearAbsoluteProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LinearAbsolute);

            return project;
        }

        public static Project GetLinearSmallProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LinearSmall);

            return project;
        }

        public static Project GetLinearSmallSlowProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LinearSmallSlow);

            return project;
        }

        public static Project GetLinearAbsoluteSmallProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LinearAbsoluteSmall);

            return project;
        }

        public static Project GetLinearFullyCorrelatedProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);
            project.CorrelationMatrix.SetCorrelation(project.Stochasts[0], project.Stochasts[1], 1);

            project.ZFunction = SampleDelegate(Linear);

            return project;
        }

        public static Project GetLinearPartialCorrelatedProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);
            project.CorrelationMatrix.SetCorrelation(project.Stochasts[0], project.Stochasts[1], 0.5);

            project.ZFunction = SampleDelegate(Linear);

            return project;
        }

        public static Project GetLinearNegativeFullyCorrelatedProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(0, 1.5));
            project.Stochasts.Add(GetUniformStochast(-1, 1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.CorrelationMatrix.SetCorrelation(project.Stochasts[0], project.Stochasts[1], -1);

            project.ZFunction = SampleDelegate(Linear);

            return project;
        }

        public static Project GetLinearMultiCorrelatedProject(bool switchCorrelations)
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-0.5, 0.5));
            project.Stochasts.Add(GetUniformStochast(-1));

            project.CorrelationMatrix.Initialize(project.Stochasts);
            if (switchCorrelations)
            {
                project.CorrelationMatrix.SetCorrelation(project.Stochasts[1], project.Stochasts[0], 1);
                project.CorrelationMatrix.SetCorrelation(project.Stochasts[2], project.Stochasts[1], 2);
            }
            else
            {
                project.CorrelationMatrix.SetCorrelation(project.Stochasts[0], project.Stochasts[1], 1);
                project.CorrelationMatrix.SetCorrelation(project.Stochasts[1], project.Stochasts[2], 2);
            }

            project.ZFunction = SampleDelegate(Linear);

            return project;
        }

        public static Project GetDoubleLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(-1, 1));
            project.Stochasts.Add(GetUniformStochast(-1, 1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(DoubleLinear);

            return project;
        }

        public static Project GetEdgeProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast(0, 1));
            project.Stochasts.Add(GetUniformStochast(0, 1));
            project.Stochasts.Add(GetUniformStochast(0, 1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Edge);

            return project;
        }

        public static Project GetNonLinearProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(NonLinear);

            return project;
        }

        public static Project GetBlockProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Block);

            return project;
        }

        public static Project GetSemiBlockProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(SemiBlock);

            return project;
        }

        public static Project GetLoadStrengthProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast(5));
            project.Stochasts.Add(GetNormalStochast(7));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LoadStrength);

            return project;
        }

        public static Project GetLoadStrengthSurvivedProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast(5));
            project.Stochasts.Add(GetNormalStochast(7));
            project.Stochasts.Add(GetDeterministicStochast(6));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(LoadStrengthSurvived);

            return project;
        }


        public static Project GetConvexProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Convex);

            return project;
        }

        public static Project GetDiscontinuousProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast(15, 2.5));
            project.Stochasts.Add(GetNormalStochast(5, 0.5));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Discontinuous);

            return project;
        }

        public static Project GetManyVarsProject()
        {
            var project = new Project();

            for (var i = 0; i < 13; i++) project.Stochasts.Add(GetUniformStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(ManyVars);

            return project;
        }

        public static Project GetNoisyProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetLogNormalStochast(120, 12));
            project.Stochasts.Add(GetLogNormalStochast(120, 12));
            project.Stochasts.Add(GetLogNormalStochast(120, 12));
            project.Stochasts.Add(GetLogNormalStochast(120, 12));
            project.Stochasts.Add(GetLogNormalStochast(50, 12));
            project.Stochasts.Add(GetLogNormalStochast(40, 12));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            ZSampleOutput zSampleOutput = GetSampleOutput(Noisy);

            project.ZFunction = zSampleOutput.CalculateSample;
            project.TagRepository = zSampleOutput.GeTagRepository();

            return project;
        }

        public static Project GetOblateSpheroidProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetDeterministicStochast(10));

            for (var i = 0; i < 10; i++) project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(OblateSpheroid);

            return project;
        }

        public static Project GetParallelProject()
        {
            var project = new Project();

            for (var i = 0; i < 5; i++) project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Parallel);

            return project;
        }

        public static Project GetQuadraticProject()
        {
            var project = new Project();

            for (var i = 0; i < 3; i++) project.Stochasts.Add(GetNormalStochast(1, 0.1));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Quadratic);

            return project;
        }

        public static Project GetSaddleProject()
        {
            var project = new Project();

            for (var i = 0; i < 2; i++) project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Saddle);

            return project;
        }

        public static Project GetSeriesProject()
        {
            var project = new Project();

            for (var i = 0; i < 2; i++) project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Series);

            return project;
        }

        public static Project GetTwoBranchesProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast(10, 0.5));
            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast(4));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(TwoBranches);

            return project;
        }

        public static Project GetWaveProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast(-20, 10));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Wave);

            return project;
        }

        public static Project GetBlighProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetLogNormalStochast(1.76, 1.69));
            project.Stochasts.Add(GetNormalStochast(50, 2.5));
            project.Stochasts.Add(GetDeterministicStochast(18));
            project.Stochasts.Add(GetGumbelStochast(0.76423, 0.52072));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Bligh);

            return project;
        }

        public static Project GetNonVaryingProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetNormalStochast());
            project.Stochasts.Add(GetNormalStochast());

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(NonVarying);

            return project;
        }

        public static Project GetDiscreteProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast());
            project.Stochasts.Add(GetUniformStochast());

            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Discrete });

            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(5, 0.35));
            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(6, 0.2));
            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(7, 0.45));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Discrete);

            return project;
        }

        public static Project GetQualitativeProject()
        {
            var project = new Project();

            project.Stochasts.Add(GetUniformStochast());
            project.Stochasts.Add(GetUniformStochast());

            project.Stochasts.Add(new Stochast { DistributionType = DistributionType.Qualitative });

            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(5, 0.35));
            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(6, 0.2));
            project.Stochasts[2].DiscreteValues.Add(new DiscreteValue(7, 0.45));

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Discrete);

            return project;
        }

        public static Project GetVariableProject()
        {
            var project = new Project();

            Stochast c = GetNormalStochast(1.5, 0.1);
            Stochast a = GetNormalStochast(1.5, 0.5);
            Stochast b = GetNormalStochast(0.5, 1.5);

            c.IsVariableStochast = true;
            c.VariableSource = a;
            c.ValueSet.StochastValues.Add(new VariableStochastValue { X = -100, Location = -100, Scale = 0.1 });
            c.ValueSet.StochastValues.Add(new VariableStochastValue { X = 100, Location = 100, Scale = 0.1 });

            a.IsVariableStochast = true;
            a.VariableSource = b;
            a.ValueSet.StochastValues.Add(new VariableStochastValue { X = 0, Location = 0, Scale = 0 });
            a.ValueSet.StochastValues.Add(new VariableStochastValue { X = 1, Location = 3, Scale = 1 });
            a.ValueSet.StochastValues.Add(new VariableStochastValue { X = 2, Location = 5, Scale = 3 });

            project.Stochasts.Add(c);
            project.Stochasts.Add(a);
            project.Stochasts.Add(b);

            project.CorrelationMatrix.Initialize(project.Stochasts);

            project.ZFunction = SampleDelegate(Linear3_10);

            return project;
        }

        private static Stochast GetDeterministicStochast(double mean)
        {
            return new Stochast { DistributionType = DistributionType.Deterministic, Mean = mean };
        }

        private static Stochast GetNormalStochast(double mean = 0, double stdev = 1)
        {
            return new Stochast { DistributionType = DistributionType.Normal, Mean = mean, Deviation = stdev };
        }

        private static Stochast GetUniformStochast(double min = 0, double max = 1)
        {
            return new Stochast { DistributionType = DistributionType.Uniform, Minimum = min, Maximum = max };
        }

        private static Stochast GetLogNormalStochast(double mean = 0, double stdev = 1, double shift = 0)
        {
            return new Stochast
            { DistributionType = DistributionType.LogNormal, Mean = mean, Deviation = stdev, Shift = shift };
        }

        private static Stochast GetGumbelStochast(double mean = 0, double stdev = 1)
        {
            return new Stochast { DistributionType = DistributionType.Gumbel, Mean = mean, Deviation = stdev };
        }

        private static ZFunctionOutput Sum(double[] x)
        {
            return new ZFunctionOutput(x.Sum());
        }

        private static ZFunctionOutput Linear(double[] x)
        {
            return new ZFunctionOutput(1.8 - x.Sum());
        }

        private static ZFunctionOutput UnbalancedLinear(double[] x)
        {
            double sum = 0;
            for (int i = 0; i < x.Length; i++)
            {
                double factor = i % 2 == 0 ? 0.3 : 1.8;
                sum += factor * x[i];
            }
            return new ZFunctionOutput(1.8 - sum);
        }

        private static ZFunctionOutput Linear2(double a, double b)
        {
            return new ZFunctionOutput(1.8 - (a+b));
        }

        private static ZFunctionOutput Linear3 (double a, double b, double c)
        {
            return new ZFunctionOutput(1.8 - (a+b+c));
        }

        private static ZFunctionOutput Linear3_10(double[] x)
        {
            double sum = x[0] + x[2];
            return new ZFunctionOutput(10 - sum);
        }

        private static ZFunctionOutput InverseLinear(double[] x)
        {
            return new ZFunctionOutput(x.Sum() - 1.8);
        }

        private static ZFunctionOutput LinearAbsolute(double[] x)
        {
            return new ZFunctionOutput(1.8 - Math.Abs(x.Sum()));
        }

        private static ZFunctionOutput LinearSmall(double[] x)
        {
            return new ZFunctionOutput(1.98 - x.Sum());
        }

        private static ZFunctionOutput LinearSmallSlow(double[] x)
        {
            var delay = Task.Run(async () =>
            {
                await Task.Delay(1);
                return 1.98 - x.Sum();
            });

            return new ZFunctionOutput(delay.Result);
        }

        private static ZFunctionOutput LinearAbsoluteSmall(double[] x)
        {
            return new ZFunctionOutput(1.98 - Math.Abs(x.Sum()));
        }

        private static ZFunctionOutput NonLinear(double[] x)
        {
            var a = x[0];
            var b = x[1];

            return new ZFunctionOutput(3 - b + Math.Pow(4 * a, 4));
        }

        private static ZFunctionOutput DoubleLinear(double[] x)
        {
            double sum = x.Sum();

            double y1 = 1.6 - sum;
            double y2 = sum - 1.8;

            return new ZFunctionOutput(Math.Max(y1, y2));
        }

        private static ZFunctionOutput Edge(double[] x)
        {
            double max = 0;

            foreach (double xx in x)
            {
                max = Math.Max(max, xx);
            }

            double z = 0.9999 - max;

            return new ZFunctionOutput(z);
        }

        private static ZFunctionOutput Block(double[] x)
        {
            var a = x[0];
            var b = x[1];

            double z = 3 - b + Math.Pow(4 * a, 4);

            if (z < 0)
            {
                z = -100000;
            }
            else
            {
                z = 1;
            }

            return new ZFunctionOutput(z);
        }

        private static ZFunctionOutput SemiBlock(double[] x)
        {
            var a = x[0];
            var b = x[1];

            double z = 3 - b + Math.Pow(4 * a, 4);

            if (z < 0)
            {
                z = -100000;
            }

            return new ZFunctionOutput(z);
        }

        private static ZFunctionOutput LoadStrength(double[] x)
        {
            var l = x[0];
            var s = x[1];

            return new ZFunctionOutput(s - l);
        }

        private static ZFunctionOutput LoadStrengthSurvived(double[] x)
        {
            var L = x[0];
            var S = x[1];
            var L_obs = x[2];

            var z = S - L;
            var z_obs = S - L_obs;

            if (z_obs < 0)
                return new ZFunctionOutput(double.NaN);
            return new ZFunctionOutput(z);
        }

        private static ZFunctionOutput Convex(double[] x)
        {
            var a = x[0];
            var b = x[1];

            return new ZFunctionOutput(0.1 * (a - b) * (a - b) - (a + b) / Math.Sqrt(2.0) + 2.5);
        }

        private static ZFunctionOutput Discontinuous(double[] x)
        {
            var r = x[0];
            var s = x[1];

            if (r >= s)
                return new ZFunctionOutput(-0.5 + Math.Sqrt(r - s));
            return new ZFunctionOutput(-0.5);
        }

        private static ZFunctionOutput ManyVars(double[] x)
        {
            var a = x[0];
            var b = x[1];
            var c = x[2];
            var d = x[3];
            var e = x[4];
            var f = x[5];
            var g = x[6];
            var h = x[7];
            var i = x[8];
            var j = x[9];
            var k = x[10];
            var l = x[11];
            var m = x[12];

            double z = a + 2 * b - 3 * c + 1.5 * d * d - 3.5 * e * e * e + 1.3
                * Math.Sqrt(f) - Math.Sqrt(g + 1) + 3.2 * h - i * i + j * k + l * l - 2.1 * m * m * a;

            return new ZFunctionOutput(6 - z);
        }

        private static ZFunctionOutput Noisy(double[] x)
        {
            var a = x[0];
            var b = x[1];
            var c = x[2];
            var d = x[3];
            var e = x[4];
            var f = x[5];

            var sum = 0.0;

            sum += Math.Sin(100 * a);
            sum += Math.Sin(100 * b);
            sum += Math.Sin(100 * c);
            sum += Math.Sin(100 * d);
            sum += Math.Sin(100 * e);
            sum += Math.Sin(100 * f);

            return new ZFunctionOutput(a + 2 * b + 2 * c + d - 5 * e - 5 * f + 0.001 * sum);
        }

        private static ZFunctionOutput OblateSpheroid(double[] xValues)
        {
            var a = xValues[0];

            double sum = 0;

            var x = xValues.Skip(1).ToArray();

            for (var index = 0; index < 10; index++) sum += x[index] * x[index] / (1.0 + (index + 1) / 10.0);

            return new ZFunctionOutput(a - sum);
        }

        private static ZFunctionOutput Parallel(double[] x)
        {
            var a = x[0];
            var b = x[1];
            var c = x[2];
            var d = x[3];
            var e = x[4];

            var g1 = 2.677 - a - b;
            var g2 = 2.500 - b - c;
            var g3 = 2.323 - c - d;
            var g4 = 2.250 - d - e;

            return new ZFunctionOutput(Math.Max(Math.Max(g1, g2), Math.Max(g3, g4)));
        }

        private static ZFunctionOutput Quadratic(double[] x)
        {
            var a = x[0];
            var b = x[1];
            var c = x[2];

            return new ZFunctionOutput(2 * a * a + 1.5 * b * b - 3 * a - 2 * b + 0.5);
        }

        private static ZFunctionOutput Saddle(double[] x)
        {
            var a = x[0];
            var b = x[1];

            return new ZFunctionOutput(3 - a * b);
        }

        private static ZFunctionOutput Series(double[] x)
        {
            double sqrt2 = Math.Sqrt(2);

            double u1 = x[0];
            double u2 = x[1];
            double g1 = 0.1 * (u1 - u2) * (u1 - u2) - (u1 + u2) / sqrt2 + 3.0;
            double g2 = 0.1 * (u1 - u2) * (u1 - u2) + (u1 + u2) / sqrt2 + 3.0;
            double g3 = u1 - u2 + 3.5 * sqrt2;
            double g4 = -u1 + u2 + 3.5 * sqrt2;

            return new ZFunctionOutput(Math.Min(Math.Min(g1, g2), Math.Min(g3, g4)));
        }

        private static ZFunctionOutput TwoBranches(double[] x)
        {
            var a = x[0];
            var b = x[1];
            var c = x[2];

            return new ZFunctionOutput(c <= 5 ? a - b - c : c - b);
        }

        private static ZFunctionOutput Wave(double[] xValues)
        {
            var x = xValues[0];

            double z = 0;

            if (x < -1)
                z = -1 + 0.01 * (x + 1);
            else if (x > 1)
                z = 1 + 0.01 * (x - 1);
            else
                z = x;

            return new ZFunctionOutput(0.8 - z);
        }

        private static ZFunctionOutput Bligh(double[] xValues)
        {
            var m = xValues[0];
            var l = xValues[1];
            var c = xValues[2];
            var h = xValues[3];

            return new ZFunctionOutput(m * l / c - h);
        }

        private static ZFunctionOutput Discrete(double[] xValues)
        {
            double a = xValues[0];
            double b = xValues[1];
            double scen = xValues[2];

            double z;

            if (scen == 1)
            {
                z = a + b;
            }
            else if (scen == 2)
            {
                z = 2.5 * a - 0.5 * b;
            }
            else
            {
                z = 2.2 - (a + b);
            }

            return new ZFunctionOutput(1.8 - z);
        }

        private static ZFunctionOutput NonVarying(double[] xValues)
        {
            return new ZFunctionOutput(1);
        }

        public static DesignPoint GetSimpleDesignPoint(double beta, int count)
        {
            DesignPoint designPoint = new DesignPoint();
            designPoint.Beta = beta;

            for (int i = 0; i < count; i++)
            {
                StochastPointAlpha alpha = new StochastPointAlpha();
                alpha.Parameter = GetUniformStochast();
                alpha.Alpha = -Math.Sqrt(1.0 / count);
                alpha.AlphaCorrelated = alpha.Alpha;
                alpha.U = - beta * alpha.Alpha;
                alpha.X = alpha.Parameter.GetXFromU(alpha.U);

                designPoint.Alphas.Add(alpha);
            }

            return designPoint;
        }
    }
}
