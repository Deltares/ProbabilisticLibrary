using System.Collections.Generic;
using Deltares.Probabilistic.Kernels;
using Deltares.Probabilistics.Wrappers.Test;
using NUnit.Framework;

namespace Deltares.Reliability.Test
{
    [TestFixture]
    public class TestFORM
    {
        private const double margin = 0.01;

        [Test]
        public void TestLinear()
        {
            Project project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint.Beta, margin);
            Assert.AreEqual(0.01, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.99, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(99.7, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.7, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.7, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.9, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.9, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearMessages()
        {
            Project project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.SaveConvergence = true;
            modelRunner.Settings.SaveEvaluations = true;

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint.Beta, margin);

            Assert.AreEqual(6, designPoint.ReliabilityResults.Count);
            Assert.AreEqual(18, designPoint.Evaluations.Count);
        }

        [Test]
        [SetCulture("en-US")]
        public void TestLinearProgress()
        {
            Project project = ProjectBuilder.GetLinearProject();

            ProgressHolder progressHolder = new ProgressHolder();
            ProgressIndicatorWrapper progressIndicator = new ProgressIndicatorWrapper(progressHolder.SetProgress, progressHolder.SetDetailedProgress, progressHolder.SetTextualProgress);

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, progressIndicator);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint.Beta, margin);

            Assert.AreEqual(6, progressHolder.Invocations);
            Assert.AreEqual(0.1, progressHolder.Progress, margin);
            Assert.AreEqual("5/50, Reliability = 2.327, Convergence = 0.009", progressHolder.Text);
            Assert.AreEqual(5, progressHolder.Step);
            Assert.AreEqual(0, progressHolder.Loop);
            Assert.AreEqual(2.327, progressHolder.Reliability, 1e-3);
            Assert.AreEqual(0.009, progressHolder.Convergence, 1e-3);
        }

        [Test]
        public void TestLinearCorrelated()
        {
            Project project = ProjectBuilder.GetLinearFullyCorrelatedProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint.Beta, margin);

            Assert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLinearMultiCorrelated()
        {
            Project project = ProjectBuilder.GetLinearMultiCorrelatedProject();

            //Assert.IsFalse(project.CorrelationMatrix.IsValid());

            project.CorrelationMatrix.ResolveConflictingCorrelations();

            //Assert.IsTrue(project.CorrelationMatrix.IsValid());

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.65, designPoint.Beta, margin);

            Assert.AreEqual(0.45, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.45, designPoint.Alphas[1].X, margin);
            Assert.AreEqual(0.90, designPoint.Alphas[2].X, margin);

            Assert.AreEqual(0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(0, designPoint.Alphas[2].Alpha, margin);

            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[1].AlphaCorrelated, margin);
            Assert.AreEqual(designPoint.Alphas[0].AlphaCorrelated, designPoint.Alphas[2].AlphaCorrelated, margin);
        }

        [Test]
        public void TestLoadStrength()
        {
            Project project = ProjectBuilder.GetLoadStrengthProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.41, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexStartPoint()
        {
            Project project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.StochastSettings.Add(new StochastSettingsWrapper { StartValue = 1 });
            form.Settings.StochastSettings.Add(new StochastSettingsWrapper { StartValue = 1 });

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestNoisy()
        {
            Project project = ProjectBuilder.GetNoisyProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.59, designPoint.Beta, margin);
        }

        [Test]
        public void TestQuadratic()
        {
            Project project = ProjectBuilder.GetQuadraticProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(-4.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestWaveRelaxationLoops()
        {
            Project project = ProjectBuilder.GetWaveProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.09, designPoint.Beta, margin);
            Assert.AreEqual(2, designPoint.ContributingDesignPoints.Count);
        }

        [Test]
        public void TestBligh()
        {
            Project project = ProjectBuilder.GetBlighProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(1.63, designPoint.Beta, margin);
        }

        //[Test]
        //public void TestBlighFragilityCurve()
        //{
        //    Project project = ProjectBuilder.GetBlighProject();

        //    FragilityCurve fragilityCurve = Reliability.GetFragilityCurve(project.Stochasts[0], new List<double> {0, 0.5, 1, 1.25, 1.5, 1.75, 2}, new FORM(), project.ZFunction, project.Stochasts);

        //    Assert.AreEqual(7, fragilityCurve.FragilityValues.Count);

        //    DesignPoint designPoint = Reliability.GetDesignPointFromFragilityCurve(fragilityCurve, project.Stochasts[0]);

        //    Assert.AreEqual(1.55, designPoint.Beta, margin);
        //}

        [Test]
        public void TestNonVarying()
        {
            Project project = ProjectBuilder.GetNonVaryingProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);
            modelRunner.Settings.SaveMessages = true;
            modelRunner.Settings.MaxMessages = 1000;
            modelRunner.Settings.LowestMessageType = MessageType.Warning;

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(40, designPoint.Beta, margin);
            Assert.AreEqual(1, designPoint.Messages.Count);

            Assert.AreEqual(MessageType.Error, designPoint.Messages[0].Type);
            Assert.AreEqual("No variation in model results found at start point", designPoint.Messages[0].Text);
        }
    }
}