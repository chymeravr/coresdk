cmake_minimum_required (VERSION 3.4.1)

project(DaydreamAdClient)

set(REPO_PATH ../../../../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../../../../Conf/)
set(CoreEngine_PATH ../../../../CoreEngine/)
set(GLImplementation_PATH ../../../../GLImplementation)


set(Image360root ../../../../Image360)
set(DaydreamImplementationRoot ../../../../DaydreamImplementation)
set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../distribution)
set(GVR_LIBRARIES ${CMAKE_SOURCE_DIR}/../gvr_libraries)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm
                    ${Conf_PATH}/include
                    ${CoreEngine_PATH}/include
                    ${ovr_mobile_root}/VrApi/Include
                    ${Image360root}/include
                    ${GLImplementation_PATH}/include
                    ${DaydreamImplementationRoot}/include
                    ${GVR_LIBRARIES}/headers)

FILE(GLOB_RECURSE INCLUDE_HEADERS ${PROJECT_SOURCE_DIR}/src/main/jni/*.h)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/main/jni/gvrActivityHandlers.cpp
            ${PROJECT_SOURCE_DIR}/src/main/jni/DaydreamActivityCloseEvent.cpp
            ${PROJECT_SOURCE_DIR}/src/main/jni/Image360AppWrapper.cpp)

add_library(${PROJECT_NAME} SHARED ${INCLUDE_HEADERS} ${SOURCES})



find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
find_library(gles-v2 GLESv2)

set(build_type "debug")
if(CMAKE_BUILD_TYPE EQUAL "release")
    set(build_type "release")
endif()
if(CMAKE_BUILD_TYPE EQUAL "releaseDemo")
    set(build_type "release")
endif()

add_library(core_engine SHARED IMPORTED)
set_target_properties(
    core_engine
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/coreEngine/${build_type}/${ANDROID_ABI}/libCoreEngine.so
)

add_library(gl_implementation SHARED IMPORTED)
set_target_properties(
    gl_implementation
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/glImplementation/${build_type}/${ANDROID_ABI}/libGLImplementation.so
)

add_library(image360 SHARED IMPORTED)
set_target_properties(
    image360
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/image360/${build_type}/${ANDROID_ABI}/libImage360.so
)

add_library(daydream_implementation SHARED IMPORTED)
set_target_properties(
    daydream_implementation
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/daydreamimplementation/${build_type}/${ANDROID_ABI}/libDaydreamImplementation.so
)

add_library(gvr SHARED IMPORTED)
set_target_properties(
    gvr
    PROPERTIES IMPORTED_LOCATION
    ${GVR_LIBRARIES}/jni/${ANDROID_ABI}/libgvr.so
)

target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360
daydream_implementation gvr ${log-lib} ${android-lib})
