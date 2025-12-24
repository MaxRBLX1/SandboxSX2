cmake_minimum_required(VERSION 3.18.1)

project(sandboxsx2)

add_library(
        sandboxsx2
        SHARED
        ps2_core.cpp
        ps2_jni.cpp
)

find_library(
        log-lib
        log
)

target_link_libraries(
        sandboxsx2
        ${log-lib}
)