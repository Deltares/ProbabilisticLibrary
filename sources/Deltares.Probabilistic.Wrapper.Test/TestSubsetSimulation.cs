using Deltares.Models.Wrappers;
using Deltares.Probabilistics.Wrappers.Test;
using Deltares.Reliability.Wrappers;
using NUnit.Framework;
using System.Linq;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestSubsetSimulation
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.52, designPoint.Beta, margin);
            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);

            Assert.AreEqual(-0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearAdaptiveConditional()
        {
            var project = ProjectBuilder.GetLinearProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.59, designPoint.Beta, margin);
            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);

            Assert.AreEqual(-0.71, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.93, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-2.50, designPoint.Beta, margin);

            Assert.AreEqual(0.70, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.92, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmallMarkovChain()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.SampleMethod = SampleMethod.MarkovChain;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.91, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallAdaptiveConditional()
        {
            var project = ProjectBuilder.GetLinearSmallProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.90, designPoint.Beta, margin);

        }

        [Test]
        public void TestManyVars()
        {
            var project = ProjectBuilder.GetManyVarsProject();

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;

            project.ReliabilityMethod = subsetSimulation;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.13, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            var project = ProjectBuilder.GetNonLinearProject();

            project.ReliabilityMethod = new SubsetSimulation();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.47, designPoint.Beta, margin);
        }
    }
}