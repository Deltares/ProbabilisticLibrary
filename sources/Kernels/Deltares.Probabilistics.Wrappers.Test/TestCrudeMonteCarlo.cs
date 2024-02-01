using System;
using System.Collections.Generic;
using System.Linq;
using NUnit.Framework;
using Deltares.Probabilistic.Kernels;

namespace Deltares.Probabilistics.Wrappers.Test
{
    [TestFixture]
    public class TestCrudeMonteCarlo
    {
        private const double margin = 0.01;

        [Test] 
        public void TestLinear()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);
            Assert.AreEqual(0.005, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.995, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(178.1, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.73, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            DirectionReliabilityWrapper direction = new DirectionReliabilityWrapper();
            direction.Settings.SetStartPoint(designPoint);

            DesignPointWrapper limitStatePoint = direction.GetDesignPoint(modelRunner);

            var z = project.ZFunction.Invoke(limitStatePoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(0, z.Z, margin);

            Assert.AreEqual(0.89, limitStatePoint.Alphas[0].X, margin);
            Assert.AreEqual(0.91, limitStatePoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            DesignPointWrapper designPoint2 = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
        }

        [Test] // c++
        public void TestInverseLinear()
        {
            var project = ProjectBuilder.GetInverseLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(-2.54, designPoint.Beta, margin);

            Assert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.73, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.92, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.93, designPoint.Alphas[1].X, margin);

            DirectionReliabilityWrapper direction = new DirectionReliabilityWrapper();
            direction.Settings.SetStartPoint(designPoint);

            DesignPointWrapper limitStatePoint = direction.GetDesignPoint(modelRunner);

            var z = project.ZFunction.Invoke(limitStatePoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(0, z.Z, margin);

            Assert.AreEqual(0.89, limitStatePoint.Alphas[0].X, margin);
            Assert.AreEqual(0.91, limitStatePoint.Alphas[1].X, margin);
        }

        [Test] // c++
        public void TestLinearParallel()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.MaxParallelProcesses = 4;

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(0, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(0, designPoint.Evaluations.Count);
        }

        [Test] // c++
        public void TestLinearMessages()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.SaveEvaluations = true;
            modelRunner.Settings.SaveConvergence = true;

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(1106, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(10001, designPoint.Evaluations.Count);
        }

        [Test] // c++
        public void TestLinearProgress()
        {
            var project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();
            ProgressIndicatorWrapper progressIndicator = new ProgressIndicatorWrapper(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, progressIndicator);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);

            Assert.AreEqual(10000, progressHolder.Invocations);
            Assert.AreEqual(1, progressHolder.Progress, margin);
            Assert.AreEqual("10000/10000, Reliability = 2.536, Convergence = 0.133", progressHolder.Text);
        }

        [Test] // c++
        public void TestLinearLimited()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            crudeMonteCarlo.Settings.MaximumSamples = 1000;

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                crudeMonteCarlo.Settings.StochastSettings.Add(new StochastSettingsWrapper {UMin = 1});
            }

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.54, designPoint.Beta, margin);
        }

        [Test] // c++
        public void TestLinearManySamples()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.SaveConvergence = false;
            modelRunner.Settings.SaveEvaluations = false;
            modelRunner.Settings.SaveMessages = false;

            CrudeMonteCarloWrapper monteCarlo = new CrudeMonteCarloWrapper();
            monteCarlo.Settings.MinimumSamples = 100000;
            monteCarlo.Settings.MaximumSamples = monteCarlo.Settings.MinimumSamples;

            DesignPointWrapper designPoint = monteCarlo.GetDesignPoint(modelRunner);
            //var designPoint = Reliability.GetDesignPoint(monteCarlo, project.ZFunction, project.Stochasts);

            Assert.AreEqual(2.57, designPoint.Beta, margin);

            var zDesignPoint = project.ZFunction.Invoke(designPoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(-0.06, zDesignPoint.Z, margin);

            DirectionReliabilityWrapper direction = new DirectionReliabilityWrapper();
            direction.Settings.SetStartPoint(designPoint);

            DesignPointWrapper limitStatePoint = direction.GetDesignPoint(modelRunner);
            //var limitStatePoint = Reliability.GetLimitStatePoint(designPoint, project.ZFunction, project.Stochasts);

            var zLimitState = project.ZFunction.Invoke(limitStatePoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(0, zLimitState.Z, margin);
        }

        [Test] // c++
        public void TestLinearFullyCorrelated()
        {
            var project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

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

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

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

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

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

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.17, designPoint.Beta, margin);
        }

        [Test] // c++
        public void TestNonLinear()
        {
            var project = ProjectBuilder.GetNonLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.72, designPoint.Beta, margin);

            DirectionReliabilityWrapper direction = new DirectionReliabilityWrapper();
            direction.Settings.SetStartPoint(designPoint);

            DesignPointWrapper limitStatePoint = direction.GetDesignPoint(modelRunner);

            var z = project.ZFunction.Invoke(limitStatePoint.Alphas.Select(p => p.X).ToArray());
            Assert.AreEqual(0, z.Z, margin);
        }

        [Test] // c++
        public void TestBligh()
        {
            var project = ProjectBuilder.GetBlighProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.63, designPoint.Beta, margin);
            Assert.AreEqual(project.Stochasts.Count, designPoint.Alphas.Count, margin);
        }

        [Test] // c++
        public void TestLoadStrength()
        {
            var project = ProjectBuilder.GetLoadStrengthProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            //var designPoint = Reliability.GetDesignPoint(new CrudeMonteCarlo(), project.ZFunction, project.Stochasts);

            Assert.AreEqual(1.38, designPoint.Beta, margin);
        }

        [Test] // c++
        public void TestLoadStrengthSurvived()
        {
            var project = ProjectBuilder.GetLoadStrengthSurvivedProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            //var designPoint = Reliability.GetDesignPoint(new CrudeMonteCarlo(), project.ZFunction, project.Stochasts);

            Assert.AreEqual(1.87, designPoint.Beta, margin);
        }

        [Test] // c++
        public void TestConvex()
        {
            var project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.64, designPoint.Beta, margin);
        }

        [Test]
        public void TestManyVars()
        {
            Project project = ProjectBuilder.GetManyVarsProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.16, designPoint.Beta, margin);
        }

        [Test] // c++
        public void TestNoisy()
        {
            var project = ProjectBuilder.GetNoisyProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.51, designPoint.Beta, margin);
        }

        [Test]
        public void TestSeries()
        {
            var project = ProjectBuilder.GetSeriesProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            crudeMonteCarlo.Settings.MaximumSamples = 100000;

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.84, designPoint.Beta, margin);
        }

        [Test]
        public void TestDiscrete()
        {
            var project = ProjectBuilder.GetDiscreteProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            crudeMonteCarlo.Settings.MinimumSamples = 10000;

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

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

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();
            crudeMonteCarlo.Settings.MinimumSamples = 10000;

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.43, designPoint.Beta, margin);

            Assert.AreEqual(0.69, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.72, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(0.16, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.15, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(7, designPoint.Alphas[2].X, margin); // wrong, should be 7
        }

        [Test]
        public void TestNoisyStochastSettings()
        {
            var project = ProjectBuilder.GetNoisyProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            for (int i = 0; i < project.Stochasts.Count; i++)
            {
                crudeMonteCarlo.Settings.StochastSettings.Add(new StochastSettingsWrapper());
            }

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.51, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisyProgressIndicator()
        {
            var project = ProjectBuilder.GetNoisyProject();

            ProgressIndication progress = new ProgressIndication();
            ProgressIndicatorWrapper progressIndicator = new ProgressIndicatorWrapper(progress.DoProgress, null, progress.DoTextualProgress);

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, progressIndicator);

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.51, designPoint.Beta, margin);

            Assert.AreEqual(10000, progress.Count);
            Assert.AreEqual("10000/10000, Reliability = 2.506, Convergence = 0.128", progress.LastText);
        }

        [Test]
        public void TestNoisyMessages()
        {
            var project = ProjectBuilder.GetNoisyProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.SaveConvergence = true;
            modelRunner.Settings.SaveEvaluations = true;

            CrudeMonteCarloWrapper crudeMonteCarlo = new CrudeMonteCarloWrapper();

            DesignPointWrapper designPoint = crudeMonteCarlo.GetDesignPoint(modelRunner);

            // call garbage collector, subsequent tests test whether tag is not freed 
            GC.Collect();

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

            public void DoTextualProgress(Deltares.Probabilistic.Kernels.ProgressTextType progressType, string text)
            {
                LastText = text;
            }
        }
    }
}