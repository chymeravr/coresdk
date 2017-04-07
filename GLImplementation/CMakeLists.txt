cmake_minimum_required (VERSION 3.4.1)

project (GLImplementation)
message("Building GLImplementation")

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

function(exclude_subdirectory _source _exclude_dir _dest_list)
	#message( "Exclude Dir = ${_exclude_dir}")
	SET(_dest_list "" PARENT_SCOPE)
	foreach (TMP_PATH ${_source})
		#message("TMP PATH = ${TMP_PATH}")
		if ("${TMP_PATH}" MATCHES "(.*)\/${_exclude_dir}\/(.*)")
		else()
      message(STATUS "Adding Item from List:${TMP_PATH}")
      list (APPEND _dest_list ${TMP_PATH})
    endif ()
	endforeach(TMP_PATH)
	set(${_dest_list} "" PARENT_SCOPE)
endfunction(exclude_subdirectory)

if (WINDOWS)
	# remove opengl es files and keep only core files
	set (EXCLUDE_DIR1 "opengles2")
	set (EXCLUDE_DIR2 "opengles3")
	set (NEW_SOURCES "")
	exclude_subdirectory( "${SOURCES}" "${EXCLUDE_DIR1}" NEW_SOURCES)
	set(SOURCES "")
	exclude_subdirectory( "${NEW_SOURCES}" "${EXCLUDE_DIR2}" SOURCES)

	include_directories(${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/include)

	add_library(${PROJECT_NAME} STATIC ${HEADERS} ${SOURCES})
	target_link_libraries(${PROJECT_NAME} CoreEngine)
else ()
	# remove opengl core files and keep only es files
	set (EXCLUDE_DIR "opengl")

	set (NEW_SOURCES "")
	set (NEW_HEADERS "")
	exclude_subdirectory( "${SOURCES}" "${EXCLUDE_DIR}" NEW_SOURCES)
	exclude_subdirectory( "${HEADERS}" "${EXCLUDE_DIR}" NEW_HEADERS)
	
	set(GLES_SOURCES "")
	set(GLES_HEADERS "")

	message(STATUS "${GLES_V2}")
	if(GLES_V2)
		message(STATUS "excluding gles3 files")
		set(EXCLUDE_GLES3_DIR "opengles3")
		exclude_subdirectory( "${NEW_SOURCES}" "${EXCLUDE_GLES3_DIR}" GLES_SOURCES)
		exclude_subdirectory( "${NEW_HEADERS}" "${EXCLUDE_GLES3_DIR}" GLES_HEADERS)
		#add_library(${PROJECT_NAME} SHARED ${GLES_HEADERS} ${GLES_SOURCES})
	else()
		message(STATUS "excluding gles2 files")
		set(EXCLUDE_GLES2_DIR "opengles2")
		exclude_subdirectory( "${NEW_SOURCES}" "${EXCLUDE_GLES2_DIR}" GLES_SOURCES)
		exclude_subdirectory( "${NEW_HEADERS}" "${EXCLUDE_GLES2_DIR}" GLES_HEADERS)
		#add_library(${PROJECT_NAME} SHARED ${GLES_HEADERS} ${GLES_SOURCES})
	endif()

	add_library(${PROJECT_NAME} SHARED ${GLES_HEADERS} ${GLES_SOURCES})
	set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)

	find_library(log-lib log)
	find_library(android-lib android)
	find_library(egl EGL)
	
	add_library(core_engine SHARED IMPORTED)
	set_target_properties(
		core_engine
		PROPERTIES IMPORTED_LOCATION
		${ANDROID_LIBRARIES}/coreEngine/${ANDROID_ABI}/libCoreEngine.so
	)
	if(GLES_V2)
		message(STATUS "adding OpenGLES2 libraries")
		find_library(gles GLESv2)
	else()
		message(STATUS "adding OpenGLES3 libraries")
		find_library(gles GLESv3)
	endif()
	target_link_libraries(${PROJECT_NAME} core_engine ${log-lib} ${android-lib} ${egl} ${gles})
endif ()