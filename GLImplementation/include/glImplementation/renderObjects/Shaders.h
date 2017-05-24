//
// Created by robin on 05/23/2017.
// this file helps select right version of shaders - gl, gles2, gles3 
// on runtime 
//

#ifndef GLIMPLEMENTATION_SHADERS_H
#define GLIMPLEMENTATION_SHADERS_H

// shader sources should be mutually exclusive since they define the same variable with different 
// shader implementations as strings. not doing so will give multiple definition error at compile
// time

#ifdef WINDOWS_GL
#include <glImplementation/renderObjects/opengl/ShadersGL.h>
#endif

// todo - separate out gles2 and gles3
// todo - fix incomptability with glint64 field to support older android phones

#ifdef GLES2_0
#include <glImplementation/renderObjects/opengles2/ShadersGLES2.h>
#endif

#ifdef GLES3_0
#include <glImplementation/renderObjects/opengles3/ShadersGLES3.h>
#endif

#endif //GLIMPLEMENTATION_SHADERS_H