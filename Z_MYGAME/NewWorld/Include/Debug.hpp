#ifndef DEBUGFILE
#define DEBUGFILE
#ifdef DEBUG
#include <iostream>
#endif

#ifdef DEBUG
#define HELLOWORLD \
    std::cout << "Hello World" << std::endl;

#define GLFWINITSTATUS                             \
    std::cout << "GLFW INIT FAILURE" << std::endl; \
    return 0;

#define GLADINITSTATUS                             \
    std::cout << "GLAD INIT FAILURE" << std::endl; \
    return 0;

#else
#define HELLOWORLD \
    do             \
    {              \
    } while (0);

#define GLFWINITSTATUS \
    do                 \
    {                  \
    } while (0);

#define GLADINITSTATUS \
    do                 \
    {                  \
    } while (0);

#endif
#endif
