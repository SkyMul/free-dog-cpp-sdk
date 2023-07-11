# Compute installation prefix relative to this file.
get_filename_component(_dir "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_prefix "${_dir}/../.." ABSOLUTE)

# Import the targets.
include("${_prefix}/lib/free_dog_sdk_cpp/free_dog_sdk_cpp-targets.cmake")

# Report other information.
set(free_dog_sdk_cpp_INCLUDE_DIRS "${_prefix}/include/free_dog_sdk_cpp")
set(free_dog_sdk_cpp_LIBRARIES "${_prefix}/lib/free_dog_sdk_cpp/libfree_dog_sdk_cpp.so")

