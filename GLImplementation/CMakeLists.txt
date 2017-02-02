cmake_minimum_required (VERSION 3.2)

project (GLImplementation)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${CMAKE_SOURCE_DIR}/CoreEngine/include
					${PROJECT_SOURCE_DIR}/include)
FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")
set(SOURCES ${PROJECT_SOURCE_DIR}/src/renderObjects/CameraGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/CameraGLContainer.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/MaterialDiffuseTextureCubeMapGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/MaterialDiffuseTextureGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/MaterialUniformColorGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/ModelGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/SceneGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/ShaderDiffuseTextureCubeMapGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/ShaderDiffuseTextureGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/ShaderUniformColorGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/TextureCubeMapGL.cpp
			${PROJECT_SOURCE_DIR}/src/renderObjects/TextureGL.cpp
			${PROJECT_SOURCE_DIR}/src/util/ShaderProgramUtil.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} CoreEngine)
