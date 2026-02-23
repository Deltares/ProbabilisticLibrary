using Deltares.Probabilistic.Statistics;

namespace Deltares.Probabilistic.Model;

internal interface IStochastProvider
{
    Stochast GetStochast(int stochastId);
}