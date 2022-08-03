#ifndef __GRAPH_CPP_CHRISPS__
#define __GRAPH_CPP_CHRISPS__
#include "Graph.hpp"
#ifdef USE_LIBCPP
#include <format>
#endif
#include <sstream>
#include <string>
#include <type_traits>

namespace
{
    template <typename T>
    std::string to_str_helper_(T data)
    {
        std::ostringstream sstr;
        sstr << data;
        return sstr.str();
    }
}

// Graph::Vertex methods
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::Vertex::Vertex()
{
}
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::Vertex::Vertex(V data) : std::optional<V>(data)
{
}

// Graph methods
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::Graph() : graph_(std::map<Id, Vertex>())
{
}
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::~Graph()
{
}
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::edge_dir(Id start, Id end, std::optional<E> data)
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
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::edge_bidir(Id start, Id end, std::optional<E> data)
{
    graph_[start][end] = data;
    graph_[end][start] = data;
}
template <typename V, typename E, typename Id>
inline std::string Graph<V, E, Id>::to_string()
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
            result += "(" + to_str_helper_(v.first) + ":" + to_str_helper_(v.second.value()) + "):";
        }
        else
            result += to_str_helper_(v.first) + ":";
        for (const auto &[ekey, evalue] : v.second)
        {
            if (evalue.has_value())
                result += " (" + to_str_helper_(ekey) + ": " + to_str_helper_(evalue.value()) + ")";
            else
                result += " " + to_str_helper_(ekey);
#endif
        }
        result += '\n';
    }
    return result;
}
#endif