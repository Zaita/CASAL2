cmake_minimum_required(VERSION 3.12)
project(kthohr-mcmc)


# Change the output folders for our build to something cleaner and platform dependent
SET(EXE_NAME "kthohr")
SET(CMAKE_ROOT   build)

MESSAGE("-- CMake Home Directory: ${CMAKE_HOME_DIRECTORY}")

# GLOBAL BUILD VARIABLES AND LIBRARIES
SET(COMPILE_OPTIONS "-Wall -Werror -fmessage-length=0 -Wno-unused-local-typedefs")
SET(COMPILE_OPTIONS "-O2 -g0 ${COMPILE_OPTIONS}")

# Set variables based on OS
IF (WIN32)
 IF (NOT MSVC)
 	SET(COMPILE_OPTIONS "${COMPILE_OPTIONS} -D__GNUDOS__ -std=c++17")
 ENDIF()
ELSE (WIN32)
 SET(COMPILE_OPTIONS "${COMPILE_OPTIONS} -fPIC -std=c++17")
ENDIF ()
IF (MSVC)
 SET(COMPILE_OPTIONS "/std:c++17 /MP /O2 /GT /GL")
ENDIF ()
MESSAGE("-- Compiler Options: ${COMPILE_OPTIONS}")

INCLUDE_DIRECTORIES(SYSTEM "${CMAKE_HOME_DIRECTORY}/../armadillo-code-9.900.x/include")
INCLUDE_DIRECTORIES(SYSTEM "${CMAKE_HOME_DIRECTORY}/include")

# This snippet of code will find all of our source and test files
# and auto-populate variables with them for the build
FILE(GLOB_RECURSE sourceFiles ${CMAKE_HOME_DIRECTORY}/src/*.cpp)

SET(CMAKE_SOURCE_DIR "${CMAKE_HOME_DIRECTORY}/src/")

SET(LINK_OPTIONS " ")
IF (NOT MSVC)
	SET(LINK_OPTIONS "-static-libgcc -static-libstdc++ -static")  
ELSE()
	SET(LINK_OPTIONS " ")
ENDIF()

MESSAGE("Link Options: ${LINK_OPTIONS}")
MESSAGE("Third party libraries: ${thirdPartyLibraries}")

ADD_LIBRARY(${EXE_NAME} STATIC ${sourceFiles} )
SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS} LINK_FLAGS ${LINK_OPTIONS})
