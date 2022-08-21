set(
    EXCEPTIONS_HPP
    exceptions/graph_exceptions.hpp
    exceptions/edge_without_value_exception.hpp
    exceptions/vertex_without_value_exception.hpp
    exceptions/edge_not_found_exception.hpp
    exceptions/vertex_not_found_exception.hpp
)

set(
    EXCEPTIONS_CPP
    exceptions/edge_without_value_exception.cpp
    exceptions/vertex_without_value_exception.cpp
)

set(GRAPH_CPP Graph.cpp)

add_library(
    Graph
    Graph.cpp
    ${EXCEPTIONS_CPP}
)

target_include_directories(Graph
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/exceptions>
    $<INSTALL_INTERFACE:include/exceptions>
)
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

set(installable_libs Graph)

if(BETTER_FLAGS)
    list(APPEND installable_libs compiler_flags)
    target_link_libraries(Graph PUBLIC compiler_flags)
endif()

install(TARGETS ${installable_libs} EXPORT GraphTargets DESTINATION lib)
install(FILES Graph.hpp DESTINATION include)
install(FILES ${EXCEPTIONS_HPP} DESTINATION include/exceptions)

if(INCLUDE_TEST_SOURCE)
    install(FILES tests/graph_tests.cpp DESTINATION tests)
endif()

if(INSTALL_TESTS)
    install(TARGETS graph_tests.out DESTINATION bin)
endif()

set_target_properties(
    Graph PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
)

add_subdirectory(tests)

add_executable(graph_tests.out tests/graph_tests.cpp)

target_include_directories(
    graph_tests.out
    PRIVATE
    "${PROJECT_BINARY_DIR}"
    "${PROJECT_SOURCE_DIR}/Graph"
)

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
