set(SUBPROJECT_NAME CoreEngine)
set(SUBPROJECT_PATH ${PROJECT_SOURCE_DIR}/${SUBPROJECT_NAME})

#include the header files
include_directories ("${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm" "${PROJECT_SOURCE_DIR}/Conf/include" "${SUBPROJECT_PATH}/include")

set(SRC_DIR ${SUBPROJECT_PATH}/src)
set(SOURCES 
	${SRC_DIR}/components/transform/TransformCamera.cpp
	${SRC_DIR}/components/transform/TransformModel.cpp
	${SRC_DIR}/conf/MathType.cpp
	${SRC_DIR}/events/EventQueue.cpp
	${SRC_DIR}/modelBuilder/CubeBuilder.cpp
	${SRC_DIR}/modelBuilder/UVSphereBuilder.cpp
	${SRC_DIR}/modifier/ImageBMPLoader.cpp
	${SRC_DIR}/modifier/ImageModifier.cpp
	${SRC_DIR}/modifier/ModelModifier.cpp
	${SRC_DIR}/renderObjects/Camera.cpp
	${SRC_DIR}/renderObjects/ComponentList.cpp
	${SRC_DIR}/renderObjects/Relation.cpp
	${SRC_DIR}/renderObjects/Scene.cpp
	${SRC_DIR}/util/SimpleOBJLoader.cpp)
add_library(${SUBPROJECT_NAME} STATIC  ${SOURCES})

#######################################################################################
# CoreEngineTests
#######################################################################################

######################################################################
# Creating GMock/GTest integration
######################################################################
if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(${PROJECT_SOURCE_DIR}/DownloadProject.cmake)
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


######################################################################
# Building CoreEngineTest
######################################################################
set(TEST_SRC_DIR ${SUBPROJECT_PATH}/test/src)
include_directories ("${SUBPROJECT_PATH}/test/include")
include (CTest)
set(TEST_SOURCES 
	${TEST_SRC_DIR}/AllTests.cpp
	${TEST_SRC_DIR}/ComponentListTest.cpp
	${TEST_SRC_DIR}/EventQueueTest.cpp
	${TEST_SRC_DIR}/MaterialTest.cpp
	${TEST_SRC_DIR}/ModelTest.cpp
	${TEST_SRC_DIR}/SceneTest.cpp
	${TEST_SRC_DIR}/TextureTest.cpp
	${TEST_SRC_DIR}/TransformModelTest.cpp
	${TEST_SRC_DIR}/CameraTest.cpp
	${TEST_SRC_DIR}/CubeBuilderTest.cpp
	${TEST_SRC_DIR}/ImageModifierTest.cpp
	${TEST_SRC_DIR}/ModelModifierTest.cpp
	${TEST_SRC_DIR}/RelationTest.cpp
	${TEST_SRC_DIR}/ShaderTest.cpp
	${TEST_SRC_DIR}/TransformCameraTest.cpp
	${TEST_SRC_DIR}/UVSphereBuilderTest.cpp
)

# Building test executable
add_executable(CoreEngineTest ${TEST_SOURCES})
target_link_libraries(CoreEngineTest gtest gmock_main CoreEngine)
enable_testing()
add_test(NAME CoreEngineTest_ COMMAND CoreEngineTest)
