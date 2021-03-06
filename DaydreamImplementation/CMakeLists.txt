cmake_minimum_required (VERSION 3.4.1)

project(DaydreamImplementation)

set(REPO_PATH ../Repository)
set (GLM_VERSION 0.9.7.6)
set(Conf_PATH ../Conf/)
set(CoreEngine_PATH ../CoreEngine/)
set(GLImplementation_PATH ../GLImplementation)
set(image360root ../Image360)
set(gvr_libraries ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/CvrAndroidSdk/gvr_libraries)

include_directories(${REPO_PATH}/thirdParty/glm/glm-${GLM_VERSION}/glm 
                    ${Conf_PATH}/include
					${CoreEngine_PATH}/include
                    ${image360root}/include
					${GLImplementation_PATH}/include
					${PROJECT_SOURCE_DIR}/include
                    ${gvr_libraries}/headers)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/LoggerDaydream.cpp
            ${PROJECT_SOURCE_DIR}/src/RendererDaydream.cpp
            ${PROJECT_SOURCE_DIR}/src/ControllerDaydream.cpp
            ${PROJECT_SOURCE_DIR}/src/CloseButtonListenerDaydream.cpp
            ${PROJECT_SOURCE_DIR}/src/ActionButtonListenerDaydream.cpp)

add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})

set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../Projects/AndroidStudio/CvrAndroidSdk/distribution)

find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
find_library(gles-v2 GLESv2)

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

add_library(gvr SHARED IMPORTED)
set_target_properties(
    gvr
    PROPERTIES IMPORTED_LOCATION
    ${gvr_libraries}/jni/${ANDROID_ABI}/libgvr.so
)

target_link_libraries(${PROJECT_NAME} core_engine gl_implementation image360
 ${log-lib} ${android-lib} ${egl} ${gles-v2} gvr)

