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

template <typename V, typename E, typename Id>
template <typename U>
inline std::string Graph<V, E, Id>::fmt_(const std::string &fmt, U &&arg)
{
    std::ostringstream sstr;
    bool bracket = false;
    for (auto it = fmt.begin(); it != fmt.end(); ++it)
    {
        switch (*it)
        {
        case '{':
            break;
        case '}':
            sstr << arg << std::string(++it, fmt.end());
            return sstr.str();
        default:
            if (!bracket)
                sstr << (*it);
            break;
        }
    }
    return sstr.str();
}
template <typename V, typename E, typename Id>
template <typename U, typename... Args>
inline std::string Graph<V, E, Id>::fmt_(const std::string &fmt, U &&arg, Args &&...args)
{
    std::ostringstream sstr;
    bool bracket = false;
    for (auto it = fmt.begin(); it != fmt.end(); ++it)
    {
        switch (*it)
        {
        case '{':
            break;
        case '}':
            sstr << arg << fmt_(std::string(++it, fmt.end()), std::forward<Args>(args)...);
            return sstr.str();
        default:
            if (!bracket)
                sstr << (*it);
            break;
        }
    }
    return sstr.str();
}
template <typename V, typename E, typename Id>
template <typename... Args>
inline std::string Graph<V, E, Id>::format_(const std::string &fmt, Args &&...args)
{
#ifdef HAS_FORMAT
    return std::format(fmt, args);
#else
    return fmt_(fmt, std::forward<Args>(args)...);
#endif
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::is_edge_equal_(Id start, Id end,
                                            Id other_start, Id other_end) const
{
    return graph_.at(start).at(end) == graph_.at(other_start).at(other_end);
}

// Graph::Vertex methods
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::Vertex::Vertex(Vopt data) : Vopt(data)
{
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::Vertex::operator==(const Vertex &other) const
{
    if (dynamic_cast<Vopt>(*this) != dynamic_cast<Vopt>(other))
        return false;
    for (auto [k, v] : *this)
    {
        try
        {
            if (v != other.at(k))
                return false;
        }
        catch (const std::out_of_range &e)
        {
            return false;
        }
    }
    return true;
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
inline void Graph<V, E, Id>::erase_vertex(Id id)
{
    for (auto &v : graph_)
    {
        v.second.erase(id);
    }
    graph_.erase(id);
}

/**
 * @brief Creates a new vertex of given id and value.
 *
 * @tparam  V  vertex data type.
 * @tparam  E  edge data type.
 * @tparam  Id  vertex identification type.
 * @param  id  vertex id.
 * @param  value  (optional) value to be associated with the vertex
 * (vertex will have no value if omitted).
 */
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::make_vertex(Id id, Vopt value)
{
    graph_[id] = Vertex(value);
}

/**
 * @brief Creates a one-directional edge from start to end with a given value.
 * Additionally creates vertices if needed.
 *
 * @tparam  V  vertex data type.
 * @tparam  E  edge data type.
 * @tparam  Id  vertex identification type.
 * @param  start  edge starting point.
 * @param  end  edge ending point.
 * @param  data  (optional) value to be associated with the edge.
 * (edge will have no value if omitted).
 */
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::make_dir(Id start, Id end, Eopt data)
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

/**
 * @brief Creates a two-directional edge from start to end with a given value.
 * Additionally creates vertices if needed. The value is given to both
 * directions of the edge (going from start to end gives you the same value as
 * going from end to start).
 *
 * @tparam  V  vertex data type.
 * @tparam  E  edge data type.
 * @tparam  Id  vertex identification type.
 * @param  start  edge starting point.
 * @param  end  edge ending point.
 * @param  data  (optional) value to be associated with the edge
 * (edge will have no value if omitted).
 */
template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::make_bidir(Id start, Id end, Eopt data)
{
    graph_[start][end] = data;
    graph_[end][start] = data;
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::join_dir(Id start, Id end, Eopt data)
{
    try
    {
        graph_.at(end);
        graph_.at(start)[end] = data;
    }
    catch (const std::out_of_range &e)
    {
        throw VertexNotFoundException("Vertex was not found.");
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::join_bidir(Id start, Id end, Eopt data)
{
    try
    {
        join_dir(start, end, data);
        join_dir(end, start, data);
    }
    catch (const VertexNotFoundException &e)
    {
        throw e;
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::update_dir(Id start, Id end, Eopt data)
{
    try
    {
        graph_.at(start).at(end) = data;
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
        graph_.at(start).at(end) = data;
        graph_.at(end).at(start) = data;
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::erase_dir(Id start, Id end)
{
    graph_[start].erase(end);
}

template <typename V, typename E, typename Id>
inline void Graph<V, E, Id>::erase_bidir(Id start, Id end)
{
    erase_dir(start, end);
    erase_dir(end, start);
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::is_vertex(Id id) const
{
    return graph_.contains(id);
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::is_dir(Id start, Id end) const
{
    try
    {
        return graph_.at(start).contains(end);
    }
    catch (const std::exception &e)
    {
        throw VertexNotFoundException("Vertex was not found.");
    }
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::is_bidir(Id start, Id end) const
{
    try
    {
        if (!is_dir(start, end))
            return false;
        if (!is_dir(end, start))
            return false;
        return is_edge_equal_(start, end, end, start);
    }
    catch (const VertexNotFoundException &e)
    {
        throw e;
    }
}

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::has_data(Id id) const
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

template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::has_data(Id start, Id end) const
{
    try
    {
        return graph_.at(start).at(end).has_value();
    }
    catch (const std::out_of_range &e)
    {
        throw EdgeNotFoundException("Edge was not found.");
    }
}

template <typename V, typename E, typename Id>
inline V Graph<V, E, Id>::data(Id id) const
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
inline E Graph<V, E, Id>::data(Id start, Id end) const
{
    try
    {
        return graph_.at(start).at(end).value();
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

// graph printing method
template <typename V, typename E, typename Id>
inline std::string Graph<V, E, Id>::to_string() const
{
    std::string result = "";
    for (const auto &v : graph_)
    {
        if (v.second.has_value())
            result += format_("({}: {}):", v.first, v.second.value());
        else
            result += format_("{}:", v.first);
        for (const auto &[ekey, evalue] : v.second)
        {
            if (evalue.has_value())
                result += format_(" ({}: {})", ekey, evalue.value());
            else
                result += format_(" {}", ekey);
        }
        result += '\n';
    }
    return result;
}

/**
 * @brief Returns the amount of vertices a graph has.
 *
 * @tparam V
 * @tparam E
 * @tparam Id
 * @return std::size_t
 */
template <typename V, typename E, typename Id>
inline std::size_t Graph<V, E, Id>::size() const
{
    return graph_.size();
}

/**
 * @brief Return true if the graph contains no vertices.
 *
 * @tparam V
 * @tparam E
 * @tparam Id
 * @return true
 * @return false
 */
template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::empty() const
{
    return graph_.empty();
}

/**
 * @brief Returns true if the graph has at least a single vertex.
 *
 * @tparam V
 * @tparam E
 * @tparam Id
 * @return true
 * @return false
 */
template <typename V, typename E, typename Id>
inline Graph<V, E, Id>::operator bool() const
{
    return !empty();
}

/**
 * @brief Returns true if both graphs are identical. Conditions that must be
 * met:
 *
 *
 * @tparam V
 * @tparam E
 * @tparam Id
 * @param other
 * @return true
 * @return false
 */
template <typename V, typename E, typename Id>
inline bool Graph<V, E, Id>::operator==(const Graph &other) const
{
    return this->graph_ == other.graph_;
}
#endif