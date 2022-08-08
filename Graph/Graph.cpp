#ifndef __GRAPH_CPP_CHRISPS__
#define __GRAPH_CPP_CHRISPS__
#include "Graph.hpp"
#include "graph_exceptions.hpp"
#ifdef HAS_FORMAT
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
inline Graph<V, E, Id>::Vertex::Vertex(Vopt data) : Vopt(data)
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

// vertex add, update and remove
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::add_vertex(Id id, Vopt value)
{
    graph_[id] = Vertex(value);
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::update_vertex(Id id, Vopt value)
{
    try
    {
        graph_.at(id) = Vertex(value);
    }
    catch (const std::out_of_range &e)
    {
        throw VertexNotFoundException("Vertex was not found.");
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::remove_vertex(Id id)
{
    for (auto &v : graph_)
    {
        v.second.erase(id);
    }
    graph_.erase(id);
}

// edge add, update and remove
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::edge_dir(Id start, Id end, Eopt data)
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
inline void Graph<V, E, Id>::edge_bidir(Id start, Id end, Eopt data)
{
    graph_[start][end] = data;
    graph_[end][start] = data;
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::update_dir(Id start, Id end, Eopt data)
{
    try
    {
        graph_.at(start).second.at(end) = data;
        graph_.at(end);
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::update_bidir(Id start, Id end, Eopt data)
{
    try
    {
        graph_.at(start).second.at(end) = data;
        graph_.at(end).second.at(start) = data;
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::remove_dir(Id start, Id end)
{
    graph_[start].erase(end);
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::remove_bidir(Id start, Id end)
{
    remove_dir(start, end);
    remove_dir(end, start);
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::edge_has_data(Id start, Id end) const
{
    try
    {
        return graph_.at(start).second.at(end).has_data();
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
}

template <typename V, typename E, typename Id>
inline V Graph<V, E, Id>::get_vertex_data(Id id) const
{
    try
    {
        return graph_.at(id).value();
    }
    catch (const std::out_of_range &e)
    {
        throw VertexNotFoundException("Vertex was not found.");
    }
    catch (const std::bad_optional_access &e)
    {
        throw VertexWithoutValueException("Vertex has no value.");
    }
}

template <typename V, typename E, typename Id>
inline E Graph<V, E, Id>::get_edge_data(Id start, Id end) const
{
    try
    {
        return graph_.at(start).second.at(end).data();
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
    catch (const std::bad_optional_access &e)
    {
        throw EdgeWithoutValueException("Edge has no value.");
    }
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::vertex_has_data(Id id) const
{
    try
    {
        return graph_.at(id).has_value();
    }
    catch (const std::out_of_range &e)
    {
        throw VertexNotFoundException("Vertex was not found.");
    }
}

// graph printing method
template <typename V, typename E, typename Id>
inline std::string Graph<V, E, Id>::to_string() const
{
    std::string result = "";
    for (const auto &v : graph_)
    {
        if (v.second.has_value())

#ifdef HAS_FORMAT

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