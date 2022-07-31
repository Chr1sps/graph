#ifndef __GRAPH_H_CHRISPS
#define __GRAPH_H_CHRISPS
#include <iostream>
#include <memory>
#include <vector>
template <class V, class E>
class Graph
{
    class Edge : E
    {
        int _vertex;

    public:
        Edge(int, E);
        int get_vertex();
    };

    class Vertex : public std::vector<Edge>, public V
    {
        // public:
        //     using std::vector<Edge>::push_back;
    };

    std::vector<Vertex> _graph;

public:
    Graph(int = 0);
    ~Graph();

    void edge_dir(int, int, E = E());
    void edge_bidir(int, int, E = E());
    std::string to_string();
    friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};
#include "Graph.cpp"
#endif