using System;
using System.Threading;
using NUnit.Framework;
using Deltares.Models.Wrappers;
using Deltares.Reliability.Wrappers;

namespace Deltares.Probabilistic.Wrapper.Test
{
    [TestFixture]
    public class TestCrudeMonteCarlo
    {
        private const double margin = 0.01;

        [Test] 
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            ((CrudeMonteCarlo)project.ReliabilityMethod).Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.58, designPoint.Beta, margin);
            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(203.6, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.70, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            DirectionReliability direction = new DirectionReliability();
            direction.Settings.SetStartPoint(designPoint);
            project.ReliabilityMethod = direction;

            DesignPoint limitStatePoint = project.GetDesignPoint();

            ModelSample sample = limitStatePoint.GetModelSample();
            project.ZFunction.Invoke(sample);
            Assert.AreEqual(0, sample.Z, margin);

            Assert.AreEqual(0.89, limitStatePoint.Alphas[0].X, margin);
            Assert.AreEqual(0.91, limitStatePoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            DesignPoint designPoint2 = project.GetDesignPoint();

            Assert.AreEqual(2.54, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test] 
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(-2.54, designPoint.Beta, margin);

            Assert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.73, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            DirectionReliability direction = new DirectionReliability();
            direction.Settings.SetStartPoint(designPoint);
            project.ReliabilityMethod = direction;

            DesignPoint limitStatePoint = project.GetDesignPoint();

            ModelSample sample = limitStatePoint.GetModelSample();
            project.ZFunction.Invoke(sample);
            Assert.AreEqual(0, sample.Z, margin);

            Assert.AreEqual(0.89, limitStatePoint.Alphas[0].X, margin);
            Assert.AreEqual(0.91, limitStatePoint.Alphas[1].X, margin);
        }

        [Test] 
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.MaxParallelProcesses = 4;

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(0, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(0, designPoint.Evaluations.Count);
        }

        [Test] 
        public void TestLinearMessages()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.SaveEvaluations = true;
            project.Settings.SaveConvergence = true;

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            // call garbage collector, subsequent tests test whether tag is not freed 
            GC.Collect();
            Thread.Sleep(10);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(1106, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(10001, designPoint.Evaluations.Count);
        }

        [Test] 
        public void TestLinearProgress()
        {
            var project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();
            project.ProgressIndicator = new ProgressIndicator(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(10000, progressHolder.Invocations);
            Assert.AreEqual(1, progressHolder.Progress, margin);
            Assert.AreEqual("10000/10000, Reliability = 2.536, Convergence = 0.133", progressHolder.Text);
        }

        [Test] 
        public void TestLinearLimited()
        {
            var project = ProjectBuilder.GetLinearProject();

            CrudeMonteCarlo crudeMonteCarlo = new CrudeMonteCarlo();
            crudeMonteCarlo.Settings.MaximumSamples = 1000;
            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                crudeMonteCarlo.Settings.StochastSettings.Add(new StochastSettings {UMin = 1, Stochast = project.Stochasts[i] });
            }

            project.ReliabilityMethod = crudeMonteCarlo;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.54, designPoint.Beta, margin);
        }

        [Test]
        public void TestLinearManySamples()
        {
            var project = ProjectBuilder.GetLinearProject();

            project.Settings.SaveConvergence = false;
            project.Settings.SaveEvaluations = false;
            project.Settings.SaveMessages = false;

            CrudeMonteCarlo monteCarlo = new CrudeMonteCarlo();
            monteCarlo.Settings.MinimumSamples = 100000;
            monteCarlo.Settings.MaximumSamples = monteCarlo.Settings.MinimumSamples;
            monteCarlo.Settings.RandomSettings.RandomGeneratorType = RandomGeneratorType.MersenneTwister;

            project.ReliabilityMethod = monteCarlo;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.57, designPoint.Beta, margin);

