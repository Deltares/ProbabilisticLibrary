using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestAdaptiveImportanceSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.56, designPoint.Beta, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmall()
        {
            var project = ProjectBuilder.GetLinearSmallProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.InternalSettings.MaximumSamples = 10000;
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.88, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlow()
        {
            Project project = ProjectBuilder.GetLinearSmallSlowProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.InternalSettings.MaximumSamples = 100;
            importanceSampling.InternalSettings.MinimumSamples = 100;
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.InternalSettings.MaximumSamplesNoResult = 100;
            importanceSampling.InternalSettings.MaximumSamples = 200;

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.96, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlowParallel()
        {
            Project project = ProjectBuilder.GetLinearSmallSlowProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.InternalSettings.MaximumSamples = 100;
            importanceSampling.InternalSettings.MinimumSamples = 100;
            importanceSampling.InternalSettings.RunSettings.MaxParallelProcesses = 4;
            importanceSampling.Settings.RunSettings.MaxParallelProcesses = 4;
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.InternalSettings.MaximumSamplesNoResult = 100;
            importanceSampling.InternalSettings.MaximumSamples = 200;

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.96, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearClusters()
        {
            Project project = ProjectBuilder.GetLinearAbsoluteSmallProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.InternalSettings.MaximumSamples = 5000;
            importanceSampling.InternalSettings.MinimumSamples = 1000;

            importanceSampling.InternalSettings.MaximumSamples = 5000;
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.Clustering = true;

            importanceSampling.Settings.ClusterSettings.MaxClusters = 5;
            importanceSampling.Settings.ClusterSettings.OptimizeNumberClusters = false;

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.66, designPoint.Beta, margin);
        }
    }
}