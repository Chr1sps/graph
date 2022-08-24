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

configure_file(include/Graph_config.hpp.in Graph_config.hpp)

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

if(BETTER_FLAGS)
    list(APPEND installable_libs compiler_flags)
    target_link_libraries(Graph PUBLIC compiler_flags)
endif()

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
