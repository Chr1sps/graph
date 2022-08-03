#ifndef __GRAPH_HPP_CHRISPS__
#define __GRAPH_HPP_CHRISPS__
#include <cstddef>
#include <map>
#include <memory>
#include <optional>
#include <vector>
template <typename V, typename E, typename Id = int>
class Graph
{
    class Vertex : public std::map<Id, std::optional<E>>, public std::optional<V>
    {

    public:
        Vertex();
        Vertex(V);

        using std::optional<V>::operator bool;
        using std::optional<V>::has_value;
        using std::optional<V>::value;
        using std::optional<V>::value_or;
        using std::optional<V>::reset;

        using std::map<Id, std::optional<E>>::operator[];
        using std::map<Id, std::optional<E>>::insert;
        using std::map<Id, std::optional<E>>::begin;
        using std::map<Id, std::optional<E>>::end;
    };

    std::map<Id, Vertex> graph_;

public:
    Graph();
    ~Graph();

    void edge_dir(Id, Id, std::optional<E> = std::nullopt);

    void edge_bidir(Id, Id, std::optional<E> = std::nullopt);

    E get_edge_data(Id, Id);
    V get_vertex_data(Id);

    std::string to_string();

    // friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};

#include "Graph.cpp"
#endif