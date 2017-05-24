//
// Created by robin on 05/23/2017.
// this file helps select right version of shaders - gl, gles2, gles3 
// on runtime 
//

#ifndef GLIMPLEMENTATION_UISHADERS_H
#define GLIMPLEMENTATION_UISHADERS_H

// shader sources should be mutually exclusive since they define the same variable with different 
// shader implementations as strings. not doing so will give multiple definition error at compile
// time

#ifdef WINDOWS_GL
#include <glImplementation/ui/opengl/UiShadersGL.h>
#endif

// todo - separate out gles2 and gles3
// todo - fix incomptability with glint64 field to support older android phones

#ifdef GLES2_0
#include <glImplementation/ui/opengles2/UiShadersGLES2.h>
#endif

#ifdef GLES3_0
#include <glImplementation/ui/opengles3/UiShadersGLES3.h>
#endif

#endif //GLIMPLEMENTATION_UISHADERS_H