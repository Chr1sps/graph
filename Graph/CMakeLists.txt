add_library(
    Graph
    Graph.cpp
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

# if(BETTER_FLAGS)
# target_link_libraries(Graph PUBLIC compiler_flags)
# endif()