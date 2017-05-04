cmake_minimum_required (VERSION 3.4.1)

project(Image360Ovrm)

set(REPO_PATH ../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../Conf/)
set(CoreEngine_PATH ../CoreEngine/)
set(GLImplementation_PATH ../GLImplementation)
set(ovr_mobile_root ../Projects/AndroidStudio/OvrMSdk/ovr_sdk_mobile)
set(image360root ../Image360)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
                    ${Conf_PATH}/include
					${CoreEngine_PATH}/include
                    ${ovr_mobile_root}/VrApi/Include
                    ${image360root}/include
					${GLImplementation_PATH}/include
					${PROJECT_SOURCE_DIR}/include)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

#FILE(GLOB_RECURSE SOURCES "src/**/*.cpp")

set(SOURCES #${PROJECT_SOURCE_DIR}/src/ImageBMPLoaderAndroid.cpp
            ${PROJECT_SOURCE_DIR}/src/LoggerOVRM.cpp
            #${PROJECT_SOURCE_DIR}/src/OVRMUtils.cpp
            ${PROJECT_SOURCE_DIR}/src/RendererOVRM.cpp
            ${PROJECT_SOURCE_DIR}/src/CloseButtonListenerOVRM.cpp
            ${PROJECT_SOURCE_DIR}/src/ActionButtonListenerOVRM.cpp
            ${PROJECT_SOURCE_DIR}/src/RendererOVRMStereo.cpp)

add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})

set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/distribution)
set(OVR_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/OvrMSdk/ovr_sdk_mobile)

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

add_library(vrapi SHARED IMPORTED)
set_target_properties(
    vrapi
    PROPERTIES IMPORTED_LOCATION
    ${OVR_LIBRARIES}/VrApi/Libs/Android/${ANDROID_ABI}/Release/libvrapi.so
)

target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360 
 ${log-lib} ${android-lib} ${egl} ${gles-v3} vrapi)

