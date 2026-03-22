cmake_minimum_required(VERSION 4.1)
project(disk_scheduler_sim)

set(CMAKE_CXX_STANDARD 23)

add_executable(
        disk_scheduler_sim src/main.cpp
        src/request/request.h
        src/request/RequestsLoaders.h
        src/request/RequestsLoaders.cpp
        src/core/SimulationEngine.cpp
        src/core/SimulationEngine.h
        src/disk/DiskState.h
        src/disk/DiskModel.cpp
        src/disk/DiskModel.h
)

add_custom_command(
        TARGET disk_scheduler_sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_BINARY_DIR}/data"
)

