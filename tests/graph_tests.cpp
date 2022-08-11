#include "Graph.hpp"
#include <catch2/catch_all.hpp>
#include <vector>

/**
 * @brief Used for testing side-effects of methods under exception-throwing
 * circumstances.
 *
 */
#define IGNORE(x)                   \
    try                             \
    {                               \
        x;                          \
    }                               \
    catch (const std::exception &e) \
    {                               \
    }

using namespace std;

vector<int> ids_int = {1, 2, 3, 4};
vector<std::string> ids_string = {"one", "two", "three", "four"};

TEST_CASE("Graph initialization.", "[INIT]")
{
    SECTION("normal")
    {
        Graph<int, int>();
        Graph<string, string>();
        Graph<string, string, string>();
        Graph<int, int, char>();
    }
    SECTION("unusual")
    {
        Graph<nullptr_t, nullptr_t, nullptr_t>();
    }
    SECTION("init method values")
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
    SECTION("make_vertex")
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
    SECTION("update_vertex")
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
    SECTION("update_vertex exception")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);

        REQUIRE_THROWS_AS(graph_int.update_vertex(3, 5), VertexNotFoundException);

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");

        REQUIRE_THROWS_AS(graph_str.update_vertex("seven", "five"), VertexNotFoundException);
    }
    SECTION("update_vertex side effects")
    {
        graph_int.make_vertex(2);
        graph_int.make_vertex(1, 3);

        result_str = "(1: 3):\n2:\n";

        IGNORE(graph_int.update_vertex(3, 5));
        REQUIRE(graph_int.to_string() == result_str);
        REQUIRE_FALSE(graph_int.is_vertex(3));

        graph_str.make_vertex("three", "two");
        graph_str.make_vertex("one");

        result_str = "one:\n(three: two):\n";

        IGNORE(graph_str.update_vertex("seven", "five"));

        REQUIRE(graph_str.to_string() == result_str);
        REQUIRE_FALSE(graph_str.is_vertex("seven"));
    }
    SECTION("erase_vertex")
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
    SECTION("make_dir")
    {
        graph_int.make_dir(1, 2);
        graph_int.make_dir(3, 4, 6);

        result_str = "1: 2\n2:\n3: (4: 6)\n4:\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_dir(1, 2));
        REQUIRE(graph_int.is_dir(3, 4));

        REQUIRE_FALSE(graph_int.has_data(1, 2));
        REQUIRE(graph_int.has_data(3, 4));
        REQUIRE(graph_int.data(3, 4) == 6);

        graph_str.make_dir("one", "two");
        graph_str.make_dir("three", "four", "six");

        result_str = "four:\n\
one: two\n\
three: (four: six)\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_dir("one", "two"));
        REQUIRE(graph_str.is_dir("three", "four"));

        REQUIRE_FALSE(graph_str.has_data("one", "two"));
        REQUIRE(graph_str.has_data("three", "four"));

        REQUIRE(graph_str.data("three", "four") == "six");
    }
    SECTION("make_dir non-existent")
    {
        graph_int.make_dir(5, 7);
        graph_int.make_dir(8, 6, 6);

        result_str = "1:\n2:\n3:\n4:\n5: 7\n6:\n7:\n8: (6: 6)\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_dir(5, 7));
        REQUIRE(graph_int.is_dir(8, 6));

        REQUIRE_FALSE(graph_int.has_data(5, 7));
        REQUIRE(graph_int.has_data(8, 6));
        REQUIRE(graph_int.data(8, 6) == 6);

        graph_str.make_dir("seven", "six");
        graph_str.make_dir("nine", "eight", "six");

        result_str = "eight:\n\
four:\n\
nine: (eight: six)\n\
one:\n\
seven: six\n\
six:\n\
three:\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_dir("seven", "six"));
        REQUIRE(graph_str.is_dir("nine", "eight"));

        REQUIRE_FALSE(graph_str.has_data("seven", "six"));
        REQUIRE(graph_str.has_data("nine", "eight"));

        REQUIRE(graph_str.data("nine", "eight") == "six");
    }
    SECTION("make_bidir")
    {
        graph_int.make_bidir(1, 5);
        graph_int.make_bidir(4, 2, 7);

        graph_str.make_bidir("one", "five");
        graph_str.make_bidir("four", "two", "seven");
    }
    SECTION("join_dir")
    {
        graph_int.join_dir(1, 2);
        graph_int.join_dir(3, 4, 6);

        result_str = "1: 2\n2:\n3: (4: 6)\n4:\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_dir(1, 2));
        REQUIRE(graph_int.is_dir(3, 4));

        REQUIRE_FALSE(graph_int.has_data(1, 2));
        REQUIRE(graph_int.has_data(3, 4));
        REQUIRE(graph_int.data(3, 4) == 6);

        graph_str.join_dir("one", "two");
        graph_str.join_dir("three", "four", "six");

        result_str = "four:\n\
one: two\n\
three: (four: six)\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_dir("one", "two"));
        REQUIRE(graph_str.is_dir("three", "four"));

        REQUIRE_FALSE(graph_str.has_data("one", "two"));
        REQUIRE(graph_str.has_data("three", "four"));

        REQUIRE(graph_str.data("three", "four") == "six");
    }
    SECTION("join_dir exception")
    {
        REQUIRE_THROWS_AS(graph_int.join_dir(5, 7), VertexNotFoundException);
        REQUIRE_THROWS_AS(graph_int.join_dir(8, 6, 6), VertexNotFoundException);
        REQUIRE_THROWS_AS(graph_str.join_dir("seven", "six"), VertexNotFoundException);
        REQUIRE_THROWS_AS(graph_str.join_dir("nine", "eight", "six"), VertexNotFoundException);
    }
    SECTION("join_dir side effects")
    {
        IGNORE(graph_int.join_dir(5, 7));
        IGNORE(graph_int.join_dir(8, 6, 6));
        result_str = "1:\n2:\n3:\n4:\n";

        REQUIRE(graph_int.to_string() == result_str);
        REQUIRE_FALSE(graph_int.is_vertex(5));
        REQUIRE_FALSE(graph_int.is_vertex(6));
        REQUIRE_FALSE(graph_int.is_vertex(7));
        REQUIRE_FALSE(graph_int.is_vertex(8));

        IGNORE(graph_str.join_dir("seven", "six"));
        IGNORE(graph_str.join_dir("nine", "eight", "six"));
        result_str = "four:\n\
one:\n\
three:\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);
        REQUIRE_FALSE(graph_str.is_vertex("six"));
        REQUIRE_FALSE(graph_str.is_vertex("seven"));
        REQUIRE_FALSE(graph_str.is_vertex("eight"));
        REQUIRE_FALSE(graph_str.is_vertex("nine"));
    }
}
