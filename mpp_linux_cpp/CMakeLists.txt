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

include_directories(
${CMAKE_CURRENT_SOURCE_DIR}/home/rpdzkj/Downloads/mpp/inc/
${CMAKE_CURRENT_SOURCE_DIR}/home/rpdzkj/Downloads/mpp/
${CMAKE_CURRENT_SOURCE_DIR}/home/rpdzkj/Downloads/mpp/osal/linux/
${CMAKE_CURRENT_SOURCE_DIR}/home/rpdzkj/Downloads/mpp/osal/inc/
)
target_link_libraries(mpp_linux_demo mpp pthread drm)
#[[
target_include_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/inc)
target_link_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/inc)
target_include_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/)
target_link_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/)
target_include_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/osal/linux/)
target_link_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/osal/linux/)
target_include_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/osal/inc/)
target_link_directories(mpp_linux_demo PRIVATE /home/rpdzkj/Downloads/mpp/osal/inc/)

target_link_libraries(mpp_linux_demo mpp pthread drm)
]]