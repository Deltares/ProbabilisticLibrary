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
			static double getPFromU(const double u);
			static double getQFromU(const double u);
			static double getUFromP(const double p);
			static double getUFromQ(const double q);
			static double getRFromP(const double p);
			static double getPFromR(const double r);
			static void getPQfromU(const double u, double& p, double& q);
			static double ReturnTimeFromBeta(const double beta);
			static double FreqFromBeta(const double beta);
			static double LogQFromBeta(const double beta);

			static constexpr double BetaMax = 40;
			static constexpr double UMax = 8;
		};
	}
}

