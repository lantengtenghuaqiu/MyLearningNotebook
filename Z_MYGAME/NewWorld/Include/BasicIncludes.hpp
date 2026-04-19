#ifndef BASICINCLUDES
#define BASICINCLUDES

#include <stdio.h>


#ifdef __APPLE__
#ifndef BASEPATH
#define BASEPATH "../"
#endif
#else
#define BASEPATH "G:/user/desktop/C++/GraphicLearning"
#endif


#define OID_VAO 0x900
#define OID_VBO 0x901
#define OID_EBO 0x902
#define OID_TEX 0x903
#define OID_UBO 0x904
#define OID_FBO 0x905

#define Ortho 0x906
#define Persp 0x907

// Components:
enum COMPONENT
{
    Transform = 1 << 0,
    Image = 1 << 1,
    MeshRender = 1 << 2,
};

// MeshRender
enum FETCH
{
    FAILD = 0 << 0,
    EBO = 1 << 0,
    MESH = 1 << 1,
    UV = 1 << 2,
    NORMAL = 1 << 3
};

#define FLOAT_SIZE 4
#define VEC3_SIZE 12
#define VEC4_SIZE 16
#define MAT4_SIZE 64

#define FAIL 0
#define SUCCESS 1

#endif