using Deltares.Probabilistic.Kernels;
using NUnit.Framework;

namespace Deltares.Probabilistics.Wrappers.Test
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
            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);

            Assert.AreEqual(0.01, designPoint.ProbabilityFailure, margin / 10);
            Assert.AreEqual(0.99, designPoint.ProbabilityNonFailure, margin / 10);
            Assert.AreEqual(99.7, designPoint.ReturnPeriod, margin * 10);

            Assert.AreEqual(-0.7, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(-0.7, designPoint.Alphas[1].Alpha, margin);

            Assert.AreEqual(0.9, designPoint.Alphas[0].X, margin);
            Assert.AreEqual(0.9, designPoint.Alphas[1].X, margin);
        }

        [Test]
        public void TestLinearDoubleExecuted()
        {
            var project = ProjectBuilder.GetLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint.Beta, margin);

            DesignPointWrapper designPoint2 = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.33, designPoint2.Beta, margin);

            Assert.AreNotSame(designPoint, designPoint2);
            Assert.AreNotSame(designPoint.Alphas[0], designPoint2.Alphas[0]);
            Assert.AreSame(designPoint.Alphas[0].Parameter, designPoint2.Alphas[0].Parameter);
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

            for (int i = 0; i < designPoint.ReliabilityResults.Count; i++)
            {
                Assert.AreEqual(i, designPoint.ReliabilityResults[i].Index);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i].Iteration);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i + 1].Iteration);
                Assert.AreEqual(i, designPoint.Evaluations[3 * i + 2].Iteration);
            }
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
            Project project = ProjectBuilder.GetLinearMultiCorrelatedProject(false);

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
        public void TestLinearMultiCorrelatedSwitchOrder()
        {
            Project project = ProjectBuilder.GetLinearMultiCorrelatedProject(true);

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

            Assert.AreEqual(0.0, designPoint.Alphas[0].Alpha, margin);
            Assert.AreEqual(0.0, designPoint.Alphas[1].Alpha, margin);
            Assert.AreEqual(-1.0, designPoint.Alphas[2].Alpha, margin);

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
        public void TestConvexRayStartPoint()
        {
            Project project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.StochastSettings.Add(new StochastSettingsWrapper { StartValue = 1 });
            form.Settings.StochastSettings.Add(new StochastSettingsWrapper { StartValue = 1 });
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.RaySearch;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRayStartPointNoInitialization()
        {
            Project project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.RaySearch;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.5, designPoint.Beta, margin);
        }

        [Test]
        public void TestConvexRaySphereStartPoint()
        {
            Project project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.SphereSearch;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.5, designPoint.Beta, margin);

            Assert.AreEqual(1, designPoint.ContributingDesignPoints.Count, margin);
            Assert.AreEqual(3.54, designPoint.ContributingDesignPoints[0].Beta, margin);
        }

        [Test]
        public void TestConvexRaySensitivityStartPoint()
        {
            Project project = ProjectBuilder.GetConvexProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.StartPointCalculatorSettings.StartMethod = StartMethodType.SensitivitySearch;

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
        public void TestOblateSpheroid()
        {
            Project project = ProjectBuilder.GetOblateSpheroidProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.39, designPoint.Beta, margin);
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
        public void TestWaveRelaxationLoopsDoubleGradient()
        {
            Project project = ProjectBuilder.GetWaveProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.RelaxationFactor = 0.7;
            form.Settings.RelaxationLoops = 5;
            form.Settings.GradientCalculatorSettings.GradientType = GradientType.TwoDirections;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(2.08, designPoint.Beta, margin);
            Assert.AreEqual(1, designPoint.ContributingDesignPoints.Count);

            Assert.IsTrue(designPoint.ConvergenceReport.IsConverged);
        }

        [Test]
        public void TestNonLinear()
        {
            Project project = ProjectBuilder.GetNonLinearProject();

            ModelRunnerWrapper modelRunner = new ModelRunnerWrapper(project.Function, project.Stochasts, project.CorrelationMatrix, null);

            FORMWrapper form = new FORMWrapper();
            form.Settings.RelaxationFactor = 0.75;
            form.Settings.RelaxationLoops = 8;

            DesignPointWrapper designPoint = form.GetDesignPoint(modelRunner);

            Assert.AreEqual(3.01, designPoint.Beta, margin);
            Assert.AreEqual(7, designPoint.ContributingDesignPoints.Count);
            Assert.AreEqual("Relaxation loop 1", designPoint.ContributingDesignPoints[0].Identifier);

            Assert.IsFalse(designPoint.ConvergenceReport.IsConverged);
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