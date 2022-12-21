cmake_minimum_required(VERSION 3.10)

project(kv_cache 
VERSION 0.1
LANGUAGES C CXX)

# Abseil requires C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

# Process Abseil's CMake build system
# add_subdirectory(deps/abseil-cpp)
add_subdirectory(tests)

include_directories(include/)
add_library (arc STATIC src/arc.cc )
# Declare dependency on the absl
target_link_libraries(arc)

enable_testing()
add_test(NAME test_arc_simple COMMAND $<TARGET_FILE:test_arc_simple>)
