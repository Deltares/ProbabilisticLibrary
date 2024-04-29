#pragma once
#include "ModelSample.h"

namespace Deltares
{
	namespace Models
	{
		namespace Wrappers
		{
			public interface class ICanCalculateBeta
			{
				/// <summary>
				/// Calculates the beta in a direction
				/// </summary>
				/// <param name="sample">Direction</param>
				/// <param name="beta">Uncorrelated beta</param>
				/// <returns></returns>
				double GetBeta(ModelSample^ sample, double beta);

				/// <summary>
				/// Indicates whether the beta in a direction can be calculated
				/// </summary>
				/// <returns></returns>
				bool CanCalculateDirection();
			};
		}
	}
}

