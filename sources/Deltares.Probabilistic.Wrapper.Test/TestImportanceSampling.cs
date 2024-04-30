using Deltares.Reliability.Wrappers;
using Deltares.Models.Wrappers;

using NUnit.Framework;
using System.Linq;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestImportanceSampling
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(177.4, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.71, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(-2.54, designPoint.Beta, margin);

            Assert.AreEqual(0.71, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            DirectionReliability direction = new DirectionReliability();
            direction.Settings.SetStartPoint(designPoint);

            DesignPoint limitStatePoint = direction.GetDesignPoint(modelRunner);

            var z = project.ZFunction.Invoke(limitStatePoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(0, z.Z, margin);

            Assert.AreEqual(0.90, limitStatePoint.Alphas[0].X, margin);
            Assert.AreEqual(0.90, limitStatePoint.Alphas[1].X, margin);
        }



        [Test]
        public void TestLinearMessages()
        {
            Project project = ProjectBuilder.GetLinearProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            modelRunner.Settings.SaveEvaluations = true;
            modelRunner.Settings.SaveConvergence = true;

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(1559, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(10001, designPoint.Evaluations.Count);

            Assert.IsTrue(!double.IsNaN(designPoint.Evaluations[1].Weight));
        }

        [Test]
        public void TestLinearSmall()
        {
            Project project = ProjectBuilder.GetLinearSmallProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            importanceSampling.Settings.MaximumSamples = 10000;
            importanceSampling.Settings.MinimumSamples = 5000;

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                importanceSampling.Settings.StochastSettings.Add(new StochastSettings { StartValue = 0, VarianceFactor = 2, Stochast = project.Stochasts[i] });
            }

            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.89, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearCorrelated()
        {
            Project project = ProjectBuilder.GetLinearFullyCorrelatedProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);
        }

        [Test]
        public void TestBligh()
        {
            Project project = ProjectBuilder.GetBlighProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.67, designPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrength()
        {
            Project project = ProjectBuilder.GetLoadStrengthProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.42, designPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrengthSurvived()
        {
            Project project = ProjectBuilder.GetLoadStrengthSurvivedProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.79, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvex()
        {
            Project project = ProjectBuilder.GetConvexProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.62, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();
            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            ImportanceSampling importanceSampling = new ImportanceSampling();
            DesignPoint designPoint = importanceSampling.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.49, designPoint.Beta, margin);
        }
    }
}