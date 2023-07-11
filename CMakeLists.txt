cmake_minimum_required(VERSION 3.14)
project(free_dog_sdk_cpp VERSION 1.0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

find_package(Threads REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${Boost_INCLUDE_DIRS})

# Library
add_library(${PROJECT_NAME} SHARED
        src/common.cpp
        src/lowState.cpp
        src/lowCmd.cpp
        src/unitreeConnection.cpp
        )

set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME}Targets
  DESTINATION lib/${PROJECT_NAME})

file(GLOB HEADERS include/*.h)
install(FILES ${HEADERS} DESTINATION include/${PROJECT_NAME})

# Setup CMake Package Bindings
#configure_file(
#   ${${PROJECT_NAME}_SOURCE_DIR}/pkg/${PROJECT_NAME}-config.cmake.in
#   ${${PROJECT_NAME}_BINARY_DIR}/pkg/${PROJECT_NAME}-config.cmake @ONLY)

 ## Version CMAKE
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}ConfigVersion.cmake"
  VERSION ${VERSION}
  COMPATIBILITY SameMinorVersion
)
 ## Targets CMAKE
export(EXPORT ${PROJECT_NAME}Targets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}/${PROJECT_NAME}-targets.cmake"
  NAMESPACE FREE_DOG_SDK::
)

install(EXPORT ${PROJECT_NAME}Targets
  FILE
    ${PROJECT_NAME}-targets.cmake
  NAMESPACE
    FREE_DOG_SDK::
  DESTINATION
    lib/${PROJECT_NAME}
)

install(FILES pkg/${PROJECT_NAME}-config.cmake
         "${CMAKE_CURRENT_BINARY_DIR}/free_dog_sdk_cpp/free_dog_sdk_cppConfigVersion.cmake"
         DESTINATION lib/${PROJECT_NAME})

# Runner
add_executable(${PROJECT_NAME}_debug_live example/debug_live.cpp)
target_link_libraries(${PROJECT_NAME}_debug_live ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_example_position_lowlevel example/example_position_lowlevel.cpp)
target_link_libraries(${PROJECT_NAME}_example_position_lowlevel ${PROJECT_NAME})

add_executable(${PROJECT_NAME}_debug_print_command example/debug_print_command.cpp)
target_link_libraries(${PROJECT_NAME}_debug_print_command ${PROJECT_NAME})

# Test
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/4a1a299b206ba250a4318f74938ea67c75c3c0c9.zip
)
FetchContent_MakeAvailable(googletest)


enable_testing()

add_executable(
        low_test
        test/low_test.cpp
)
target_link_libraries(
        low_test
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(low_test)

