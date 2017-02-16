cmake_minimum_required (VERSION 3.4.1)

project (GLImplementation)

set(REPO_PATH ../Repository)
set(Conf_INCLUDE_DIRS ../Conf/include)
set(CoreEngine_INCLUDE_DIRS ../CoreEngine/include)
set (GLM_VERSION 0.9.7.6)

include_directories(${Conf_INCLUDE_DIRS}
					${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
					${CoreEngine_INCLUDE_DIRS}
					${PROJECT_SOURCE_DIR}/include)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

FILE(GLOB_RECURSE HEADERS "include/**/*.h")
FILE(GLOB_RECURSE SOURCES "src/**/*.cpp")


FUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST _InFileList _excludeDirName _verbose)
  foreach (ITR ${_InFileList})
    if ("${_verbose}")
      message(STATUS "ITR=${ITR}")
    endif ("${_verbose}")

    if ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")                   # Check if the item matches the directory name in _excludeDirName
      message(STATUS "Remove Item from List:${ITR}")
      list (REMOVE_ITEM _InFileList ${ITR})                              # Remove the item from the list
    endif ("${ITR}" MATCHES "(.*)${_excludeDirName}(.*)")

  endforeach(ITR)
  set(SOURCE_FILES ${_InFileList} PARENT_SCOPE)                          # Return the SOURCE_FILES variable to the calling parent
ENDFUNCTION (EXCLUDE_FILES_FROM_DIR_IN_LIST)

if(WINDOWS_GL)
	set (EXCLUDE_HEADERS "/opengles2/"
						 "/opengles3/")
	set (EXCLUDE_SOURCES "/opengles2/"
						 "/opengles3/")
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${HEADERS}" "/opengles2/" FALSE)
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCES}" "/opengles2/" FALSE)
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${HEADERS}" "/opengles3/" FALSE)
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCES}" "/opengles3/" FALSE)
elseif(GLES2_0)
	set (EXCLUDE_HEADERS "/opengl/")
	set (EXCLUDE_SOURCES "/opengl/")
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${HEADERS}" "/opengl/" FALSE)
	EXCLUDE_FILES_FROM_DIR_IN_LIST("${SOURCES}" "/opengl/" FALSE)
endif()


add_library(${PROJECT_NAME} SHARED ${HEADERS} ${SOURCES})
if(WINDOWS)
	target_link_libraries(${PROJECT_NAME} CoreEngine)
else()
	set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)

	find_library(log-lib log)
	find_library(android-lib android)
	find_library(egl EGL)
	find_library(gles-v3 GLESv3)

	add_library(core_engine SHARED IMPORTED)
	set_target_properties(
		core_engine
		PROPERTIES IMPORTED_LOCATION
		${ANDROID_LIBRARIES}/coreEngine/${ANDROID_ABI}/libCoreEngine.so
	)
	target_link_libraries(${PROJECT_NAME} core_engine ${log-lib} ${android-lib} ${egl} ${gles-v3})
endif()
