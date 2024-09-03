// Copyright (C) Stichting Deltares. All rights reserved.
//
// This file is part of the Probabilistic Library.
//
// The Probabilistic Library is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <http://www.gnu.org/licenses/>.
//
// All names, logos, and references to "Deltares" are registered trademarks of
// Stichting Deltares and remain full property of Stichting Deltares at all times.
// All rights reserved.
//
#include <limits>
#include "createDistribution.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

std::shared_ptr <Stochast> createDistribution::createValid(const EnumDistributions distHR, double p[4])
{
    std::vector<double> pValues(4);
    for (int i = 0; i < 4; i++)
    {
        pValues[i] = p[i];
    }

    auto s = create(distHR, pValues);
    if (!s->isValid())
    {
        throw probLibException("parameters are not valid for distribution.");
    }
    return s;
}

std::shared_ptr <Stochast> createDistribution::create(const EnumDistributions distHR, std::vector<double> p)
{
    DistributionType dist;
    bool truncated = false;
    bool setShapeScaleShift = false;
    double truncatedMin; double truncatedMax;
    switch (distHR)
    {
    case EnumDistributions::normal:
        dist = DistributionType::Normal;
        break;
    case EnumDistributions::deterministic:
        dist = DistributionType::Deterministic;
        break;
    case EnumDistributions::lognormal2:
        dist = DistributionType::LogNormal;
        break;
    case EnumDistributions::uniform:
        dist = DistributionType::Uniform;
        break;
    case EnumDistributions::gumbel2:
        dist = DistributionType::Gumbel;
        break;
    case EnumDistributions::truncatedNormal: {
        dist = DistributionType::Normal;
        truncated = true;
        truncatedMin = p[2];
        truncatedMax = p[3]; }
        break;
    case EnumDistributions::weibull:
        setShapeScaleShift = true;
        dist = DistributionType::Weibull;
        break;
    case EnumDistributions::conditionalWeibull:
        setShapeScaleShift = true;
        dist = DistributionType::ConditionalWeibull;
        break;
    case EnumDistributions::rayleigh:
        setShapeScaleShift = true;
        dist = DistributionType::Rayleigh;
        break;
    case EnumDistributions::RayleighN:
        setShapeScaleShift = true;
        dist = DistributionType::RayleighN;
        break;
    case EnumDistributions::uspace: {
        dist = DistributionType::Normal;
        p = { 0.0, 1.0, 0.0, 0.0 }; }
        break;
    default:
        throw probLibException("Unknown distribution function - code: ", (int)distHR);
    }

    std::shared_ptr<Stochast> stochast;
    if (setShapeScaleShift)
    {
        std::shared_ptr< StochastProperties> properties = std::make_shared< StochastProperties>();
        properties->Scale = p[0];
        properties->Shape = p[1];
        properties->Shift = p[2];
        properties->ShapeB = p[3];
        stochast = std::make_shared<Stochast>(dist, properties);
    }
    else
    {
        stochast = std::make_shared<Stochast>(dist, p);
    }

    if (truncated)
    {
        stochast->setTruncated(true);
        stochast->getProperties()->Minimum = truncatedMin;
        stochast->getProperties()->Maximum = truncatedMax;
    }

    return stochast;
}

