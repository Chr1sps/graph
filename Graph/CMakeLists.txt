add_library(
    Graph
    Graph.cpp
)
target_include_directories(Graph PUBLIC exceptions)

# if(BETTER_FLAGS)
# target_link_libraries(Graph PUBLIC compiler_flags)
# endif()