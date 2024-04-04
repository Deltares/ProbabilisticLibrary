#include "createDistribution.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"

using namespace Deltares::Reliability;
using namespace Deltares::Statistics;

std::shared_ptr <Stochast> createDistribution::create(const EnumDistributions distHR, double p[4])
{
    DistributionType dist;
    bool truncated = false;
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
    case EnumDistributions::uspace: {
        dist = DistributionType::Normal;
        std::vector<double> params {0.0, 1.0};
        std::shared_ptr<Stochast> s(new Stochast(dist, params));
        return s; }
        break;
    default:
        throw probLibException("Unknown distribution function - code: ", (int)distHR);
    }

    std::vector<double> pValues(4);
    for (int i = 0; i < 4; i++)
    {
        pValues[i] = p[i];
    }

    std::shared_ptr<Stochast> s(new Stochast(dist, pValues));

    if (truncated)
    {
        s->setTruncated(true);
        s->getProperties()->Minimum = truncatedMin;
        s->getProperties()->Maximum = truncatedMax;
    }

    return s;
}

