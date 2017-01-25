cmake_minimum_required (VERSION 3.2)
project (GraphicsSDK)

# The version number.
set (GraphicsSDK_VERSION_MAJOR 1)
set (GraphicsSDK_VERSION_MINOR 0)

# Target Architecture Win32, x64 #
option (WINDOWS "Whether on Windows operating system" ON)
option (Win32 "Use Win32 Libraries" ON)
option (x64 "Use x64 Libraries" OFF)

##GL options
option (WINDOWS_GL "Use GL in Windows" ON)
option (NOGL "Don't use GL" OFF)

set (REPO_PATH ${PROJECT_SOURCE_DIR}/Repository)
set (GLM_VERSION 0.9.7.6)
set (GLEW_VERSION 2.0.0)
set (CMAKE_CXX_STANDARD 11)

# configure a header file to pass some of the CMake settings
# to the source code

if(WINDOWS_GL)
	add_definitions(-DWINDOWS_GL)
elseif(NOGL)
	add_definitions(-DNOGL)
endif()


#configure_file (
#  "${PROJECT_SOURCE_DIR}/GraphicsSDKConfig.h.in"
#  "${PROJECT_BINARY_DIR}/GraphicsSDKConfig.h"
#  )
add_subdirectory(Conf)
add_subdirectory(CoreEngine)
add_subdirectory(GLImplementation)
add_subdirectory(Renderer)
add_subdirectory(Image360)
if(WINDOWS AND WINDOWS_GL)
	add_subdirectory(WindowsImplementation)
	add_subdirectory(Image360WindowsLauncher)
endif()