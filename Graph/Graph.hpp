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
    };

    class Vertex : V, std::vector<E>
    {
    };

    std::shared_ptr<std::vector<V>> _graph;

public:
    Graph();
    Graph(int);
    ~Graph();

    void edge_dir(int, int, E);
    void edge_bidir(int, int, E);
    friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};
#include "Graph.cpp"
#endif