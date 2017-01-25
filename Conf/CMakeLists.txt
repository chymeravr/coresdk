cmake_minimum_required (VERSION 3.2)

project (Conf)

set (INCLUDE_DIRS ${REPO_PATH}/thirdParty/glew/glew-${GLEW_VERSION}/include ${PROJECT_SOURCE_DIR}/include)


# Expose GLImplementation's definitions
# to other subprojects through cache variable.
#set(${PROJECT_NAME}_DEFINITIONS ${CoreEngine_DEFINITIONS}
#    CACHE INTERNAL "${PROJECT_NAME}: Definitions" FORCE)

# Expose GLImplementation's public includes (including the ones of GLImplementation's dependencies transitively)
# to other subprojects through cache variable.
set(${PROJECT_NAME}_INCLUDE_DIRS ${INCLUDE_DIRS}
    CACHE INTERNAL "${PROJECT_NAME}: Include Directories" FORCE)
#set(${PROJECT_NAME}_GL_LINK_DIRS ${GL_LINK_DIRS}
#	CACHE INTERNAL "${PROJECT_NAME}: GL Link Directories" FORCE)