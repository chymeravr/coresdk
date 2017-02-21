cmake_minimum_required (VERSION 3.2)

project (FreeTypeTest)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${REPO_PATH}/thirdParty/freeglut/include
					${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/FreeTypeTest.cpp
			${PROJECT_SOURCE_DIR}/src/TextRenderer.cpp
			${PROJECT_SOURCE_DIR}/src/stdafx.cpp
)


if(x64)
	set (GLEW_LINK_DIRS ${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/lib/Release/x64)
	set (GLUT_LINK_DIRS ${REPO_PATH}/thirdParty/freeglut/lib/X64)
	set (GLEW_SHARED_LINK_DIRS ${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/bin/Release/x64)
	set (GLUT_SHARED_LINK_DIRS ${REPO_PATH}/thirdParty/freeglut/bin/X64)
elseif(Win32)
	set (GLEW_LINK_DIRS ${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/lib/Release/Win32)
	set (GLUT_LINK_DIRS ${REPO_PATH}/thirdParty/freeglut/lib)
	set (GLEW_SHARED_LINK_DIRS ${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/bin/Release/Win32)
	set (GLUT_SHARED_LINK_DIRS ${REPO_PATH}/thirdParty/freeglut/bin)
endif()

#######################################################
# Creating FreeType integration
#######################################################
if (CMAKE_VERSION VERSION_LESS 3.2)
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "")
else()
    set(UPDATE_DISCONNECTED_IF_AVAILABLE "UPDATE_DISCONNECTED 1")
endif()

include(${CMAKE_SOURCE_DIR}/DownloadProject.cmake)
download_project(PROJ                freetype
                 URL      http://download.savannah.gnu.org/releases/freetype/freetype-2.7.tar.bz2
                 ${UPDATE_DISCONNECTED_IF_AVAILABLE}
)

add_subdirectory(${freetype_SOURCE_DIR} ${freetype_BINARY_DIR})
include_directories("${freetype_SOURCE_DIR}/include")
########################################################

link_directories(${GLEW_LINK_DIRS}
				 ${GLUT_LINK_DIRS})

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} freetype freeglut glew32)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${GLEW_SHARED_LINK_DIRS}/glew32.dll"      # <--this is in-file
        "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/glew32.dll")                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${GLUT_SHARED_LINK_DIRS}/freeglut.dll"      # <--this is in-file
        "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/freeglut.dll")                 # <--this is out-file path