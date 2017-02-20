cmake_minimum_required (VERSION 3.2)

project (Renderer)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${CMAKE_SOURCE_DIR}/CoreEngine/include
					${PROJECT_SOURCE_DIR}/include)
FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")
set(SOURCES ${PROJECT_SOURCE_DIR}/src/RendererNoHMD.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} CoreEngine)
