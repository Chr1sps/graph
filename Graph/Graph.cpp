#ifndef __GRAPH_CPP_CHRISPS__
#define __GRAPH_CPP_CHRISPS__
#include "Graph.hpp"
#ifdef USE_LIBCPP
#include <format>
#endif
#include <string>
#include <type_traits>

// Graph::Vertex methods
template <typename V, typename E>
inline Graph<V, E>::Vertex::Vertex()
{
}
template <typename V, typename E>
inline Graph<V, E>::Vertex::Vertex(V data) : std::optional<V>(data)
{
}

// Graph methods
template <typename V, typename E>
inline Graph<V, E>::Graph() : graph_(std::map<int, Vertex>())
{
}
template <typename V, typename E>
inline Graph<V, E>::~Graph()
{
}
template <typename V, typename E>
inline void Graph<V, E>::edge_dir(int start, int end, std::optional<E> data)
{
    graph_[start][end] = data;
    try
    {
        graph_.at(end);
    }
    catch (const std::out_of_range &e)
    {
        graph_[end] = Vertex();
    }
}
template <typename V, typename E>
inline void Graph<V, E>::edge_bidir(int start, int end, std::optional<E> data)
{
    graph_[start][end] = data;
    graph_[end][start] = data;
}
template <typename V, typename E>
inline std::string Graph<V, E>::to_string()
{
    std::string result = "";
    for (const auto &v : graph_)
    {
        if (v.second.has_value())

#if defined(USE_LIBCPP) && __cplusplus >= 202002L

            result += std::format("({}: {}):", v.first, v.second.value());
        else
            result += std::format("{}: ", v.first);
        for (const auto &[ekey, evalue] : v)
        {
            if (evalue.has_value)
                result += std::format(" ({}: {})", ekey, evalue.value());
            else
                result += std::format(" {}", ekey);

#else
        {
            result += "(" + std::to_string(v.first) + ":" + std::to_string(v.second.value()) + "):";
        }
        else
            result += std::to_string(v.first) + ":";
        for (const auto &[ekey, evalue] : v.second)
        {
            if (evalue.has_value())
                result += " (" + std::to_string(ekey) + ": " + std::to_string(evalue.value()) + ")";
            else
                result += " " + std::to_string(ekey);
#endif
        }
        result += '\n';
    }
    return result;
}
#endif