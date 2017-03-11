cmake_minimum_required (VERSION 3.4.1)

project (Image360)

set(REPO_PATH ../Repository)
set (GLM_VERSION 0.9.7.6)

set(Conf_INCLUDE_DIRS ../Conf/include)
set(CoreEngine_INCLUDE_DIRS ../CoreEngine/include)
set(Renderer_PATH ../Renderer)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${CoreEngine_INCLUDE_DIRS}
					${PROJECT_SOURCE_DIR}/include)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")
set(SOURCES ${PROJECT_SOURCE_DIR}/src/Image360.cpp
			${PROJECT_SOURCE_DIR}/src/Image360Stereo.cpp)

if(WINDOWS)
	link_directories(${Conf_GL_LINK_DIRECTORIES})
	include_directories(${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/include)
	
	add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS} ${SOURCES})
	target_link_libraries(${PROJECT_NAME} CoreEngine GLImplementation Renderer)
else()

	set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)

	find_library(log-lib log)
	find_library(android-lib android)
	
	add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})
	add_library(core_engine SHARED IMPORTED)
	set_target_properties(
		core_engine
		PROPERTIES IMPORTED_LOCATION
		${ANDROID_LIBRARIES}/coreEngine/${ANDROID_ABI}/libCoreEngine.so
	)
	target_link_libraries(${PROJECT_NAME} core_engine ${log-lib} ${android-lib})

endif()