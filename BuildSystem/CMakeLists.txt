cmake_minimum_required(VERSION 3.12)
project(CASAL2)

# Variables
SET(EXE_NAME "casal2")
SET(OUTPUT_PATH "" CACHE STRING "Combination of OS and Compiler from CommandLine e.g. -D windows_gcc")

# Change the output folders for our build to something cleaner and platform dependent
SET(CMAKE_ROOT   bin/cmake)

STRING(REPLACE "BuildSystem" "CASAL2" PROJECT_HOME_DIRECTORY ${CMAKE_HOME_DIRECTORY})
MESSAGE("-- Root Folder: ${PROJECT_HOME_DIRECTORY}")

# GLOBAL BUILD VARIABLES AND LIBRARIES
SET(COMPILE_OPTIONS "-Wall -Werror -fmessage-length=0 -Wno-unused-local-typedefs")
IF (DEBUG)
 SET(COMPILE_OPTIONS "-O0 -g3 ${COMPILE_OPTIONS}")
ELSEIF (RELEASE)
 SET(COMPILE_OPTIONS "-O2 -g0 ${COMPILE_OPTIONS}")
ELSEIF (TESTMODE)
 SET(COMPILE_OPTIONS "-DTESTMODE -O2 -g0 ${COMPILE_OPTIONS}")
ELSE()
 SET(COMPILE_OPTIONS "-O2 -g0 ${COMPILE_OPTIONS}")
ENDIF ()

# Set variables based on OS
IF (WIN32)
 IF (NOT MSVC)
 	SET(COMPILE_OPTIONS "${COMPILE_OPTIONS} -D__GNUDOS__ -std=c++17")
 	SET(thirdPartyLibraries ${thirdPartyLibraries} pthread)
 ENDIF()
ELSE (WIN32)
 SET(COMPILE_OPTIONS "${COMPILE_OPTIONS} -fPIC -std=c++17")
 SET(thirdPartyLibraries ${thirdPartyLibraries} pthread rt)
ENDIF ()
IF (MSVC)
 SET(COMPILE_OPTIONS "/std:c++17 /MP /O2 /GT /GL")
ENDIF ()
MESSAGE("-- Compiler Options: ${COMPILE_OPTIONS}")

INCLUDE_DIRECTORIES(SYSTEM "${CMAKE_HOME_DIRECTORY}/bin/${OUTPUT_PATH}/thirdparty/include")
INCLUDE_DIRECTORIES("${PROJECT_HOME_DIRECTORY}/source")

# This snippet of code will find all of our source and test files
# and auto-populate variables with them for the build
FILE(GLOB_RECURSE sourceFiles ${PROJECT_HOME_DIRECTORY}/source/*.cpp)
IF (NOT TESTMODE)
  FILE(GLOB_RECURSE testFiles ${PROJECT_HOME_DIRECTORY}/source/*.Test.cpp)
  list(REMOVE_ITEM sourceFiles ${testFiles})
ENDIF ()

# Get a list of our third party libraries
IF (RELEASE)
 SET(thirdPartyFolder "bin/${OUTPUT_PATH}/thirdparty/lib/release/")
ELSE (RELEASE)
 SET(thirdPartyFolder "bin/${OUTPUT_PATH}/thirdparty/lib/debug/")
ENDIF ()
MESSAGE("Third Party Folder: ${thirdPartyFolder}")
LINK_DIRECTORIES("${CMAKE_HOME_DIRECTORY}/${thirdPartyFolder}")

IF (MSVC)
	FILE(GLOB thirdPartyLibrariesRaw RELATIVE ${CMAKE_HOME_DIRECTORY} ${thirdPartyFolder}*.lib)
	FOREACH(library ${thirdPartyLibrariesRaw})
  		STRING(REPLACE ${thirdPartyFolder} "" shortLibrary ${library})
  		SET(thirdPartyLibraries " ") #${thirdPartyLibraries} ${shortLibrary})
	ENDFOREACH()	
	SET(thirdPartyLibraries " ")
ELSE(MSVC)
	FILE(GLOB thirdPartyLibrariesRaw RELATIVE ${CMAKE_HOME_DIRECTORY} ${thirdPartyFolder}*.a)
	FOREACH(library ${thirdPartyLibrariesRaw})
	  STRING(REPLACE ${thirdPartyFolder} "" shortLibrary ${library})
	  SET(thirdPartyLibraries ${thirdPartyLibraries} -l:${shortLibrary})
	ENDFOREACH()
ENDIF()

IF (TESTMODE)
  LIST(SORT thirdPartyLibraries)
ENDIF()

SET(LINK_OPTIONS " ")
IF(NOT TESTMODE)
	IF (NOT MSVC)
		IF(WIN32)
		  MESSAGE("Building ICON")
		  ADD_CUSTOM_TARGET(icon ALL COMMAND windres --output "${CMAKE_HOME_DIRECTORY}/icon.o" --input "${CMAKE_HOME_DIRECTORY}/casal2.rc")
		  MESSAGE("Icon Built")
		  SET(LINK_OPTIONS "-static-libgcc -static-libstdc++ -static ${CMAKE_HOME_DIRECTORY}/icon.o")  
		ENDIF(WIN32)
	ELSE()
		SET(LINK_OPTIONS " ")
	ENDIF()
ENDIF()
MESSAGE("Link Options: ${LINK_OPTIONS}")
MESSAGE("Third party libraries: ${thirdPartyLibraries}")

ADD_EXECUTABLE(${EXE_NAME} ${sourceFiles} )


SET_TARGET_PROPERTIES(${EXE_NAME} PROPERTIES COMPILE_FLAGS ${COMPILE_OPTIONS} LINK_FLAGS ${LINK_OPTIONS})
TARGET_LINK_LIBRARIES(${EXE_NAME} ${thirdPartyLibraries})
IF(NOT TESTMODE)
	IF (NOT MSVC)
		IF(WIN32)
		  ADD_DEPENDENCIES(${EXE_NAME} icon)
		ENDIF(WIN32)
	ENDIF()
ENDIF()