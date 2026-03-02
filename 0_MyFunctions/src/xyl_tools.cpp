#include "../includes/xyl_tools.hpp"

char *GetAbsolutePath(char *relative_path , char * absoulte_path)
{
#ifdef _WIN32
    return _fullpath(relative_path,absoulte_path,sizeof(absoulte_path));
#else

#endif
}
