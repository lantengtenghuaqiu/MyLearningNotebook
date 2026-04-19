#ifndef ASSIMPMANGER
#define ASSIMPMANGER

#ifdef __APPLE__
#include "./Mac/Importer.hpp"
#include "./Mac/scene.h"
#include "./Mac/postprocess.h"
#elif __WIN32__
#include "./Windows/Importer.hpp"
#include "./Windows/scene.h"
#include "./Windows/postprocess.h"
#endif

#endif