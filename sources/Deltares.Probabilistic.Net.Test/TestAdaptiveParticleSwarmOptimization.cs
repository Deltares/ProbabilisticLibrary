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
using Deltares.Probabilistic.Model;
using Deltares.Probabilistic.Optimization;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test
{

    [TestFixture]
    public class TestAdaptiveParticleSwarmOptimization
    {
        private const double margin = 0.01;

        [Test]
        public void TestPolynome()
        {
            var project = ProjectBuilder.GetPolynomeOptimizationProject(2.2, 8.3, 4.25);

            project.Settings.OptimizationMethod = OptimizationMethod.AdaptiveParticleSwarmOptimization;

            foreach (ModelParameter parameter in project.InputParameters)
            {
                project.Settings.SearchParameterSettings.Add(new SearchParameterSettings
                    { Parameter = parameter, MinValue = 0, MaxValue = 10, StartValue = 0});
            }

            project.Settings.GenerationCount = 50;
            project.Settings.EliteCount = 8;
            project.Settings.Seed = 12345;
            project.Settings.PopulationCount = 125;
            project.Settings.StopAfterNonImprovingGenerations = 3;
            project.Settings.DifferentialWeight = 0.3;
            project.Settings.CrossOver = 0.3;
            project.Settings.Beta = 0.5;
            project.Settings.Delta = 0.7;

            project.Settings.AllowRepository = false;

            ClassicAssert.True(project.IsValid());

            project.Run();

            OptimizationResult result = project.Result;

            ClassicAssert.AreEqual(2, result.Values.Length);

            ClassicAssert.AreEqual(2.2, result.Values[0], margin);
            ClassicAssert.AreEqual(8.3, result.Values[1], margin);
            ClassicAssert.AreEqual(4.25, result.MinimumValue, margin);
            ClassicAssert.AreEqual(1500, result.ModelRuns);
        }

        [Test]
        public void TestPolynomeSearchArea()
        {
            var project = ProjectBuilder.GetPolynomeOptimizationProject(12.2, -8.3, 4.25);

            project.Settings.OptimizationMethod = OptimizationMethod.AdaptiveParticleSwarmOptimization;
            project.Settings.EpsilonBeta = 0.01;

            foreach (ModelParameter parameter in project.InputParameters)
            {
                project.Settings.SearchParameterSettings.Add(new SearchParameterSettings
                    { Parameter = parameter, MinValue = -100, MaxValue = 100 });
            }

            project.Run();

            OptimizationResult result = project.Result;

            ClassicAssert.AreEqual(2, result.Values.Length);

            ClassicAssert.AreEqual(12.2, result.Values[0], margin);
            ClassicAssert.AreEqual(-8.3, result.Values[1], margin);
            ClassicAssert.AreEqual(4.25, result.MinimumValue, margin);
            ClassicAssert.AreEqual(6375, result.ModelRuns);
        }

        [Test]
        public void TestPolynomeNegativeResult()
        {
            var project = ProjectBuilder.GetPolynomeOptimizationProject(12.2, -8.3, -4.25);

            project.Settings.OptimizationMethod = OptimizationMethod.AdaptiveParticleSwarmOptimization;
            project.Settings.EpsilonBeta = 0.01;

            foreach (ModelParameter parameter in project.InputParameters)
            {
                project.Settings.SearchParameterSettings.Add(new SearchParameterSettings
                    { Parameter = parameter, MinValue = -100, MaxValue = 100 });
            }

            project.Run();

            OptimizationResult result = project.Result;

            ClassicAssert.AreEqual(2, result.Values.Length);

            ClassicAssert.AreEqual(12.2, result.Values[0], margin);
            ClassicAssert.AreEqual(-8.3, result.Values[1], margin);
            ClassicAssert.AreEqual(-4.25, result.MinimumValue, margin);
            ClassicAssert.AreEqual(6375, result.ModelRuns);
        }

        [Test]
        public void TestPolynomeAllIterations()
        {
            var project = ProjectBuilder.GetPolynomeOptimizationProject(12.2, -8.3, 4.25);

            project.Settings.OptimizationMethod = OptimizationMethod.AdaptiveParticleSwarmOptimization;
            project.Settings.EpsilonBeta = 0;
            project.Settings.MaximumIterations = 800;

            foreach (ModelParameter parameter in project.InputParameters)
            {
                project.Settings.SearchParameterSettings.Add(new SearchParameterSettings
                    { Parameter = parameter, MinValue = -100, MaxValue = 100 });
            }

            project.Run();

            OptimizationResult result = project.Result;

            ClassicAssert.AreEqual(2, result.Values.Length);

            ClassicAssert.AreEqual(12.2, result.Values[0], margin);
            ClassicAssert.AreEqual(-8.3, result.Values[1], margin);
            ClassicAssert.AreEqual(4.25, result.MinimumValue, margin);
            ClassicAssert.AreEqual(6375, result.ModelRuns);
        }

    }
}
