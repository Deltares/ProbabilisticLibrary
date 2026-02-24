namespace Deltares.Probabilistic.Reliability;

internal interface IDesignPointProvider
{
    DesignPoint GetDesignPoint(int designPointId);
}