using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Statistics.Wrappers;
using Deltares.Sensitivity.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestImportanceSamplingSensitivity
    {
        private const double margin = 0.02;

        [Test]
        public void TestAddOne()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetAddOneProject());

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1, stochast.Mean, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(0, stochast.Minimum, margin);
            Assert.AreEqual(2, stochast.Maximum, margin);
        }

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearProject());

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

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

            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.MaximumSamples = 100000;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.8, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            Assert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearFullyCorrelatedProject());
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.82, stochast.Deviation, margin);

            stochast.DistributionType = DistributionType.Uniform;

            Assert.AreEqual(-0.2, stochast.Minimum, 10 * margin);
            Assert.AreEqual(3.8, stochast.Maximum, 10 * margin);
        }

        [Test]
        public void TestLinearNegativeFullyCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearNegativeFullyCorrelatedProject());
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.04, stochast.Mean, margin);
            Assert.AreEqual(0.59, stochast.Deviation, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetSensitivityProject(ProjectBuilder.GetLinearPartialCorrelatedProject());
            project.SensitivityMethod = new ImportanceSamplingS();
            ((ImportanceSamplingS)project.SensitivityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            Stochast stochast = project.GetStochast();

            Assert.AreEqual(1.79, stochast.Mean, margin);
            Assert.AreEqual(0.76, stochast.Deviation, margin);
        }
    }
}