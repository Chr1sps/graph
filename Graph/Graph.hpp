#ifndef __GRAPH_HPP_CHRISPS__
#define __GRAPH_HPP_CHRISPS__
#include <cstddef>
#include <memory>
#include <optional>
#include <vector>
template <typename V, typename E>
class Graph
{
    class Edge : public std::optional<E>
    {
        int vertex_;

    public:
        Edge(int);
        Edge(int, E);
        int get_vertex();

        // using std::optional<E>::operator==;
        // using std::optional<E>::operator!=;
        // using std::optional<E>::operator<;
        // using std::optional<E>::operator<=;
        // using std::optional<E>::operator>;
        // using std::optional<E>::operator>=;
        // using std::optional<E>::operator<=>;
    };

    class Vertex : public std::vector<Edge>, public std::optional<V>
    {

    public:
        Vertex();
        Vertex(V);

        using std::optional<V>::operator bool;
        using std::optional<V>::has_value;
        using std::optional<V>::value;
        using std::optional<V>::value_or;
        using std::optional<V>::reset;

        using std::vector<Edge>::operator[];
        using std::vector<Edge>::push_back;
        using std::vector<Edge>::begin;
        using std::vector<Edge>::end;
    };

    std::vector<Vertex> graph_;

public:
    Graph(int = 0);
    ~Graph();

    void edge_dir(int, int);
    void edge_dir(int, int, E);

    void edge_bidir(int, int);
    void edge_bidir(int, int, E);

    E get_edge_data(int, int);
    V get_vertex_data(int);

    std::string to_string();

    // friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};
#include "Graph.cpp"
#endif