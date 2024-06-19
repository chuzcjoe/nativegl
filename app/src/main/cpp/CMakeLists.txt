cmake_minimum_required(VERSION 3.22.1)

# Declares and names the project.

project("nativegl")

set(CPP_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

# include directories
include_directories("${CPP_ROOT}/gl"
                    "${CPP_ROOT}/shader"
                    "${CPP_ROOT}/utils")

add_library( # Sets the name of the library.
             nativegl

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${CPP_ROOT}/gl/GLBase.cpp
             ${CPP_ROOT}/gl/GLRender.cpp
             nativegl.cpp
            )

target_link_libraries( # Specifies the target library.
                       nativegl

                       # Links the target library to the log library
                       # included in the NDK.
                       EGL
                       GLESv3
                       android
                       log)
