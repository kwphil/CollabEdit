cmake_minimum_required(VERSION 3.15)
project(CollabEdit VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Rust 1.70 REQUIRED)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(USE_SYSTEM_CRYPTO "Use system crypto libraries" OFF)
option(ENABLE_LTO "Enable Link Time Optimization" ON)

set(RUST_TARGET_DIR "${CMAKE_CURRENT_BINARY_DIR}/rust-target")
set(RUST_CRATE_TYPE "staticlib")  

add_subdirectory(rust)      
add_subdirectory(cpp)       

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/bindings/cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/cpp/include
)

install(DIRECTORY include/ DESTINATION include)
install(TARGETS collaboration-server DESTINATION bin)
