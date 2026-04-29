// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
using Deltares.Probabilistic.Logging;
using Deltares.Probabilistic.Model;
using NUnit.Framework;
using NUnit.Framework.Legacy;

namespace Deltares.Probabilistic.Test;

[TestFixture]
public class TestRunProject
{
    private const double margin = 0.01;

    [Test]
    public void TestLinearMeanValues()
    {
        var project = ProjectBuilder.GetRunProject(ProjectBuilder.GetLinearAndUnbalancedProject());
        project.OutputParameters.Add(new ModelParameter { Name = "Linear" });
        project.OutputParameters.Add(new ModelParameter { Name = "Unbalanced" });

        project.Settings.RunValuesType = RunValuesType.MeanValues;
        project.Run();

        Evaluation eval = project.Realization;

        ClassicAssert.AreEqual(2, eval.InputValues.Length);
        ClassicAssert.AreEqual(2, eval.OutputValues.Length);

        ClassicAssert.AreEqual(1.8, eval.OutputValues[0], margin);
        ClassicAssert.AreEqual(1.8, eval.OutputValues[1], margin);
    }

    [Test]
    public void TestLinearDesignValues()
    {
        var project = ProjectBuilder.GetRunProject(ProjectBuilder.GetLinearAndUnbalancedProject());
        project.OutputParameters.Add(new ModelParameter { Name = "Linear" });
        project.OutputParameters.Add(new ModelParameter { Name = "Unbalanced" });

        project.Stochasts[0].DesignQuantile = 0.1;
        project.Stochasts[0].DesignFactor = 2;

        project.Stochasts[1].DesignQuantile = 0.9;
        project.Stochasts[0].DesignFactor = 2.5;

        project.Settings.RunValuesType = RunValuesType.DesignValues;
        project.Run();

        Evaluation eval = project.Realization;

        ClassicAssert.AreEqual(2, eval.InputValues.Length);
        ClassicAssert.AreEqual(2, eval.OutputValues.Length);

        ClassicAssert.AreEqual(1.32, eval.OutputValues[0], margin);
        ClassicAssert.AreEqual(0.456, eval.OutputValues[1], margin);
    }
}