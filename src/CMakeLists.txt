# CMakeList.txt : CMake project for src, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)


project ("src")


add_subdirectory(Client)
#set(OPT_WARNINGS_AS_ERRORS_DEVELOPER_DEFAULT TRUE)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-checks=*")
set(CMAKE_C17_CPPCHECK "cppcheck")

# Add source to this project's executable.
add_executable (src "main.cpp" "server.h" "server.cpp" "message.h" )

#----------------------------------------------------------
##################### SOURCE / HEADERS ####################
#----------------------------------------------------------

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libsodium/src/libsodium)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libsodium/src/libsodium/include)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/libsodium/src/libsodium/include/sodium)
#For version.h.in to version.h
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/libsodium/src/libsodium/include/sodium)

file(GLOB_RECURSE libsodium_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/libsodium/src/libsodium/*.h)

file(GLOB_RECURSE libsodium_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/libsodium/src/libsodium/*.c)

#----------------------------------------------------------
######################### LIBRARY #########################
#----------------------------------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/build")


IF(ENABLE_BLOCKING_RANDOM)
    ADD_DEFINITIONS("-DUSE_BLOCKING_RANDOM")
ENDIF(ENABLE_BLOCKING_RANDOM)

IF(LIBSODIUM_BUILD_SHARED_LIBRARIES)
    ADD_LIBRARY(libsodium SHARED ${libsodium_HEADERS} ${libsodium_SOURCE})
ELSE(LIBSODIUM_BUILD_SHARED_LIBRARIES)
    ADD_DEFINITIONS("-DSODIUM_STATIC")
    ADD_LIBRARY(libsodium STATIC ${libsodium_HEADERS} ${libsodium_SOURCE})
ENDIF(LIBSODIUM_BUILD_SHARED_LIBRARIES)

SET_TARGET_PROPERTIES(libsodium PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/lib
    PROJECT_LABEL "libsodium"
)