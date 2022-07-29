#ifndef __GRAPH_CPP_CHRISPS
#define __GRAPH_CPP_CHRISPS
#include "Graph.hpp"

template <typename V, typename E>
inline Graph<V, E>::Graph()
{
    std::cout << "Graph constructor." << std::endl;
}
template <typename V, typename E>
inline Graph<V, E>::~Graph()
{
    std::cout << "Graph destructor." << std::endl;
}
#endif