cmake_minimum_required(VERSION 3.4.1)
project(CoreEngine)

#include the header files

set(REPO_PATH ../Repository)
set(Conf_INCLUDE_DIRS ../Conf/include)
set(SOIL_INCLUDE_DIRS ../SOIL/include)
set (GLM_VERSION 0.9.7.6)

include_directories (${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					 ${Conf_INCLUDE_DIRS} 
					 ${SOIL_INCLUDE_DIRS}
					 ${PROJECT_SOURCE_DIR}/include)


message("soil directories = '${SOIL_INCLUDE_DIRS}'")
FILE(GLOB_RECURSE HEADERS "include/**/*.h")
FILE(GLOB_RECURSE SOURCES "src/**/*.cpp")

FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")

# Android cannot download files using cmake - only do this on WINDOWS

if(WINDOWS)
	#######################################################
	# Creating FreeType integration
	#######################################################
	if (CMAKE_VERSION VERSION_LESS 3.2)
		set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
	else()
		set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
	endif()

	if(WINDOWS)
		include(${CMAKE_SOURCE_DIR}/DownloadProject.cmake)
	else()
		include(../DownloadProject.cmake)
	endif()

	download_project(PROJ                freetype
					URL      http://download.savannah.gnu.org/releases/freetype/freetype-2.7.tar.bz2
					${UPDATE_DISCONNECTED_IF_AVAILABLE}
	)

	add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
	include_directories("${freetype_SOURCE_DIR}/include")
	########################################################

	add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS}  ${SOURCES})
	target_link_libraries(${PROJECT_NAME} freetype SOIL)
	#target_link_libraries(${PROJECT_NAME} SOIL)
else()
	set(FREETYPE_DIRS ../freetype)
	include_directories("${FREETYPE_DIRS}/include")
	add_library(${PROJECT_NAME} SHARED ${INCLUDE_HEADERS} ${SOURCES})
	set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)
	
	add_library(freetype_lib SHARED IMPORTED)
	set_target_properties(
		freetype_lib
		PROPERTIES IMPORTED_LOCATION
		${ANDROID_LIBRARIES}/freetype/${ANDROID_ABI}/libfreetype.so
	)

	add_library(soil_lib SHARED IMPORTED)
	set_target_properties(
		soil_lib
		PROPERTIES IMPORTED_LOCATION
		${ANDROID_LIBRARIES}/soil/${ANDROID_ABI}/libSOIL.so
	)
	target_link_libraries(${PROJECT_NAME} freetype_lib soil_lib)
endif()

if(WINDOWS)
	#######################################################################################
	# CoreEngineTests
	#######################################################################################

	#######################################################
	# Creating GMock/GTest integration
	#######################################################
	if (CMAKE_VERSION VERSION_LESS 3.2)
		set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
	else()
		set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
	endif()

	include(${CMAKE_SOURCE_DIR}/DownloadProject.cmake)
	download_project(PROJ                googletest
					GIT_REPOSITORY      https://github.com/google/googletest.git
					GIT_TAG             master
					${UPDATE_DISCONNECTED_IF_AVAILABLE}
	)

	# Prevent GoogleTest from overriding our compiler/linker options
	# when building with Visual Studio
	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})

	# When using CMake 2.8.11 or later, header path dependencies
	# are automatically added to the gtest and gmock targets.
	# For earlier CMake versions, we have to explicitly add the
	# required directories to the header search path ourselves.
	if (CMAKE_VERSION VERSION_LESS 2.8.11)
		include_directories("${gtest_SOURCE_DIR}/include"
							"${gmock_SOURCE_DIR}/include")
	endif()


	#####################################################
	# Building CoreEngineTest
	#####################################################
	set(TEST_SRC_DIR ${PROJECT_SOURCE_DIR}/test/src)
	include_directories ("${PROJECT_SOURCE_DIR}/test/include")
	FILE(GLOB_RECURSE INCLUDE_TEST_HEADERS "test/include/*.h")
	include (CTest)
	set(TEST_SOURCES	${PROJECT_SOURCE_DIR}/test/src/AllTests.cpp
						${PROJECT_SOURCE_DIR}/test/src/CameraTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/ComponentListTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/CubeBuilderTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/EventQueueTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/GazeDetectorTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/GazeDetectorBoxTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/ImageModifierTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/MaterialTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/ModelModifierTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/ModelTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/RelationTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/SceneTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/ShaderTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TextureTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TransformCameraTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TransformModelTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TransformTreeTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TransformTreeCameraTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/TransformTreeModelTest.cpp
						${PROJECT_SOURCE_DIR}/test/src/UVSphereBuilderTest.cpp)

	# Building test executable
	add_executable(CoreEngineTest ${INCLUDE_TEST_HEADERS} ${TEST_SOURCES})
	target_link_libraries(CoreEngineTest gtest gmock_main CoreEngine)
	enable_testing()
	add_test(NAME CoreEngineTest_ COMMAND CoreEngineTest)
endif()