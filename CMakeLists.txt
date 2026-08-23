cmake_minimum_required(VERSION 3.20)

project(task_scheduler)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(task_scheduler
    src/main.cpp
    src/ThreadPool.cpp
)

target_include_directories(task_scheduler PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)