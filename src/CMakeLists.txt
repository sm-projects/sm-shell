#***********************************************************
# 
#  Project  - SMSHELL - a simple shell program.
#
#***********************************************************
cmake_minimum_required (VERSION 2.8 FATAL_ERROR)
#project name
project (smshell)
#Enable C language
enable_language(C)

#Set project version
set(VERSION_MAJOR 1)
set(VERSION_MINOR 0)
set(VERSION_PATCH 0)

#Guard against in-source builds
#include(safeguards)

# detect operating system
message(STATUS "We are on a ${CMAKE_SYSTEM_NAME} system")
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    add_definitions(-DSYSTEM_LINUX)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    add_definitions(-DSYSTEM_DARWIN)
endif()
if(${CMAKE_SYSTEM_NAME} STREQUAL "AIX")
    add_definitions(-DSYSTEM_AIX)
endif()
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
    add_definitions(-DSYSTEM_WINDOWS)
endif()

# detect host processor
message(STATUS "The host processor is ${CMAKE_HOST_SYSTEM_PROCESSOR}")

# example how to set c++ compiler flags for GNU
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    message(STATUS "You are using GNU C compiler.")
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g3")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

add_executable(smshell smshell.c)
set(CMAKE_C_FLAGS "-std=c99")

add_custom_target(run
     COMMAND smshell
     WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)

add_custom_target(sync
    COMMAND  git 
    WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
