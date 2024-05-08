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

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

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

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

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

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

            Assert.AreEqual(-2.50, designPoint.Beta, margin);

            Assert.AreEqual(0.70, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.92, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearSmallMarkovChain()
        {
            Project project = ProjectBuilder.GetLinearSmallProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.VariationCoefficientFailure = 0;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;
            subsetSimulation.Settings.SampleMethod = SampleMethod.MarkovChain;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.91, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearSmallAdaptiveConditional()
        {
            Project project = ProjectBuilder.GetLinearSmallProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.MinimumSamples = 500;
            subsetSimulation.Settings.MaximumSamples = 5000;
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.90, designPoint.Beta, margin);

        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            SubsetSimulation subsetSimulation = new SubsetSimulation();
            subsetSimulation.Settings.SampleMethod = SampleMethod.AdaptiveConditional;
            subsetSimulation.Settings.MarkovChainDeviation = 0.5;

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.13, designPoint.Beta, margin);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            ModelRunner modelRunner = new ModelRunner(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            SubsetSimulation subsetSimulation = new SubsetSimulation();

            DesignPoint designPoint = subsetSimulation.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.47, designPoint.Beta, margin);
        }
    }
}