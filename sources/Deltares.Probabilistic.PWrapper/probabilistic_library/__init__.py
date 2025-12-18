# Copyright (C) Stichting Deltares. All rights reserved.
#
# This file is part of the Probabilistic Library.
#
# The Probabilistic Library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.
#
# All names, logos, and references to "Deltares" are registered trademarks of
# Stichting Deltares and remain full property of Stichting Deltares at all times.
# All rights reserved.
#
"""The Probabilistic Library provides functionality to perform reliability, uncertainty and sensitivity analyses via a
`probabilistic_library.project.ReliabilityProject`, `probabilistic_library.project.UncertaintyProject` or
`probabilistic_library.project.SensitivityProject`.

Using an externally defined model as a python script, the Probabilistic Library derives input and output values. They will be
provided to the user as `probabilistic_library.statistic.Stochast`s, so that the user can provide uncertainty to them. Correlations
are provided via a `probabilistic_library.statistic.CorrelationMatrix`.

When running an analysis, the result is a `probabilistic_library.reliability.DesignPoint`, `probabilistic_library.uncertainty.UncertaintyResult`
or `probabilistic_library.sensitivity.SensitivityResult`.
"""

__version__ = "25.3.1"

from .interface import *
from .logging import *
from .statistic import *
from .reliability import *
from .uncertainty import *
from .sensitivity import *
from .project import *


