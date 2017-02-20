cmake_minimum_required (VERSION 3.2)

project (WindowsImplementation)

include_directories(${CMAKE_SOURCE_DIR}/CoreEngine/include
					${PROJECT_SOURCE_DIR}/include)
FILE(GLOB_RECURSE INCLUDE_HEADERS "include/*.h")
set(SOURCES ${PROJECT_SOURCE_DIR}/src/LoggerWindows.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDE_HEADERS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} CoreEngine)
