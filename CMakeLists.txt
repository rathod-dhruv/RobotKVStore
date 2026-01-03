cmake_minimum_required(VERSION 3.10)
project(RobotKVStore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.tar.xz
)
FetchContent_MakeAvailable(json)

include_directories(include)

add_executable(RobotKVStore
    src/main.cpp
    src/kv_store.cpp
)

# Create logs directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/../logs)

target_link_libraries(RobotKVStore PRIVATE nlohmann_json::nlohmann_json)
