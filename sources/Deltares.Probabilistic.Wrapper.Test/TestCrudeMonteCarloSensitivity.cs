using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Statistics.Wrappers;
using Deltares.Sensitivity.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestCrudeMonteCarloSensitivity
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            Stochast stochast2 = project.GetStochast();

            Assert.AreNotSame(stochast, stochast2);
            Assert.AreEqual(stochast.Mean, stochast2.Mean);
        }

        [Test]
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.Settings.MaxParallelProcesses = 4;

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearManySamples()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());
            project.Settings.SaveConvergence = false;
            project.Settings.SaveEvaluations = false;
            project.Settings.SaveMessages = false;

            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.MinimumSamples = 100000;
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.MaximumSamples = ((CrudeMonteCarloS)project.SensitivityMethod).Settings.MinimumSamples;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearFullyCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearNegativeFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearNegativeFullyCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.04, stochast.Mean, margin);
            Assert.AreEqual(0.59, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearPartialCorrelatedProject());
            project.SensitivityMethod = new CrudeMonteCarloS();
            ((CrudeMonteCarloS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.76, stochast.Deviation, margin);
        }
    }
}