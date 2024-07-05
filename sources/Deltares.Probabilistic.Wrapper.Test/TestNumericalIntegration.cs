using NUnit.Framework;
using Deltares.Reliability.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestNumericalIntegration
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new NumericalIntegration();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.57, designPoint.Beta, margin);
            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(199.4, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.71, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new NumericalIntegration();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-2.57, designPoint.Beta, margin);

            Assert.AreEqual(0.71, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.MaxParallelProcesses = 4;

            project.ReliabilityMethod = new NumericalIntegration();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.57, designPoint.Beta, margin);

            Assert.AreEqual(0, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(0, designPoint.Evaluations.Count);
        }

        [Test]
        public void TestLinearLimited()
        {
            var project = ProjectBuilder.GetLinearProject();

            var numericalIntegration = new NumericalIntegration();

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                numericalIntegration.Settings.StochastSettings.Add(new StochastSettings { UMin = 1, Stochast = project.Stochasts[i] });
            }

            project.ReliabilityMethod = numericalIntegration;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.57, designPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrength()
        {
            var project = ProjectBuilder.GetLoadStrengthProject();

            project.ReliabilityMethod = new NumericalIntegration();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.43, designPoint.Beta, margin);
        }

        [Test]
        public void TestLoadStrengthSurvived()
        {
            var project = ProjectBuilder.GetLoadStrengthSurvivedProject();

            project.ReliabilityMethod = new NumericalIntegration();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.66, designPoint.Beta, margin);
        }
    }
}