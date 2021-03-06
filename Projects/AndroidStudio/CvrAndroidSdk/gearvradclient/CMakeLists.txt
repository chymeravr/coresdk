cmake_minimum_required (VERSION 3.4.1)

project(GearVrAdClient)

set(REPO_PATH ../../../../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../../../../Conf/)
set(CoreEngine_PATH ../../../../CoreEngine/)
set(GLImplementation_PATH ../../../../GLImplementation)
set(ovr_mobile_root ../ovr_sdk_mobile)
set(image360root ../../../../Image360)
set(gearVrImplementationRoot ../../../../GearVrImplementation)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm
                    ${Conf_PATH}/include
                    ${CoreEngine_PATH}/include
                    ${ovr_mobile_root}/VrApi/Include
                    ${image360root}/include
                    ${GLImplementation_PATH}/include
                    ${gearVrImplementationRoot}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/main/jni/gearVrActivityHandlers.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../distribution)
set(OVR_LIBRARIES ${CMAKE_SOURCE_DIR}/../ovr_sdk_mobile)

find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
find_library(gles-v3 GLESv3)

add_library(core_engine SHARED IMPORTED)
set_target_properties(
    core_engine
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/coreEngine/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libCoreEngine.so
)

add_library(gl_implementation SHARED IMPORTED)
set_target_properties(
    gl_implementation
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/glImplementation/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libGLImplementation.so
)

add_library(image360 SHARED IMPORTED)
set_target_properties(
    image360
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/image360/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libImage360.so
)

add_library(gearvr_implementation SHARED IMPORTED)
set_target_properties(
    gearvr_implementation
    PROPERTIES IMPORTED_LOCATION
    ${ANDROID_LIBRARIES}/gearvrimplementation/${CMAKE_BUILD_TYPE}/${ANDROID_ABI}/libGearVrImplementation.so
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_library(vrapi SHARED IMPORTED)
    set_target_properties(
        vrapi
        PROPERTIES IMPORTED_LOCATION
        ${ANDROID_LIBRARIES}/ovrApi/${ANDROID_ABI}/libvrapi.so
    )
    target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360
    gearvr_implementation ${log-lib} ${android-lib} ${egl} ${gles-v3} vrapi)
else()
    target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360
    gearvr_implementation ${log-lib} ${android-lib} ${egl} ${gles-v3})
endif()