cmake_minimum_required(VERSION 3.16.3)

project(Graph VERSION 1.0)

configure_file(Graph_config.hpp.in Graph_config.hpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# add_subdirectory(tests)

# add_library(compiler_flags INTERFACE)
# target_compile_features(compiler_flags INTERFACE cxx_std_20)

# set(gcc_like_cxx
# "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
# set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")
# target_compile_options(compiler_flags INTERFACE
# "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wpedantic>>"
# "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
# )
# add_executable(graph_tests.out tests/graph_tests.cpp)
add_subdirectory(Graph)

add_executable(graph_tests.out main.cpp)

target_include_directories(
    graph_tests.out
    PUBLIC
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Graph"
)

target_link_libraries(graph_tests.out PUBLIC Graph)