            ModelSample sample = designPoint.GetModelSample();
            project.ZFunction.Invoke(sample);
            Assert.AreEqual(-0.06, sample.Z, margin);

            DirectionReliability direction = new DirectionReliability();
            direction.Settings.SetStartPoint(designPoint);
            project.ReliabilityMethod = direction;

            DesignPoint limitStatePoint = project.GetDesignPoint();

            ModelSample directionSample = limitStatePoint.GetModelSample();
            project.ZFunction.Invoke(directionSample);
            Assert.AreEqual(0, directionSample.Z, margin);
        }

        [Test] 
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.53, designPoint.Beta, margin);

            Assert.AreEqual(1.78, designPoint.Alphas[0].U, margin);
            Assert.AreEqual(0, designPoint.Alphas[1].U, margin);
            Assert.AreEqual(1.80, designPoint.Alphas[2].U, margin);

            Assert.AreEqual(-0.70, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(-0.70, designPoint.Alphas[0].AlphaCorrelated, margin);
            Assert.AreEqual(-0.70, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(-0.71, designPoint.Alphas[2].AlphaCorrelated, margin);

            Assert.AreEqual(0.46, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.46, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.92, designPoint.Alphas[2].X, margin);
        }

        [Test]
        public void TestLinearNegativeFullyCorrelated()
        {
            var project = ProjectBuilder.GetLinearNegativeFullyCorrelatedProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.07, designPoint.Beta, margin);

            Assert.AreEqual(-1.10, designPoint.Alphas[0].U, margin);
            Assert.AreEqual(0, designPoint.Alphas[1].U, margin);
            Assert.AreEqual(1.76, designPoint.Alphas[2].U, margin);

            Assert.AreEqual(0.53, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(-0.84, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(0.53, designPoint.Alphas[0].AlphaCorrelated, margin);
            Assert.AreEqual(-0.53, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(-0.84, designPoint.Alphas[2].AlphaCorrelated, margin);

            Assert.AreEqual(-0.36, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(1.29, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.92, designPoint.Alphas[2].X, margin);
        }

        [Test]
        public void TestLinearPartialCorrelated()
        {
            var project = ProjectBuilder.GetLinearPartialCorrelatedProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.85, designPoint.Beta, margin);

            Assert.AreEqual(1.93, designPoint.Alphas[0].U, margin);
            Assert.AreEqual(0.96, designPoint.Alphas[1].U, margin);
            Assert.AreEqual(1.85, designPoint.Alphas[2].U, margin);

            Assert.AreEqual(-0.68, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.33, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(-0.65, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(-0.68, designPoint.Alphas[0].AlphaCorrelated, margin);
            Assert.AreEqual(-0.63, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(-0.65, designPoint.Alphas[2].AlphaCorrelated, margin);

            Assert.AreEqual(0.47, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.46, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[2].X, margin);
        }

        [Test]
        public void TestDoubleLinear()
        {
            Project project = ProjectBuilder.GetDoubleLinearProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.17, designPoint.Beta, margin);
        }

        [Test] 
        public void TestNonLinear()
        {
            var project = ProjectBuilder.GetNonLinearProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.72, designPoint.Beta, margin);

            DirectionReliability direction = new DirectionReliability();
            direction.Settings.SetStartPoint(designPoint);
            project.ReliabilityMethod = direction;

            DesignPoint limitStatePoint = project.GetDesignPoint();

            ModelSample sample = limitStatePoint.GetModelSample();
            project.ZFunction.Invoke(sample);
            Assert.AreEqual(0, sample.Z, margin);
        }

        [Test] 
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.63, designPoint.Beta, margin);
            Assert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }

        [Test] 
        public void TestLoadStrength()
        {
            var project = ProjectBuilder.GetLoadStrengthProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.38, designPoint.Beta, margin);
        }

        [Test] 
        public void TestLoadStrengthSurvived()
        {
            var project = ProjectBuilder.GetLoadStrengthSurvivedProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.87, designPoint.Beta, margin);
        }

        [Test] 
        public void TestConvex()
        {
            var project = ProjectBuilder.GetConvexProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();
            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.64, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(3.16, designPoint.Beta, margin);
        }

        [Test] 
        public void TestNoisy()
        {
            var project = ProjectBuilder.GetNoisyProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.51, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            CrudeMonteCarlo crudeMonteCarlo = new CrudeMonteCarlo();
            crudeMonteCarlo.Settings.MaximumSamples = 100000;

            project.ReliabilityMethod = crudeMonteCarlo;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.84, designPoint.Beta, margin);
        }

        [Test]
        public void TestDiscrete()
        {
            var project = ProjectBuilder.GetDiscreteProject();

            CrudeMonteCarlo crudeMonteCarlo = new CrudeMonteCarlo();
            crudeMonteCarlo.Settings.MinimumSamples = 10000;

            project.ReliabilityMethod = crudeMonteCarlo;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.43, designPoint.Beta, margin);

            Assert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.72, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0.02, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(0.16, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.15, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(6, designPoint.Alphas[2].X, margin);
        }

        [Test]
        public void TestQualitative()
        {
            var project = ProjectBuilder.GetQualitativeProject();

            CrudeMonteCarlo crudeMonteCarlo = new CrudeMonteCarlo();
            crudeMonteCarlo.Settings.MinimumSamples = 10000;

            project.ReliabilityMethod = crudeMonteCarlo;

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.43, designPoint.Beta, margin);

            Assert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.72, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(0.16, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.15, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(7, designPoint.Alphas[2].X, margin); 
        }

        [Test]
        public void TestVariable()
        {
            var project = ProjectBuilder.GetVariableProject();

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(1.71, designPoint.Beta, margin);

            Assert.AreEqual(-0.04, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.68, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(-0.73, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(8.51, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(8.50, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(2.37, designPoint.Alphas[2].X, margin);
        }

        [Test]
        public void TestNoisyStochastSettings()
        {
            var project = ProjectBuilder.GetNoisyProject();

            CrudeMonteCarlo crudeMonteCarlo = new CrudeMonteCarlo();

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                crudeMonteCarlo.Settings.StochastSettings.Add(new StochastSettings {Stochast = project.Stochasts[i] });
            }

            project.ReliabilityMethod = crudeMonteCarlo;


            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.51, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisyProgressIndicator()
        {
            var project = ProjectBuilder.GetNoisyProject();

            ProgressIndication progress = new ProgressIndication();
            project.ProgressIndicator = new ProgressIndicator(progress.DoProgress, null, progress.DoTextualProgress);

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            Assert.AreEqual(2.51, designPoint.Beta, margin);

            Assert.AreEqual(10000, progress.Count);
            Assert.AreEqual("10000/10000, Reliability = 2.506, Convergence = 0.128", progress.LastText);
        }

        [Test]
        public void TestNoisyMessages()
        {
            var project = ProjectBuilder.GetNoisyProject();

            project.Settings.SaveConvergence = true;
            project.Settings.SaveEvaluations = true;

            project.ReliabilityMethod = new CrudeMonteCarlo();

            DesignPoint designPoint = project.GetDesignPoint();

            // call garbage collector, subsequent tests test whether tag is not freed 
            GC.Collect();
            Thread.Sleep(10);

            Assert.AreEqual(2.51, designPoint.Beta, margin);

            Assert.AreEqual(1157, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(10001, designPoint.Evaluations.Count);

            Assert.IsTrue(designPoint.Evaluations[0].Tag is ZFunctionOutput);
        }

        private class ProgressIndication
        {
            public double LastValue = 0;
            public string LastText = "";
            public int Count = 0;

            public void DoProgress(double value)
            {
                LastValue = value;
                Count++;
            }

            public void DoTextualProgress(ProgressType progressType, string text)
            {
                LastText = text;
            }
        }
    }
}