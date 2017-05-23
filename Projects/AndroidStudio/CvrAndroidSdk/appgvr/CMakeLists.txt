cmake_minimum_required (VERSION 3.4.1)

project(appgvr)

set(GVR_LIBRARIES ${CMAKE_SOURCE_DIR}/../gvr_libraries)

include_directories(${GVR_LIBRARIES}/headers
                    ${PROJECT_SOURCE_DIR}/src/main/jni)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/main/jni/treasure_hunt_jni.cc
            ${PROJECT_SOURCE_DIR}/src/main/jni/treasure_hunt_renderer.cc)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

find_library(log-lib log)
find_library(android-lib android)
find_library(egl EGL)
# todo : make gles selection dynamic
find_library(gles-v3 GLESv3)

#if(CMAKE_BUILD_TYPE MATCHES "Debug")
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

    target_link_libraries(${PROJECT_NAME} gvr gvr_audio ${log-lib} ${android-lib} ${egl} ${gles-v3})
#else()
#    target_link_libraries(${PROJECT_NAME})
#endif()