#pragma once
namespace Deltares
{
	namespace Models
	{
		class ProxySettings
		{
		public:
			bool IsProxyModel = false;
			bool ShouldUpdateFinalSteps = false;
			double ThresholdOffset = 0;

			bool isValid()
			{
				return !IsProxyModel ||
				       ThresholdOffset >= 0;
			}
		};
	}
}

