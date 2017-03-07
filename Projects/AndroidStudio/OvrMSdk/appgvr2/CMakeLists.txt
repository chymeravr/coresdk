cmake_minimum_required (VERSION 3.4.1)

project(appgvr2)

set(ANDROID_LIBRARIES ${CMAKE_SOURCE_DIR}/../distribution)

include_directories(${ANDROID_LIBRARIES}/ovrApi/Include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/main/jni/AppGvr2.c)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
find_library(gles-v3 GLESv3)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
    add_library(vrapi SHARED IMPORTED)
        set_target_properties(
            vrapi
            PROPERTIES IMPORTED_LOCATION
            ${ANDROID_LIBRARIES}/ovrApi/${ANDROID_ABI}/libvrapi.so
    )

    target_link_libraries(${PROJECT_NAME} vrapi ${log-lib} ${android-lib} ${egl} ${gles-v3})
#else()
#    target_link_libraries(${PROJECT_NAME})
#endif()