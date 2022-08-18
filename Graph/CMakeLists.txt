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

target_include_directories(Graph PUBLIC exceptions)
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

install(TARGETS Graph DESTINATION lib)
install(FILES Graph.hpp DESTINATION include)
install(FILES ${EXCEPTIONS_HPP} DESTINATION include/exceptions)

if(BETTER_FLAGS)
    target_link_libraries(Graph PUBLIC compiler_flags)
endif()