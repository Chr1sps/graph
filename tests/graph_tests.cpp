#include "Graph.hpp"
#include <catch2/catch_all.hpp>
using namespace std;

TEST_CASE("Graph initialization.", "[INIT]")
{
    SECTION("Normal types")
    {
        Graph<int, int>();
        Graph<string, string>();
        Graph<string, string, string>();
        Graph<int, int, char>();
    }
    SECTION("Unusual types")
    {
        Graph<nullptr_t, nullptr_t, nullptr_t>();
    }
    SECTION("Method return values")
    {
        Graph<int, int> graph_int = Graph<int, int>();
        REQUIRE_FALSE(graph_int);
        REQUIRE_FALSE(graph_int.is_vertex(1));
        REQUIRE_FALSE(graph_int.is_vertex(3));
        REQUIRE(graph_int.to_string() == "");
    }
}

TEST_CASE("Adding, updating and removing vertices", "[VERTEX]")
{
    Graph<int, int> graph_int = Graph<int, int>();
    Graph<string, string, string> graph_str = Graph<string, string, string>();
    string result_str;
    SECTION("Adding vertices")
    {
        graph_int.add_vertex(2);
        graph_int.add_vertex(1, 3);
        result_str = "(1: 3):\n2:\n";
        REQUIRE(graph_int.to_string() == result_str);
        REQUIRE(graph_int.is_vertex(1));
        REQUIRE(graph_int.is_vertex(2));
        REQUIRE(graph_int.vertex_has_data(1));
        REQUIRE_FALSE(graph_int.vertex_has_data(2));
        REQUIRE(graph_int.get_vertex_data(1) == 3);
        REQUIRE(graph_int);

        graph_str.add_vertex("three", "two");
        graph_str.add_vertex("one");
        result_str = "one:\n(three: two):\n";
        REQUIRE(graph_str.to_string() == result_str);
        REQUIRE(graph_str.is_vertex("one"));
        REQUIRE(graph_str.is_vertex("three"));
        REQUIRE(graph_str.vertex_has_data("three"));
        REQUIRE_FALSE(graph_str.vertex_has_data("one"));
        REQUIRE(graph_str.get_vertex_data("three") == "two");
        REQUIRE(graph_str);
    }
    SECTION("Updating vertices - valid")
    {
        graph_int.add_vertex(2);
        graph_int.add_vertex(1, 3);
        graph_int.update_vertex(2, 4);
        result_str = "(1: 3):\n(2: 4):\n";
        REQUIRE(graph_int.vertex_has_data(2));
        REQUIRE(graph_int.get_vertex_data(2) == 4);
        REQUIRE(graph_int.to_string() == result_str);

        graph_str.add_vertex("three", "two");
        graph_str.add_vertex("one");
        graph_str.update_vertex("three");
        result_str = "one:\nthree:\n";
        REQUIRE_FALSE(graph_str.vertex_has_data("three"));
        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Updating vertices - exception")
    {
        graph_int.add_vertex(2);
        graph_int.add_vertex(1, 3);
        REQUIRE_THROWS_AS(graph_int.update_vertex(3, 5), VertexNotFoundException);

        graph_str.add_vertex("three", "two");
        graph_str.add_vertex("one");
        REQUIRE_THROWS_AS(graph_str.update_vertex("seven", "five"), VertexNotFoundException);
    }
    SECTION("Updating vertices - exception, side effects")
    {
        graph_int.add_vertex(2);
        graph_int.add_vertex(1, 3);
        result_str = "(1: 3):\n2:\n";
        try
        {
            graph_int.update_vertex(3, 5);
        }
        catch (const VertexNotFoundException &e)
        {
            REQUIRE(graph_int.to_string() == result_str);
            REQUIRE_FALSE(graph_int.is_vertex(3));
        }

        graph_str.add_vertex("three", "two");
        graph_str.add_vertex("one");
        result_str = "one:\n(three: two):\n";
        try
        {
            graph_str.update_vertex("seven", "five");
        }
        catch (const std::exception &e)
        {
            REQUIRE(graph_str.to_string() == result_str);
            REQUIRE_FALSE(graph_str.is_vertex("seven"));
        }
    }
}
TEST_CASE("Adding and removing edges", "[EDGE]")
{
    Graph<int, int> graph = Graph<int, int>();
    Graph<string, string, string> graph_str = Graph<string, string, string>();
    string result_str;
    SECTION("No data")
    {
        graph.make_dir(0, 2);
        graph.make_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");

        graph_str.make_dir("zero", "two");
        graph_str.make_bidir("three", "one");
        result_str = "one: three\n\
three: one\n\
two:\n\
zero: two\n";

        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Edge and vertex data - int")
    {
        graph.make_dir(0, 2, 2);
        graph.make_bidir(3, 1, 3);
        REQUIRE(graph.to_string() ==
                "0: (2: 2)\n1: (3: 3)\n2:\n3: (1: 3)\n");

        graph_str.make_dir("zero", "two", "two");
        graph_str.make_bidir("three", "one", "three");
        result_str = "one: (three: three)\n\
three: (one: three)\n\
two:\n\
zero: (two: two)\n";

        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Adding the same edge twice")
    {
        graph.make_dir(0, 2);
        graph.make_dir(0, 2);
        graph.make_bidir(3, 1);
        graph.make_bidir(3, 1);
        REQUIRE(graph.to_string() == "0: 2\n1: 3\n2:\n3: 1\n");

        graph_str.make_dir("zero", "two");
        graph_str.make_dir("zero", "two");
        graph_str.make_bidir("three", "one");
        graph_str.make_bidir("three", "one");
        result_str = "one: three\n\
three: one\n\
two:\n\
zero: two\n";

        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Vertex removal")
    {
        graph.make_dir(0, 2, 2);
        graph.make_bidir(3, 1, 3);
        graph.erase_vertex(1);
        REQUIRE(graph.to_string() ==
                "0: (2: 2)\n2:\n3:\n");
    }
}
