#pragma once

namespace Deltares
{
	namespace Statistics
	{
		class StandardNormal
		{
		private:
			static void normp(const double z, double& p, double& q, double& pdf);
			static constexpr double qMin = 1.0e-300;

		public:
			static double getPFromU(double u);
			static double getQFromU(double u);
			static double getUFromP(double p);
			static double getUFromQ(double q);
			static double getRFromP(double p);
			static double getPFromR(double r);

			static constexpr double BetaMax = 40;
			static constexpr double UMax = 8;
		};
	}
}

