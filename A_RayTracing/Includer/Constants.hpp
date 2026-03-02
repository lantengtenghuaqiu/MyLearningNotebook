#ifndef CONSTANTS_H

#define CONSTANTS_H

#include <limits>


namespace xyl
{
    namespace consts
    {
        const constexpr double pi = 3.1415926535897932385;
        const constexpr double deg_rad = pi / 180.0;

        const static double infinity = std::numeric_limits<double>::infinity();


        
    } // namespace consts
    
    class Interval
    {
    private:
        double min, max;

    public:
        Interval();
    };


}

#endif