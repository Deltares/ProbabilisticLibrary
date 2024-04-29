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

    std::shared_ptr <Stochast> s = std::make_shared<Stochast>();
    create(s, distHR, pValues);
    if (!s->isValid())
    {
        throw probLibException("parameters are not valid for distribution.");
    }
    return s;
}

void createDistribution::create(std::shared_ptr <Stochast> & s, const EnumDistributions distHR, std::vector<double> p)
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

    if (setShapeScaleShift)
    {
        std::shared_ptr< StochastProperties> properties = std::make_shared< StochastProperties>();
        properties->Scale = p[0];
        properties->Shape = p[1];
        properties->Shift = p[2];
        properties->ShapeB = p[3];
        std::shared_ptr<Stochast> s2(new Stochast(dist, properties));
        s.swap(s2);
    }
    else
    {
        std::shared_ptr<Stochast> s2(new Stochast(dist, p));
        s.swap(s2);
    }

    if (truncated)
    {
        s->setTruncated(true);
        s->getProperties()->Minimum = truncatedMin;
        s->getProperties()->Maximum = truncatedMax;
    }

}

