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
    static constexpr std::nullopt_t Nil = std::nullopt;
    using Vopt = std::optional<V>;
    using Eopt = std::optional<E>;

    class Vertex : public std::map<Id, Eopt>, public Vopt
    {
    public:
        Vertex(Vopt = Nil);

        using Vopt::operator bool;
        using Vopt::has_value;
        using Vopt::reset;
        using Vopt::value;
        using Vopt::value_or;

        using std::map<Id, Eopt>::operator[];
        using std::map<Id, Eopt>::insert;
        using std::map<Id, Eopt>::begin;
        using std::map<Id, Eopt>::end;
    };

    std::map<Id, Vertex> graph_;

public:
    Graph();
    ~Graph();

    void add_vertex(Id, Vopt = Nil);
    void update_vertex(Id, Vopt = Nil);
    void remove_vertex(Id);

    void edge_dir(Id, Id, Eopt = Nil);
    void edge_bidir(Id, Id, Eopt = Nil);

    void update_dir(Id, Id, Eopt = Nil);
    void update_bidir(Id, Id, Eopt = Nil);

    void remove_dir(Id, Id);
    void remove_bidir(Id, Id);

    bool is_edge_dir(Id, Id);
    bool is_edge_bidir(Id, Id);

    E get_edge_data(Id, Id);
    V get_vertex_data(Id);

    std::string to_string();
    // friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);
};

#include "Graph.cpp"
#endif