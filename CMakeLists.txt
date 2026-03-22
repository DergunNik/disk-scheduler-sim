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
        src/core/SimulationEvent.h
        src/scheduler/IScheduler.h
        src/metrics/MetricsCollector.cpp
        src/metrics/MetricsCollector.h
        src/scheduler/rand/RandScheduler.cpp
        src/scheduler/rand/RandScheduler.h
        src/scheduler/FCFS/FcsfScheduler.cpp
        src/scheduler/FCFS/FcsfScheduler.h
        src/scheduler/SCAN/ScanScheduler.cpp
        src/scheduler/SCAN/ScanScheduler.h
        src/scheduler/SCAN/CscanScheduler.cpp
        src/scheduler/SCAN/CscanScheduler.h
        src/scheduler/SSTF/SstfScheduler.cpp
        src/scheduler/SSTF/SstfScheduler.h
        src/scheduler/SCAN/ScanScheduler.h
)

add_custom_command(
        TARGET disk_scheduler_sim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "${CMAKE_BINARY_DIR}/data"
)

