#include "Graph.hpp"
#include <catch2/catch_all.hpp>
using namespace std;
TEST_CASE("Graph initialization.", "[INIT]")
{
    Graph<int, int>();
    Graph<string, string>();
    Graph<string, string, string>();
    Graph<int, int, char>();
}
TEST_CASE("Adding and removing edges", "[EDGE]")
{
    Graph<int, int> graph = Graph<int, int>();
    Graph<string, string, string> graph_str = Graph<string, string, string>();
    SECTION("No data")
    {
        graph.edge_dir(0, 2);
        graph.edge_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");

        graph_str.edge_dir("zero", "two");
        graph_str.edge_bidir("three", "one");
        REQUIRE(graph_str.to_string() == "one: three\n\
three: one\n\
two:\n\
zero: two\n");
    }
    SECTION("Edge and vertex data - int")
    {
        graph.edge_dir(0, 2, 2);
        graph.edge_bidir(3, 1, 3);
        REQUIRE(graph.to_string() ==
                "0: (2: 2)\n1: (3: 3)\n2:\n3: (1: 3)\n");

        graph_str.edge_dir("zero", "two", "two");
        graph_str.edge_bidir("three", "one", "three");
        string result_str = "one: (three: three)\n\
three: (one: three)\n\
two:\n\
zero: (two: two)\n";
        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Adding the same edge twice")
    {
        graph.edge_dir(0, 2);
        graph.edge_dir(0, 2);
        graph.edge_bidir(3, 1);
        graph.edge_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");

        graph_str.edge_dir("zero", "two");
        graph_str.edge_dir("zero", "two");
        graph_str.edge_bidir("three", "one");
        graph_str.edge_bidir("three", "one");
        REQUIRE(graph_str.to_string() == "one: three\n\
three: one\n\
two:\n\
zero: two\n");
    }
}
