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
        using std::map<Id, Eopt>::erase;
        using std::map<Id, Eopt>::find;
        using std::map<Id, Eopt>::at;
        using std::map<Id, Eopt>::empty;

        bool operator==(const Vertex &) const;
    };

    std::map<Id, Vertex> graph_;

public:
    Graph();
    Graph(const Graph &);
    Graph(Graph &&);
    virtual ~Graph();

    Graph &operator=(const Graph &);
    Graph &operator=(Graph &&);

    void add_vertex(Id, Vopt = Nil);
    void update_vertex(Id, Vopt = Nil);
    void erase_vertex(Id);

    void make_dir(Id, Id, Eopt = Nil);
    void make_bidir(Id, Id, Eopt = Nil);

    void join_dir(Id, Id, Eopt = Nil);
    void join_bidir(Id, Id, Eopt = Nil);

    void update_dir(Id, Id, Eopt = Nil);
    void update_bidir(Id, Id, Eopt = Nil);

    void erase_dir(Id, Id);
    void erase_bidir(Id, Id);

    bool is_vertex(Id);
    bool is_dir(Id, Id) const;
    bool is_bidir(Id, Id) const;

    bool edge_has_data(Id, Id) const;
    bool vertex_has_data(Id) const;

    E get_edge_data(Id, Id) const;
    V get_vertex_data(Id) const;

    std::string to_string() const;
    // friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);

    explicit operator bool() const;

    bool operator==(const Graph &) const;
};

#include "Graph.cpp"
#endif