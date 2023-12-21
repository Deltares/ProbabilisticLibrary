#include "fillStartVector.h"
#include "../utils/basic_math.h"

namespace Deltares {
    namespace ProbLibCore {

        vector1D FillStartVector::GetU(const zmodel& w, const stochSettings& s, const int numCores) const
        {
            auto u = vector1D(s.getNrStochasts());
            switch (method)
            {
            case StartMethods::RaySearch:
            case StartMethods::RaySearchVector:
            case StartMethods::RaySearchVectorScaled:
            {
                auto dir = vector1D(s.getNrStochasts());
                double maxdir = -1e300;
                for (size_t i = 0; i < s.getNrAllStochasts(); i++)
                {
                    auto q = s.getStochId(i);
                    if (q.second)
                    {
                        if (method == StartMethods::RaySearch)
                            dir(q.first) = (startVector[i] != 0.0 ? 1.0 : 0.0);
                        else
                        {
                            dir(q.first) = startVector[i];
                            maxdir = max(maxdir, startVector[i]);
                        }
                    }
                }

                switch (method) {
                case StartMethods::RaySearchVectorScaled:
                    dir *= 1.0 / maxdir; break;
                case StartMethods::RaySearchVector:
                    dir.normalize(); break;
                default: break;
                }

                auto r = ray_search(w, s, numCores);
                u = r.searchu(dir);
            }
            break;
            case StartMethods::SphereSearch:
            {
                auto dir = vector1D(s.getNrStochasts());
                for (size_t i = 0; i < s.getNrAllStochasts(); i++)
                {
                    auto q = s.getStochId(i);
                    if (q.second)
                    {
                        dir(q.first) = startVector[i];
                    }
                }
                auto r = sphere_search(w, s);
                u = r.searchu(dir);
            }
            break;
            case StartMethods::One:
            {
                for (size_t i = 0; i < s.getNrStochasts(); i++)
                {
                    u(i) = 1.0;
                }
            }
            break;
            case StartMethods::GivenVector:
            {
                for (size_t i = 0; i < s.getNrAllStochasts(); i++)
                {
                    auto q = s.getStochId(i);
                    if (q.second)
                    {
                        u(q.first) = startVector[i];
                    }
                }
            }
            break;
            default:
                break;
            }
            return u;
        }

        StartMethods FillStartVector::convert(const std::string& method)
        {
            auto pls = probLibString();
            if (pls.iStrcmp(method, "ray-search-vector-scaled"))
                return StartMethods::RaySearchVectorScaled;
            else if (pls.iStrcmp(method, "ray-search-vector"))
                return StartMethods::RaySearchVector;
            else if (pls.iStrcmp(method, "ray-search"))
                return StartMethods::RaySearch;
            else if (pls.iStrcmp(method, "sphere-search"))
                return StartMethods::SphereSearch;
            else if (pls.iStrcmp(method, "one"))
                return StartMethods::One;
            else if (pls.iStrcmp(method, "zero"))
                return StartMethods::Zero;
            else if (pls.iStrcmp(method, "given-vector"))
                return StartMethods::GivenVector;
            else
                throw probLibException("Unknown start method.");
        }
    }
}
