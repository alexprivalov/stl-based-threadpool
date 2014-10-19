cmake_minimum_required(VERSION 2.8)
#TODO : upgrade to cmake 3.0 when package available on ubuntu/debian
# it will allow for compiler C++11 feature checks

project(threadpool)

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
  # using GNU GCC
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    #to fix a bug in gcc http://stackoverflow.com/questions/19463602/compiling-multithread-code-with-g
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
  # using Clang
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Intel")
  # using Intel C++
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
  # using Visual Studio C++
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

find_package( Threads )

include_directories("include")

add_library(threadpool "src/threadpool.cpp" "include/threadpool/threadpool.h")

add_executable(threadpool_test "src/threadpool_test.cpp")
target_link_libraries(threadpool_test threadpool ${CMAKE_THREAD_LIBS_INIT})
