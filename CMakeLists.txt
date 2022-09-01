cmake_minimum_required(VERSION 3.16.3)

project(Graph VERSION 0.8)

# compiler flags fuckery
option(USE_LIBCPP "Use clang libc++ instead of gnu libstdc++" FALSE)
option(USE_EXPERIMENTAL "Use experimental clang libc++ features" FALSE)

add_library(compiler_flags INTERFACE)

target_compile_features(compiler_flags INTERFACE cxx_std_20)

set(gcc_like_cxx
    "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

target_compile_options(compiler_flags INTERFACE
    "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused;-Wpedantic;-Werror>>"
    "$<${msvc_cxx}:$<BUILD_INTERFACE:-W3>>"
)
target_link_options(compiler_flags INTERFACE "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-fsanitize=undefined>>")

if(USE_LIBCPP)
    message("Using clang libc++.")
    target_compile_options(compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-stdlib=libc++>>")

    # ;-lc++;-L/usr/lib/x86-64-linux-gnu/libc++.so
    if(USE_EXPERIMENTAL)
        message("Using experimental features.")
        target_compile_options(compiler_flags INTERFACE
            "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-DLIBCXX_ENABLE_INCOMPLETE_FEATURES=ON>>")
    endif()
else()
    message("Using gnu libstdc++.")
    target_compile_options(compiler_flags INTERFACE
        "$<${gcc_like_cxx}:$<BUILD_INTERFACE:-stdlib=libstdc++>>")
endif()

# directories lists
set(INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(INCLUDE_EXCEPTIONS "${CMAKE_CURRENT_SOURCE_DIR}/include/exceptions")
set(SRC "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(SRC_EXCEPTIONS "${CMAKE_CURRENT_SOURCE_DIR}/src/exceptions")

set(
    EXCEPTIONS_HPP
    "graph_exceptions.hpp"
    "edge_without_value_exception.hpp"
    "vertex_without_value_exception.hpp"
    "edge_not_found_exception.hpp"
    "vertex_not_found_exception.hpp"
)

list(TRANSFORM EXCEPTIONS_HPP PREPEND "${INCLUDE_EXCEPTIONS}/")

set(
    EXCEPTIONS_CPP
    "edge_without_value_exception.cpp"
    "vertex_without_value_exception.cpp"
    "edge_not_found_exception.cpp"
    "vertex_not_found_exception.cpp"
)

list(TRANSFORM EXCEPTIONS_CPP PREPEND "${SRC_EXCEPTIONS}/")

set(GRAPH_CPP "Graph.cpp")
set(GRAPH_HPP "Graph.hpp")

list(TRANSFORM GRAPH_CPP PREPEND "${SRC}/")
list(TRANSFORM GRAPH_HPP PREPEND "${INCLUDE}/")

add_library(
    Graph
    ${GRAPH_CPP}
    ${EXCEPTIONS_CPP}
)

add_library(Graph::Graph ALIAS Graph)

target_include_directories(
    Graph
    PUBLIC
    "$<BUILD_INTERFACE:${SRC}>"
    "$<BUILD_INTERFACE:${INCLUDE}>"
    "$<BUILD_INTERFACE:${INCLUDE_EXCEPTIONS}>"
    "$<INSTALL_INTERFACE:src>"
    "$<INSTALL_INTERFACE:include>"
    "$<INSTALL_INTERFACE:include/exceptions>"
)

# std::format code introspection
include(CheckCXXSourceCompiles)

check_cxx_source_compiles("
#include <format>
#include <string>
using namespace std;
int main()
{
    string s = format(\"Int: {}.\", 5);
    return 0;
}
" HAS_FORMAT)

if(HAS_FORMAT)
    target_compile_definitions(Graph PRIVATE "HAS_FORMAT")
endif()

target_compile_definitions(Graph PRIVATE "EXPORTING_GRAPH")

configure_file(cmake/Graph_config.hpp.in Graph_config.hpp)

# tests target definition
add_subdirectory(tests)

add_executable(graph_tests.out tests/graph_tests.cpp)

target_link_libraries(graph_tests.out PRIVATE Graph Catch2::Catch2WithMain)

# testing stuff
Include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG origin/devel)
FetchContent_MakeAvailable(Catch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR/extras})
include(CTest)
include(Catch)
catch_discover_tests(graph_tests.out)

set(CMAKE_CTEST_ARGUMENTS "--extra-verbose")

# installation related stuff
set(installable_libs Graph)

list(APPEND installable_libs compiler_flags)
target_link_libraries(Graph PUBLIC compiler_flags)

install(TARGETS ${installable_libs} EXPORT GraphTargets DESTINATION lib)
install(FILES ${GRAPH_HPP} DESTINATION include)
install(FILES ${EXCEPTIONS_HPP} DESTINATION include/exceptions)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/Graph_config.hpp" DESTINATION include)

if(INCLUDE_TEST_SOURCE)
    install(FILES tests/graph_tests.cpp DESTINATION tests)
endif()

if(INSTALL_TESTS)
    install(TARGETS graph_tests.out DESTINATION bin)
endif()

set_target_properties(
    Graph PROPERTIES
    POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

# installation related stuff
option(BUILD_SHARED_LIBS "Build using shared libraries" FALSE)
option(INSTALL_TESTS "Add unit tests to installed package" TRUE)
option(INCLUDE_TEST_SOURCE
    "Installed library will include test source code" FALSE
)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Graph_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Graph_VERSION_MINOR}")
set(CPACK_SOURCE_GENERATOR "TGZ")
include(CPack)

install(
    EXPORT GraphTargets
    FILE GraphTargets.cmake
    DESTINATION lib/cmake/Graph
)

include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/GraphConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/config"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/GraphConfigVersion.cmake"
    VERSION "${Graph_VERSION_MAJOR}.${Graph_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)
install(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/GraphConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/GraphConfigVersion.cmake
    DESTINATION lib/cmake
)
export(
    EXPORT GraphTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/GraphTargets.cmake"
)