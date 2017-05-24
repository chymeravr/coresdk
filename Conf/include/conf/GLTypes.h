//
// Created by chimeralabs on 11/15/2016.
//

#ifndef ANDROIDSDK_GLTYPES_H
#define ANDROIDSDK_GLTYPES_H

#ifdef WINDOWS_GL
#include "../../src/GLTypesWindows.h"
#endif

// todo - separate out gles2 and gles3
// todo - fix incomptability with glint64 field to support older android phones

#ifdef GLES2_0
#include "../../src/GLTypes_GLES2_0.h"
#endif

#ifdef GLES3_0
#include "../../src/GLTypes_GLES3_0.h"
#endif

#ifdef NOGL
#include "../../src/GLTypesNonGL.h"
#endif

#endif //ANDROIDSDK_GLTYPES_H