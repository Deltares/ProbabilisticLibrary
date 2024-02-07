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

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            DirectionalSampling directionalSampling = new DirectionalSampling();
            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            DirectionalSampling directionalSampling = new DirectionalSampling();
            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.58, designPoint.Beta, margin);

            DesignPoint designPoint2 = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.58, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            DirectionalSampling directionalSampling = new DirectionalSampling();
            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(-2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearParallel()
        {
            Project project = ProjectBuilder.GetLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.MaxParallelProcesses = 4;

            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.58, designPoint.Beta, margin);
        }

        [Test]
        public void TestDoubleLinear()
        {
            Project project = ProjectBuilder.GetDoubleLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();
            directionalSampling.Settings.MaximumSamples = 2000;
            directionalSampling.Settings.MinimumSamples = 2000;
            directionalSampling.Settings.DirectionalSettings.ModelVaryingType = VaryingType.Varying;

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.20, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestBlock()
        {
            Project project = ProjectBuilder.GetBlockProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestSemiBlock()
        {
            Project project = ProjectBuilder.GetSemiBlockProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.50, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.89, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.13, designPoint.Beta, margin);
        }

        [Test]
        public void TestSaddle()
        {
            Project project = ProjectBuilder.GetSaddleProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.34, designPoint.Beta, margin);
        }

        [Test]
        public void TestTwoBranches()
        {
            Project project = ProjectBuilder.GetTwoBranchesProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(5.01, designPoint.Beta, margin);
        }

        [Test] 
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            DirectionalSampling directionalSampling = new DirectionalSampling();

            DesignPoint designPoint = directionalSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.66, designPoint.Beta, margin);
            Assert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }
    }
}