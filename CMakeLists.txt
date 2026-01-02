cmake_minimum_required(VERSION 3.10)
project(robot_kv_store)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(robot_kv_store
    src/main.cpp
    src/kv_store.cpp
    src/network.cpp
    src/robot_node.cpp
)
