cmake_minimum_required (VERSION 3.2)

project (WindowsImplementation)

include_directories(${CMAKE_SOURCE_DIR}/CoreEngine/include
					${PROJECT_SOURCE_DIR}/include)

set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/LoggerWindows.cpp
)

add_library(${PROJECT_NAME} STATIC ${INCLUDE_DIRS} ${SOURCES})
target_link_libraries(${PROJECT_NAME} CoreEngine)
