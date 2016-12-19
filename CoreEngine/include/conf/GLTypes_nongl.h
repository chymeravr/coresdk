//
// Created by chimeralabs on 11/15/2016.
//

#ifndef ANDROIDSDK_GLTYPES_H
#define ANDROIDSDK_GLTYPES_H

#include <cstdint>

namespace cl{
    typedef bool CL_GLboolean;
    typedef signed   char CL_GLbyte;
    typedef unsigned char CL_GLubyte;
    typedef short CL_GLshort;
    typedef unsigned short CL_GLushort;
    typedef int CL_GLint;
    typedef unsigned int CL_GLuint;
    typedef int32_t CL_GLfixed;
    typedef int64_t CL_GLint64;
    typedef uint64_t CL_GLuint64;
    typedef int CL_GLsizei;
    typedef unsigned int CL_GLenum;
    typedef signed   long  int CL_GLintptr;
    typedef signed   long  int CL_GLsizeiptr;
    //typedef GLsync CL_GLsync;
    typedef unsigned int CL_GLbitfield;
    //typedef GLhalf CL_GLhalf;
    typedef float CL_GLfloat;
    typedef float CL_GLclampf;
    //typedef GLdouble CL_GLdouble;
    //typedef GLclampd CL_GLclampd;
}

#endif //ANDROIDSDK_GLTYPES_H
