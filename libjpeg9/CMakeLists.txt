cmake_minimum_required(VERSION 3.4.1)
project(jpeg)

#include the header files

include_directories (${PROJECT_SOURCE_DIR}/include)


set (INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include)

set(SOURCES  ${PROJECT_SOURCE_DIR}/src/jcapimin.c 
			 ${PROJECT_SOURCE_DIR}/src/jcapistd.c 
			 ${PROJECT_SOURCE_DIR}/src/jccoefct.c 
			 ${PROJECT_SOURCE_DIR}/src/jccolor.c 
			 ${PROJECT_SOURCE_DIR}/src/jcdctmgr.c 
			 ${PROJECT_SOURCE_DIR}/src/jchuff.c
			 ${PROJECT_SOURCE_DIR}/src/jcinit.c 
			 ${PROJECT_SOURCE_DIR}/src/jcmainct.c 
			 ${PROJECT_SOURCE_DIR}/src/jcmarker.c 
			 ${PROJECT_SOURCE_DIR}/src/jcmaster.c 
			 ${PROJECT_SOURCE_DIR}/src/jcomapi.c 
			 ${PROJECT_SOURCE_DIR}/src/jcparam.c
			 ${PROJECT_SOURCE_DIR}/src/jcprepct.c 
			 ${PROJECT_SOURCE_DIR}/src/jcsample.c 
			 ${PROJECT_SOURCE_DIR}/src/jctrans.c 
			 ${PROJECT_SOURCE_DIR}/src/jdapimin.c 
			 ${PROJECT_SOURCE_DIR}/src/jdapistd.c
			 ${PROJECT_SOURCE_DIR}/src/jdatadst.c 
			 ${PROJECT_SOURCE_DIR}/src/jdatasrc.c 
			 ${PROJECT_SOURCE_DIR}/src/jdcoefct.c 
			 ${PROJECT_SOURCE_DIR}/src/jdcolor.c 
			 ${PROJECT_SOURCE_DIR}/src/jddctmgr.c 
			 ${PROJECT_SOURCE_DIR}/src/jdhuff.c
			 ${PROJECT_SOURCE_DIR}/src/jdinput.c 
			 ${PROJECT_SOURCE_DIR}/src/jdmainct.c 
			 ${PROJECT_SOURCE_DIR}/src/jdmarker.c 
			 ${PROJECT_SOURCE_DIR}/src/jdmaster.c 
			 ${PROJECT_SOURCE_DIR}/src/jdmerge.c
			 ${PROJECT_SOURCE_DIR}/src/jdpostct.c 
			 ${PROJECT_SOURCE_DIR}/src/jdsample.c 
			 ${PROJECT_SOURCE_DIR}/src/jdtrans.c 
			 ${PROJECT_SOURCE_DIR}/src/jerror.c 
			 ${PROJECT_SOURCE_DIR}/src/jfdctflt.c 
			 ${PROJECT_SOURCE_DIR}/src/jfdctfst.c
			 ${PROJECT_SOURCE_DIR}/src/jfdctint.c 
			 ${PROJECT_SOURCE_DIR}/src/jidctflt.c 
			 ${PROJECT_SOURCE_DIR}/src/jidctfst.c 
			 ${PROJECT_SOURCE_DIR}/src/jidctint.c 
			 ${PROJECT_SOURCE_DIR}/src/jquant1.c
			 ${PROJECT_SOURCE_DIR}/src/jquant2.c 
			 ${PROJECT_SOURCE_DIR}/src/jutils.c 
			 ${PROJECT_SOURCE_DIR}/src/jmemmgr.c 
			 ${PROJECT_SOURCE_DIR}/src/jcarith.c 
			 ${PROJECT_SOURCE_DIR}/src/jdarith.c 
			 ${PROJECT_SOURCE_DIR}/src/jaricom.c 
			 ${PROJECT_SOURCE_DIR}/src/jmemnobs.c
)

set(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)


# android only builds shared libraries - for now. figure out how ovr did it
if(WINDOWS)
	# target_link_libraries(${PROJECT_NAME} Image360WindowsLauncher)
	add_library(${PROJECT_NAME} STATIC  ${INCLUDE_DIRS} ${SOURCES})
else()
	add_library(${PROJECT_NAME} SHARED ${INCLUDE_DIRS} ${SOURCES})
endif()

