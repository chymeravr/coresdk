cmake_minimum_required (VERSION 3.2)

project (Image360)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${CMAKE_SOURCE_DIR}/CoreEngine/include
					${CMAKE_SOURCE_DIR}/GLImplementation/include
					${CMAKE_SOURCE_DIR}/Renderer/include
					${PROJECT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")
set(SOURCES ${PROJECT_SOURCE_DIR}/src/Image360.cpp
)

link_directories(${Conf_GL_LINK_DIRECTORIES})

add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} CoreEngine GLImplementation Renderer)
