#ifndef INFORS_H
#define INFORS_H

#include <CL/cl.hpp>
#include <vector>
namespace xyl
{
    namespace cl
    {
        class Infos
        {
            std::vector<cl_uint> platformCouts;
            cl_platform_id platformIDs;

            inline void GetPlatformIDs()
            {

            }

        };
    } // namespace cl

}//xyl


#endif