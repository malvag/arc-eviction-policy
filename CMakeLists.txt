cmake_minimum_required(VERSION 3.10)

project(kv_cache)

# Abseil requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Process Abseil's CMake build system
add_subdirectory(deps/abseil-cpp)

add_executable(arc src/arc.cc)

# Declare dependency on the absl::strings library
target_link_libraries(arc absl::flat_hash_map absl::base absl::log_initialize absl::log)