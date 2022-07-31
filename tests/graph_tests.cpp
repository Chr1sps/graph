#include "Graph.hpp"
#include <catch2/catch_all.hpp>

TEST_CASE("Graph initialization.", "[INIT]")
{
    Graph<>();
    Graph<>(5);
    Graph<int, int>(5);
    Graph<std::string, std::string>(5);
    Graph<int>(5);
    Graph<std::nullptr_t, int>(5);
}
TEST_CASE("Adding and removing edges", "[EDGE]")
{
    SECTION("No data")
    {
        Graph<> graph = Graph<>(5);
        graph.edge_dir(0, 2);
        graph.edge_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n4:\n");
    }
    SECTION("Edge and vertex data")
    {
    }
}