cmake_minimum_required (VERSION 3.2)
project (GraphicsSDK)

# The version number.
set (GraphicsSDK_VERSION_MAJOR 1)
set (GraphicsSDK_VERSION_MINOR 0)
set (REPO_PATH ${PROJECT_SOURCE_DIR}/Repository)
set (GLM_VERSION 0.9.7.6)
set (CMAKE_CXX_STANDARD 11)

# configure a header file to pass some of the CMake settings
# to the source code

add_definitions(-DNOGL)

#configure_file (
#  "${PROJECT_SOURCE_DIR}/GraphicsSDKConfig.h.in"
#  "${PROJECT_BINARY_DIR}/GraphicsSDKConfig.h"
#  )

add_subdirectory(CoreEngine)
