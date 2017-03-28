cmake_minimum_required (VERSION 3.4.1)

project(image360ad)

set(REPO_PATH ../../../../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../../../../Conf/)
set(CoreEngine_PATH ../../../../CoreEngine/)
set(GLImplementation_PATH ../../../../GLImplementation)
set(Renderer_PATH ../../../../Renderer)

set(image360root ../../../../Image360)
set(image360GvrAndroidRoot ../../../../GVRAndroidImplementation)
set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../distribution)
set(GVR_LIBRARIES ${CMAKE_SOURCE_DIR}/../gvr_libraries)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm
                    ${Conf_PATH}/include
                    ${CoreEngine_PATH}/include
                    ${ovr_mobile_root}/VrApi/Include
                    ${image360root}/include
                    ${GLImplementation_PATH}/include
                    ${Renderer_PATH}/include
                    ${image360GvrAndroidRoot}/include
                    ${GVR_LIBRARIES}/headers)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/main/jni/gvrActivityHandlers.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})



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

add_library(renderer SHARED IMPORTED)
set_target_properties(
    renderer
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/renderer/${ANDROID_ABI}/libRenderer.so
)

add_library(image360 SHARED IMPORTED)
set_target_properties(
    image360
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/image360/${ANDROID_ABI}/libImage360.so
)

add_library(image360_gvr SHARED IMPORTED)
set_target_properties(
    image360_gvr
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/image360Gvr/${ANDROID_ABI}/libImage360Gvr.so
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_library(gvr SHARED IMPORTED)
            set_target_properties(
                gvr
                PROPERTIES IMPORTED_LOCATION
                ${GVR_LIBRARIES}/jni/${ANDROID_ABI}/libgvr.so
        )
    add_library(gvr_audio SHARED IMPORTED)
            set_target_properties(
                gvr_audio
                PROPERTIES IMPORTED_LOCATION
                ${GVR_LIBRARIES}/jni/${ANDROID_ABI}/libgvr_audio.so
        )
    target_link_libraries(${PROJECT_NAME} core_engine gl_implementation renderer image360
    image360_gvr gvr gvr_audio ${log-lib} ${android-lib} ${egl} ${gles-v3})
else()
    target_link_libraries(${PROJECT_NAME} core_engine gl_implementation renderer image360
    image360_gvr ${log-lib} ${android-lib} ${egl} ${gles-v3})
endif()