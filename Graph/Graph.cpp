#ifndef __GRAPH_CPP_CHRISPS
#define __GRAPH_CPP_CHRISPS
#include "Graph.hpp"
#include <string>
template <typename V, typename E>
inline Graph<V, E>::Edge::Edge(int vertex, E edge) : _vertex(vertex), E(edge)
{
}
template <typename V, typename E>
inline const int Graph<V, E>::Edge::get_vertex()
{
    return _vertex;
}
template <typename V, typename E>
inline Graph<V, E>::Graph(int n) : _graph(std::vector<Vertex>(n))
{
}
template <typename V, typename E>
inline Graph<V, E>::~Graph()
{
}
template <typename V, typename E>
inline void Graph<V, E>::edge_dir(int start, int end, E data)
{
    _graph[start].push_back(Edge(end, data));
}
template <typename V, typename E>
inline void Graph<V, E>::edge_bidir(int start, int end, E data)
{
    _graph[start].push_back(Edge(end, data));
    _graph[end].push_back(Edge(start, data));
}
template <typename V, typename E>
inline std::string Graph<V, E>::to_string()
{
    std::string result = "";
    int i = 0;
    for (Vertex v : _graph)
    {
        result += std::to_string(i++) + ":";
        for (Edge e : v)
        {
            result += ' ' + std::to_string(e.get_vertex());
        }
        result += '\n';
    }
    return result;
}
#endif