#include "Graph.hpp"
#include <catch2/catch_all.hpp>
#include <vector>
using namespace std;

vector<int> ids_int = {1, 2, 3, 4};
vector<std::string> ids_string = {"one", "two", "three", "four"};

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

TEST_CASE("Vertex manipulation", "[VERTEX]")
{
    Graph<int, int> graph_int = Graph<int, int>();
    Graph<string, string, string> graph_str = Graph<string, string, string>();
    string result_str;
    SECTION("Adding vertices")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);
        result_str = "(1: 3):\n2:\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_vertex(1));
        REQUIRE(graph_int.is_vertex(2));

        REQUIRE(graph_int.has_data(1));
        REQUIRE_FALSE(graph_int.has_data(2));

        REQUIRE(graph_int.data(1) == 3);

        REQUIRE(graph_int);

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");
        result_str = "one:\n(three: two):\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_vertex("one"));
        REQUIRE(graph_str.is_vertex("three"));

        REQUIRE(graph_str.has_data("three"));
        REQUIRE_FALSE(graph_str.has_data("one"));

        REQUIRE(graph_str.data("three") == "two");

        REQUIRE(graph_str);
    }
    SECTION("Updating vertices - valid")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);
        graph_int.update_vertex(2, 4);

        result_str = "(1: 3):\n(2: 4):\n";

        REQUIRE(graph_int.has_data(2));
        REQUIRE(graph_int.data(2) == 4);

        REQUIRE(graph_int.to_string() == result_str);

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");
        graph_str.update_vertex("three");

        result_str = "one:\nthree:\n";

        REQUIRE_FALSE(graph_str.has_data("three"));

        REQUIRE(graph_str.to_string() == result_str);
    }
    SECTION("Updating vertices - exception")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);

        REQUIRE_THROWS_AS(graph_int.update_vertex(3, 5), VertexNotFoundException);

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");

        REQUIRE_THROWS_AS(graph_str.update_vertex("seven", "five"), VertexNotFoundException);
    }
    SECTION("Updating vertices - exception, side effects")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);

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

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");

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
    SECTION("Erasing vertices")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);
        graph_int.erase_vertex(1);
        result_str = "2:\n";
        REQUIRE_FALSE(graph_int.is_vertex(1));
        REQUIRE(graph_int.to_string() == result_str);

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");
        graph_str.erase_vertex("one");
        result_str = "(three: two):\n";
        REQUIRE_FALSE(graph_str.is_vertex("one"));
        REQUIRE(graph_str.to_string() == result_str);
    }
}
TEST_CASE("Edge manipulation", "[EDGE]")
{
    Graph<int, int> graph_int = Graph<int, int>();
    Graph<string, string, string> graph_str = Graph<string, string, string>();
    string result_str;
    for (const int &id : ids_int)
    {
        graph_int.make_vertex(id);
    }
    for (const std::string &id : ids_string)
    {
        graph_str.make_vertex(id);
    }
    SECTION("Making dir edges")
    {
        graph_int.make_dir(1, 2);
        graph_int.make_dir(3, 5, 6);

        result_str = "1: 2\n2:\n3: (5: 6)\n4:\n5:\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_dir(1, 2));
        REQUIRE(graph_int.is_dir(3, 5));

        graph_str.make_dir("one", "two");
        graph_str.make_dir("three", "five", "six");

        result_str = "five:\n\
four:\n\
one: two\n\
three: (five: six)\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_dir("one", "two"));
        REQUIRE(graph_str.is_dir("three", "five"));

        REQUIRE_FALSE(graph_str.has_data("one", "two"));
        REQUIRE(graph_str.has_data("three", "five"));

        REQUIRE(graph_str.data("three", "five") == "six");
    }
    SECTION("Making dir edges - no vertices")
    {
    }
    SECTION("Making bidir edges")
    {
        graph_int.make_bidir(1, 5);
        graph_int.make_bidir(4, 2, 7);

        graph_str.make_bidir("one", "five");
        graph_str.make_bidir("four", "two", "seven");
    }
}
