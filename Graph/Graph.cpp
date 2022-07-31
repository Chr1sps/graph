#ifndef __GRAPH_CPP_CHRISPS__
#define __GRAPH_CPP_CHRISPS__
#include "Graph.hpp"
#include <string>
template <typename V, typename E>
inline Graph<V, E>::Edge::Edge(int vertex, E edge)
    : vertex_(vertex), data_(edge)
{
}
template <typename V, typename E>
inline Graph<V, E>::Edge::Edge(int vertex) : vertex_(vertex)
{
}
template <typename V, typename E>
inline int Graph<V, E>::Edge::get_vertex()
{
    return vertex_;
}
template <typename V, typename E>
inline const E Graph<V, E>::Edge::get_data()
{
    return data_;
}
template <typename V, typename E>
inline void Graph<V, E>::Edge::set_data(const E &data)
{
    data_ = data;
}
template <typename V, typename E>
inline Graph<V, E>::Vertex::Vertex()
{
}
template <typename V, typename E>
inline Graph<V, E>::Vertex::Vertex(V data) : data_(data)
{
}
template <typename V, typename E>
inline const V Graph<V, E>::Vertex::get_data()
{
    return data_;
}
template <typename V, typename E>
inline void Graph<V, E>::Vertex::set_data(const V &data)
{
    data_ = data;
}
template <typename V, typename E>
inline Graph<V, E>::Graph(int n) : graph_(std::vector<Vertex>(n))
{
}
template <typename V, typename E>
inline Graph<V, E>::~Graph()
{
}
template <typename V, typename E>
inline void Graph<V, E>::edge_dir(int start, int end, E data)
{
    graph_[start].push_back(Edge(end, data));
}
template <typename V, typename E>
inline void Graph<V, E>::edge_bidir(int start, int end, E data)
{
    graph_[start].push_back(Edge(end, data));
    graph_[end].push_back(Edge(start, data));
}
template <typename V, typename E>
inline std::string Graph<V, E>::to_string()
{
    std::string result = "";
    int i = 0;
    for (Vertex v : graph_)
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