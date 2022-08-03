#include "Graph.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("Graph initialization.", "[INIT]")
{
    Graph<int, int>();
    Graph<std::string, std::string>();
}
TEST_CASE("Adding and removing edges", "[EDGE]")
{
    Graph<int, int> graph = Graph<int, int>();
    SECTION("No data")
    {
        graph.edge_dir(0, 2);
        graph.edge_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");
    }
    SECTION("Edge and vertex data - int")
    {
        graph.edge_dir(0, 2, 2);
        graph.edge_bidir(3, 1, 3);
        REQUIRE(graph.to_string() ==
                "0: (2: 2)\n1: (3: 3)\n2:\n3: (1: 3)\n");
    }
    SECTION("Adding the same edge twice")
    {
        graph.edge_dir(0, 2);
        graph.edge_dir(0, 2);
        graph.edge_bidir(3, 1);
        graph.edge_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");
    }
}
