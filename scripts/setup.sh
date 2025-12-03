#!/bin/bash
set -e

echo "Setting up collaboration-engine build environment..."

# Install Rust if not present
if ! command -v rustc &> /dev/null; then
    echo "Installing Rust..."
    curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh -s -- -y
    source $HOME/.cargo/env
fi

# Install CMake if not present
if ! command -v cmake &> /dev/null; then
    echo "Please install CMake 3.15 or higher"
    exit 1
fi

# Install cbindgen for Rust bindings generation
cargo install cbindgen

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_TESTS=ON \
    -DENABLE_LTO=ON

echo "Setup complete! Run './scripts/build-all.sh' to build."
