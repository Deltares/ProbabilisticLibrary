using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;
using NUnit.Framework;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestDirectionalSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.58, designPoint.Beta, margin);

            DesignPoint designPoint2 = project.GetDesignPoint();

            Assert.AreEqual(2.58, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearParallel()
        {
            Project project = ProjectBuilder.GetLinearProject();

            project.Settings.MaxParallelProcesses = 4;

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestDoubleLinear()
        {
            Project project = ProjectBuilder.GetDoubleLinearProject();

            DirectionalSampling directionalSampling = new DirectionalSampling();
            directionalSampling.Settings.MaximumSamples = 2000;
            directionalSampling.Settings.MinimumSamples = 2000;
            directionalSampling.Settings.DirectionalSettings.ModelVaryingType = VaryingType.Varying;

            project.ReliabilityMethod = directionalSampling;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.20, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestBlock()
        {
            Project project = ProjectBuilder.GetBlockProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestSemiBlock()
        {
            Project project = ProjectBuilder.GetSemiBlockProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.50, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.89, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.13, designPoint.Beta, margin);
        }

        [Test]
        public void TestSaddle()
        {
            Project project = ProjectBuilder.GetSaddleProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.34, designPoint.Beta, margin);
        }

        [Test]
        public void TestTwoBranches()
        {
            Project project = ProjectBuilder.GetTwoBranchesProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(5.01, designPoint.Beta, margin);
        }

        [Test] 
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new DirectionalSampling();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.66, designPoint.Beta, margin);
            Assert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }
    }
}