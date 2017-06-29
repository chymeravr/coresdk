cmake_minimum_required (VERSION 3.2)

project (Image360WindowsLauncher)

set(ASSIMP_DIRS ../assimp)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${REPO_PATH}/thirdParty/freeglut/include
					${CMAKE_SOURCE_DIR}/CoreEngine/include
					${CMAKE_SOURCE_DIR}/GLImplementation/include
					${CMAKE_SOURCE_DIR}/Renderer/include
					${CMAKE_SOURCE_DIR}/Image360/include
					${CMAKE_SOURCE_DIR}/WindowsImplementation/include
					${CMAKE_SOURCE_DIR}/glfw/include
					${PROJECT_SOURCE_DIR}/include
					${ASSIMP_DIRS}/include)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/Image360WindowsLauncher.cpp
			${PROJECT_SOURCE_DIR}/src/stdafx.cpp
)

set (ASSIMP_LINK_DIRS ${ASSIMP_DIRS}/lib/Debug)


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

link_directories(${GLEW_LINK_DIRS}
				 ${GLUT_LINK_DIRS}
				 ${ASSIMP_LINK_DIRS})

add_executable(${PROJECT_NAME} ${INCLUDE_DIRS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} assimp-vc120-mt freeglut glew32 CoreEngine GLImplementation Renderer 
						Image360 WindowsImplementation glfw)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${GLEW_SHARED_LINK_DIRS}/glew32.dll"      # <--this is in-file
        "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/glew32.dll")                 # <--this is out-file path
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD        # Adds a post-build event to MyTest
    COMMAND ${CMAKE_COMMAND} -E copy_if_different  # which executes "cmake - E copy_if_different..."
        "${GLUT_SHARED_LINK_DIRS}/freeglut.dll"      # <--this is in-file
        "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)/freeglut.dll")                 # <--this is out-file path