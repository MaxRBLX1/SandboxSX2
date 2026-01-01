<<<<<<< HEAD
cmake_minimum_required(VERSION 3.22)

project(ps2native)

add_library(ps2native SHARED
        ps2_jni.cpp
        core/ps2_core.cpp
        core/ee_cpu.cpp
        core/ee_decode.cpp
        core/iop_cpu.cpp
        core/mem_map.cpp
        core/dma_stub.cpp
        core/gs_stub.cpp
        core/sif_stub.cpp
        core/scheduler.cpp
        core/debug_bus.cpp
        core/timers.cpp
)

target_include_directories(ps2native
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/core
)

find_library(log-lib log)
target_link_libraries(ps2native ${log-lib})
=======
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
>>>>>>> 98c30722c5cc4b68755cfb441b2bedf3dede0946
