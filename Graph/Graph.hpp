#ifndef __GRAPH_HPP_CHRISPS__
#define __GRAPH_HPP_CHRISPS__

#include <cstddef>
#include <initializer_list>
#include <map>
#include <memory>
#include <optional>
#include <vector>

/**
 * @brief Class representation of a graph.
 *
 * @tparam  V  - vertex data type.
 * @tparam  E  - edge data type.
 * @tparam  Id  -  vertex identifier data type (int if omitted).
 */
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
        using std::map<Id, Eopt>::contains;
        using std::map<Id, Eopt>::size;

        bool operator==(const Vertex &) const;
    };

    std::map<Id, Vertex> graph_;

    bool is_edge_equal_(Id, Id, Id, Id) const;

    template <typename U>
    static std::string fmt_(const std::string &, U &&);

    template <typename U, typename... Args>
    static std::string fmt_(const std::string &, U &&, Args &&...);

    template <typename... Args>
    static std::string format_(const std::string &, Args &&...);

    void init_(const Graph &);
    void init_(const std::initializer_list<Id> &);

public:
    Graph();
    Graph(const Graph &);
    Graph(Graph &&);
    Graph(const std::initializer_list<Id> &);
    virtual ~Graph();

    Graph &operator=(const Graph &);
    Graph &operator=(Graph &&);
    Graph &operator=(const std::initializer_list<Id> &);

    void make_vertex(Id, Vopt = Nil);  // * TESTED
    void make_dir(Id, Id, Eopt = Nil); // * TESTED
    void make_bidir(Id, Id, Eopt = Nil);

    void join_dir(Id, Id, Eopt = Nil);   // * TESTED
    void join_bidir(Id, Id, Eopt = Nil); // * EDGE CASES TESTED

    void update_vertex(Id, Vopt = Nil); // * TESTED
    void update_dir(Id, Id, Eopt = Nil);
    void update_bidir(Id, Id, Eopt = Nil);

    void erase_vertex(Id); // * TESTED
    void erase_dir(Id, Id);
    void erase_bidir(Id, Id);

    bool is_vertex(Id) const; // * TESTED
    bool is_dir(Id, Id) const;
    bool is_bidir(Id, Id) const;

    bool has_data(Id, Id) const;
    bool has_data(Id) const; // * TESTED

    E data(Id, Id) const;
    V data(Id) const; // * TESTED

    std::string to_string() const;
    // friend std::ofstream &operator<<(std::ofstream &, Graph<V, E>);

    std::size_t size() const;
    int edge_count() const;
    bool empty() const;

    explicit operator bool() const;

    bool operator==(const Graph &) const;
    bool operator!=(const Graph &) const;
};

#include "Graph.cpp"
#endif