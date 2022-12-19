# find_package (Boost COMPONENTS system filesystem unit_test_framework REQUIRED)
include_directories(../include/)
add_executable (test_arc_simple test_arc.cc)
target_link_libraries (test_arc_simple arc absl::flat_hash_map absl::base absl::log_initialize absl::log)