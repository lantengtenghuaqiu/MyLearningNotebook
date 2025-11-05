#ifndef CONSTANTS_H

#define CONSTANTS_H
#ifdef RTONEWEEK
    #include <limits>
#endif

namespace xyl
{
    namespace consts
    {
        const constexpr double pi = 3.1415926535897932385;
        const constexpr double deg_rad = pi / 180.0;
        #ifdef RTONEWEEK
            const static double infinity = std::numeric_limits<double>::infinity();

        #endif
        
    } // namespace consts
    



}

#endif