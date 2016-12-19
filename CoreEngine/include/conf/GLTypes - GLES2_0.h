//
// Created by chimeralabs on 8/18/2016.
//

#ifndef ANDROIDSDK_GLTYPES_H
#define ANDROIDSDK_GLTYPES_H

#include <GLES2/gl2.h>

namespace cl{
    typedef GLboolean CL_GLboolean;
    typedef GLbyte CL_GLbyte;
    typedef GLubyte CL_GLubyte;
    typedef GLshort CL_GLshort;
    typedef GLushort CL_GLushort;
    typedef GLint CL_GLint;
    typedef GLuint CL_GLuint;
    typedef GLfixed CL_GLfixed;
    typedef GLint64 CL_GLint64;
    typedef GLuint64 CL_GLuint64;
    typedef GLsizei CL_GLsizei;
    typedef GLenum CL_GLenum;
    typedef GLintptr CL_GLintptr;
    typedef GLsizeiptr CL_GLsizeiptr;
    typedef GLsync CL_GLsync;
    typedef GLbitfield CL_GLbitfield;
    //typedef GLhalf CL_GLhalf;
    typedef GLfloat CL_GLfloat;
    typedef GLclampf CL_GLclampf;
    //typedef GLdouble CL_GLdouble;
    //typedef GLclampd CL_GLclampd;

#define CL_GL_VERTEX_SHADER GL_VERTEX_SHADER
#define CL_GL_FRAGMENT_SHADER GL_FRAGMENT_SHADER
#define CL_GL_FALSE GL_FALSE

}
#endif //ANDROIDSDK_GLTYPES_H