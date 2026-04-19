#ifndef BASICINCLUDES
#define BASICINCLUDES

#include <stdio.h>
#include <string.h>
#ifdef DEBUG
#include <iostream>
#endif

#define OID_VAO 0x900
#define OID_VBO 0x901
#define OID_EBO 0x902
#define OID_TEX 0x903
#define OID_UBO 0x904
#define OID_FBO 0x905

#define Ortho 0x906
#define Persp 0x907

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