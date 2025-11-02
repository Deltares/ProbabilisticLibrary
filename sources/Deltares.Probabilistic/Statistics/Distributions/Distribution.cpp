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
#include "Distribution.h"

#include <algorithm>

#include "../../Math/NumericSupport.h"
#include "../../Math/RootFinders/BisectionRootFinder.h"
#include "../../Utils/probLibException.h"
#include "../StandardNormal.h"
#include "../../Logging/ValidationSupport.h"

#include <numbers>

namespace Deltares
{
    namespace Statistics
    {
        std::vector<double> Distribution::getDiscontinuityPoints(const StochastProperties& stochast)
        {
            std::shared_ptr<StochastProperties> stochastPtr = std::make_shared<StochastProperties>(stochast);

            if (isVarying(stochastPtr))
            {
                return {};
            }
            else
            {
                return {getMean(stochastPtr)};
            }
        }

        double Distribution::getFittedMinimum(std::vector<double>& x)
        {
            double min = Numeric::NumericSupport::getMinimum(x);
            double max = Numeric::NumericSupport::getMaximum(x);

            double diff = max - min;
            double add = diff / x.size();

            return min - add;
        }

        double Distribution::getLogLikelihood(std::shared_ptr<StochastProperties> stochast, double x)
        {
            return log(this->getPDF(stochast, x));
        }

        bool Distribution::isValid(StochastProperties& stochast)
        {
            std::string empty_subject;

            Logging::ValidationReport report;
            this->validate(report, stochast, empty_subject);
            return report.isValid();
        }

        void Distribution::validate(Logging::ValidationReport& report, std::shared_ptr<StochastProperties> stochast, std::string& subject)
        {
            validate(report, *stochast, subject);
        }

        void Distribution::validate(Logging::ValidationReport& report, StochastProperties& stochast, std::string& subject)
        {
            for (DistributionPropertyType property : this->getParameters())
            {
                if (property == Location)
                {
                    Logging::ValidationSupport::checkFinite(report, stochast.Location, "location", subject);
                }
                else if (property == Scale)
                {
                    Logging::ValidationSupport::checkMinimum(report, 0, stochast.Scale, "scale", subject);
                }
                else if (property == Shape)
                {
                    Logging::ValidationSupport::checkMinimum(report, 0, stochast.Shape, "shape", subject);
                }
                else if (property == ShapeB)
                {
                    Logging::ValidationSupport::checkMinimum(report, 0, stochast.ShapeB, "shape B", subject);
                }
                else if (property == Maximum)
                {
                    Logging::ValidationSupport::checkMinimum(report, stochast.Minimum, stochast.Maximum, "maximum", subject);
                }
                else if (property == ShiftB)
                {
                    Logging::ValidationSupport::checkMinimum(report, stochast.Shift, stochast.ShiftB, "shift B", subject);
                }
                else if (property == Observations)
                {
                    Logging::ValidationSupport::checkMinimumInt(report, 2, stochast.Observations, "observations", subject);
                }
            }
        }
    }
}




