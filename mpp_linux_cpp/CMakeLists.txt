set(CMAKE_C_COMPILER "/usr/bin/gcc")
set(CMAKE_CXX_coMPILER "/usr/bin/g++")
cmake_minimum_required(VERSION 2.8)

project(mpp_linux_demo)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp
        mpp/Codec.cpp
        thread/Thread.cpp
        rkrga/RGA.cpp
        rkdrm/bo.c
        rkdrm/dev.c
        rkdrm/modeset.c)
add_executable(mpp_linux_demo ${SOURCE_FILES})

#[[
find_library(MPP_LIB rockchip_mpp)
target_link_libraries(mpp_linux_demo PRIVATE "${MPP_LIB}")
]]

target_include_directories(mpp_linux_demo PRIVATE ../mpp/osal/inc/)
target_link_directories(mpp_linux_demo PRIVATE ../mpp/osal/inc/)
target_include_directories(mpp_linux_demo PRIVATE ../mpp/inc)
target_link_directories(mpp_linux_demo PRIVATE ../mpp/inc)
target_include_directories(mpp_linux_demo PRIVATE ../mpp/)
target_link_directories(mpp_linux_demo PRIVATE ../mpp/)
target_include_directories(mpp_linux_demo PRIVATE ../mpp/osal/linux/)
target_link_directories(mpp_linux_demo PRIVATE ../mpp/osal/linux/)

target_link_libraries(mpp_linux_demo rockchip_mpp pthread drm)
