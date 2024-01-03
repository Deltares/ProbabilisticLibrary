#include "pch.h"
#include "Stochast.h"

#include "DeterministicDistribution.h"
#include "NormalDistribution.h"
#include "LogNormalDistribution.h"
#include "UniformDistribution.h"


void Stochast::updateDistribution()
{
	switch (this->distributionType)
	{
		case Deterministic: this->distribution = new DeterministicDistribution(); break;
		case Normal: this->distribution = new NormalDistribution(); break;
		case LogNormal: this->distribution = new LogNormalDistribution(); break;
		case Uniform: this->distribution = new UniformDistribution(); break;
	}
}


