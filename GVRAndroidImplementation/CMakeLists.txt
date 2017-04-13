cmake_minimum_required (VERSION 3.4.1)

project(Image360Gvr)

set(REPO_PATH ../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../Conf/)
set(CoreEngine_PATH ../CoreEngine/)
set(GLImplementation_PATH ../GLImplementation)
set(image360root ../Image360)
set(gvr_libraries ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/gvr_libraries)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
                    ${Conf_PATH}/include
					${CoreEngine_PATH}/include
                    ${image360root}/include
					${GLImplementation_PATH}/include
					${PROJECT_SOURCE_DIR}/include
                    ${gvr_libraries}/headers)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/LoggerGVR.cpp
            ${PROJECT_SOURCE_DIR}/src/RendererGVR.cpp
            ${PROJECT_SOURCE_DIR}/src/CloseMeListenerGVR.cpp
            ${PROJECT_SOURCE_DIR}/src/NotifyMeListenerGVR.cpp)

add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})

set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)

find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
find_library(gles-v2 GLESv2)

add_library(core_engine SHARED IMPORTED)
set_target_properties(
    core_engine
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/coreEngine/${ANDROID_ABI}/libCoreEngine.so
)

add_library(gl_implementation SHARED IMPORTED)
set_target_properties(
    gl_implementation
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/glImplementation/${ANDROID_ABI}/libGLImplementation.so
)

add_library(image360 SHARED IMPORTED)
set_target_properties(
    image360
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/image360/${ANDROID_ABI}/libImage360.so
)

add_library(gvr SHARED IMPORTED)
set_target_properties(
    gvr
    PROPERTIES IMPORTED_LOCATION
    ${gvr_libraries}/jni/${ANDROID_ABI}/libgvr.so
)
add_library(gvr_audio SHARED IMPORTED)
set_target_properties(
    gvr_audio
    PROPERTIES IMPORTED_LOCATION
    ${gvr_libraries}/jni/${ANDROID_ABI}/libgvr_audio.so
)

target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360
 ${log-lib} ${android-lib} ${egl} ${gles-v2}  gvr gvr_audio)

