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

# TODO: Add tests and install targets if needed.
