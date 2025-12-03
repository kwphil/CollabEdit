# Rust Bridge - C++ wrapper around Rust FFI
add_library(rust-bridge STATIC)

# Find and link Rust libraries
find_package(Rust REQUIRED)

# Build Rust dependencies first
add_rust_library(crypto-engine
    ${CMAKE_CURRENT_SOURCE_DIR}/../../rust/crypto-engine
    STATIC
    RELEASE
)

target_sources(rust-bridge PRIVATE
    src/crypto_wrapper.cpp
    src/crypto_wrapper.hpp
)

target_include_directories(rust-bridge PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}/../../bindings/cpp
)

target_link_libraries(rust-bridge
    crypto-engine
)

# Add -pthread and other system libraries needed by Rust
if(UNIX AND NOT APPLE)
    target_link_libraries(rust-bridge pthread dl rt)
endif()

if(APPLE)
    target_link_libraries(rust-bridge System)
endif()
