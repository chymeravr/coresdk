cmake_minimum_required(VERSION 3.4.1)
project(SOIL)

#include the header files

include_directories (${PROJECT_SOURCE_DIR}/include)


set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/image_DXT.c
			 ${PROJECT_SOURCE_DIR}/src/image_helper.c
			 ${PROJECT_SOURCE_DIR}/src/SOIL.c
			 ${PROJECT_SOURCE_DIR}/src/stb_image_aug.c
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)

# android only builds shared libraries - for now. no idea how ovr did it
add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})

if(WINDOWS)
	target_link_libraries(${PROJECT_NAME} Image360WindowsLauncher)
endif()