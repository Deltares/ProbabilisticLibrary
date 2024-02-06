#include "createDistribution.h"
#include "../Deltares.Probabilistic/Utils/probLibException.h"

using namespace Deltares::ProbLibCore;
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
        auto params = new double[2] {1.0, 0.0};
        std::shared_ptr<Stochast> s(new Stochast(dist, params));
        delete[] params;
        return s; }
        break;
    default:
        throw probLibException("Unknown distribution function - code: ", (int)distHR);
    }

    std::shared_ptr<Stochast> s(new Stochast(dist, p));

    if (truncated)
    {
        s->setTruncated(true);
        s->Minimum = truncatedMin;
        s->Maximum = truncatedMax;
    }

    return s;
}

