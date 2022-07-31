#ifndef __GRAPH_HPP_CHRISPS__
#define __GRAPH_HPP_CHRISPS__
#include <cstddef>
#include <iostream>
#include <memory>
#include <vector>
template <typename V = std::nullptr_t, typename E = std::nullptr_t>
class Graph
{
    class Edge
    {
        int vertex_;
        E data_;

    public:
        Edge(int);
        Edge(int, E);
        int get_vertex();
        void set_data(const E &);
        const E get_data();
    };

    class Vertex : public std::vector<Edge>
    {
        V data_;

    public:
        Vertex();
        Vertex(V);

        const V get_data();
        void set_data(const V &);
    };

    std::vector<Vertex> graph_;

public:
    Graph(int = 0);
    ~Graph();

    void edge_dir(int, int, E = E());
    void edge_bidir(int, int, E = E());

    E get_edge_data(int, int);
    V get_vertex_data(int);

    std::string to_string();
    friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};
#include "Graph.cpp"
#endif