using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using Deltares.Statistics.Wrappers;
using NUnit.Framework;
using System.Linq;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestAdaptiveImportanceSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new AdaptiveImportanceSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.56, designPoint.Beta, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmall()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamples = 10000;
            project.ReliabilityMethod = importanceSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.88, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlow()
        {
            Project project = ProjectBuilder.GetLinearSmallSlowProject();

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamplesNoResult = 100;
            importanceSampling.Settings.ImportanceSamplingSettings.MinimumSamples = 100;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamples = 200;
            project.ReliabilityMethod = importanceSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.96, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallSlowParallel()
        {
            Project project = ProjectBuilder.GetLinearSmallSlowProject();

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();
            importanceSampling.Settings.RunSettings.MaxParallelProcesses = 4;
            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.ImportanceSamplingSettings.MinimumSamples = 100;
            importanceSampling.Settings.ImportanceSamplingSettings.RunSettings.MaxParallelProcesses = 4;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamplesNoResult = 100;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamples = 200;

            project.ReliabilityMethod = importanceSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.96, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearClusters()
        {
            Project project = ProjectBuilder.GetLinearAbsoluteSmallProject();

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();

            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.Clustering = true;

            importanceSampling.Settings.ClusterSettings.MaxClusters = 5;
            importanceSampling.Settings.ClusterSettings.OptimizeNumberClusters = false;

            importanceSampling.Settings.ImportanceSamplingSettings.MinimumSamples = 1000;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamples = 5000;

            project.ReliabilityMethod = importanceSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.66, designPoint.Beta, margin);
        }

        [Test]
        public void TestEdgeClusters()
        {
            Project project = ProjectBuilder.GetEdgeProject();

            AdaptiveImportanceSampling importanceSampling = new AdaptiveImportanceSampling();

            importanceSampling.Settings.MaxVarianceLoops = 5;
            importanceSampling.Settings.MinVarianceLoops = 2;
            importanceSampling.Settings.Clustering = true;

            importanceSampling.Settings.ClusterSettings.MaxClusters = 5;
            importanceSampling.Settings.ClusterSettings.OptimizeNumberClusters = true;

            importanceSampling.Settings.ImportanceSamplingSettings.MinimumSamples = 1000;
            importanceSampling.Settings.ImportanceSamplingSettings.MaximumSamples = 5000;

            project.ReliabilityMethod = importanceSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            double pSingle = 0.0001;
            double betaExpected = StandardNormal.GetUFromQ(3 * pSingle - 3 * pSingle * pSingle);
            Assert.AreEqual(betaExpected, designPoint.Beta, 2 * margin);

            Assert.AreEqual(3, designPoint.ContributingDesignPoints.Count(p => p.Identifier.StartsWith("Cluster")));
        }
    }
}