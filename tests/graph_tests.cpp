#include "Graph.hpp"

#include <catch2/catch_test_macros.hpp>

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
        REQUIRE(graph_int.size() == 0);
        REQUIRE(graph_int.edge_count() == 0);
        REQUIRE(graph_int.empty());
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
    Graph<int, int> graph_int = {1, 2, 3, 4};
    Graph<string, string, string> graph_str = {"one", "two", "three", "four"};
    string result_str;
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
        graph_int.make_bidir(1, 3);
        graph_int.make_bidir(4, 2, 7);

        result_str = "1: 3\n2: (4: 7)\n3: 1\n4: (2: 7)\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_bidir(1, 3));
        REQUIRE(graph_int.is_bidir(3, 1));
        REQUIRE(graph_int.is_bidir(2, 4));
        REQUIRE(graph_int.is_bidir(4, 2));

        REQUIRE_FALSE(graph_int.has_data(1, 3));
        REQUIRE_FALSE(graph_int.has_data(3, 1));
        REQUIRE(graph_int.has_data(2, 4));
        REQUIRE(graph_int.has_data(4, 2));
        REQUIRE(graph_int.data(2, 4) == 7);
        REQUIRE(graph_int.data(4, 2) == 7);

        graph_str.make_bidir("one", "three");
        graph_str.make_bidir("four", "two", "seven");

        result_str = "four: (two: seven)\n\
one: three\n\
three: one\n\
two: (four: seven)\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_bidir("one", "three"));
        REQUIRE(graph_str.is_bidir("three", "one"));
        REQUIRE(graph_str.is_bidir("two", "four"));
        REQUIRE(graph_str.is_bidir("four", "two"));

        REQUIRE_FALSE(graph_str.has_data("one", "three"));
        REQUIRE_FALSE(graph_str.has_data("three", "one"));
        REQUIRE(graph_str.has_data("two", "four"));
        REQUIRE(graph_str.has_data("four", "two"));
        REQUIRE(graph_str.data("two", "four") == "seven");
        REQUIRE(graph_str.data("four", "two") == "seven");
    }
    SECTION("make_bidir non-existent")
    {
        graph_int.make_bidir(5, 7);
        graph_int.make_bidir(8, 6, 6);

        result_str = "1:\n2:\n3:\n4:\n5: 7\n6: (8: 6)\n7: 5\n8: (6: 6)\n";

        REQUIRE(graph_int.to_string() == result_str);

        REQUIRE(graph_int.is_bidir(5, 7));
        REQUIRE(graph_int.is_bidir(8, 6));

        REQUIRE_FALSE(graph_int.has_data(5, 7));
        REQUIRE_FALSE(graph_int.has_data(7, 5));
        REQUIRE(graph_int.has_data(8, 6));
        REQUIRE(graph_int.has_data(6, 8));
        REQUIRE(graph_int.data(8, 6) == 6);
        REQUIRE(graph_int.data(6, 8) == 6);

        graph_str.make_bidir("seven", "six");
        graph_str.make_bidir("nine", "eight", "six");

        result_str = "eight: (nine: six)\n\
four:\n\
nine: (eight: six)\n\
one:\n\
seven: six\n\
six: seven\n\
three:\n\
two:\n";

        REQUIRE(graph_str.to_string() == result_str);

        REQUIRE(graph_str.is_bidir("seven", "six"));
        REQUIRE(graph_str.is_bidir("nine", "eight"));

        REQUIRE_FALSE(graph_str.has_data("seven", "six"));
        REQUIRE_FALSE(graph_str.has_data("six", "seven"));
        REQUIRE(graph_str.has_data("nine", "eight"));
        REQUIRE(graph_str.has_data("eight", "nine"));

        REQUIRE(graph_str.data("nine", "eight") == "six");
        REQUIRE(graph_str.data("eight", "nine") == "six");
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
    SECTION("join_bidir side effects")
    {
        IGNORE(graph_int.join_bidir(5, 7));
        IGNORE(graph_int.join_bidir(8, 6, 6));
        IGNORE(graph_int.join_bidir(4, 5));
        IGNORE(graph_int.join_bidir(5, 4));

        result_str = "1:\n2:\n3:\n4:\n";

        REQUIRE(graph_int.to_string() == result_str);
        REQUIRE_FALSE(graph_int.is_vertex(5));
        REQUIRE_FALSE(graph_int.is_vertex(6));
        REQUIRE_FALSE(graph_int.is_vertex(7));
        REQUIRE_FALSE(graph_int.is_vertex(8));

        IGNORE(graph_str.join_bidir("seven", "six"));
        IGNORE(graph_str.join_bidir("nine", "eight", "six"));
        IGNORE(graph_str.join_bidir("four", "five"));
        IGNORE(graph_str.join_bidir("five", "four"));
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

TEST_CASE("Comparing graphs - true", "[COMP][TRUE]")
{
    Graph<int, int> first, second;
    SECTION("empty")
    {
        REQUIRE(first == second);
    }
    SECTION("vertices only")
    {
        first = {1, 2, 3, 4};
        second = {1, 2, 3, 4};
        REQUIRE(first == second);
    }
    SECTION("copying constructor")
    {
        first = {1, 2, 3, 4};
        first.join_dir(1, 3, 4);
        first.join_dir(2, 1);
        Graph<int, int> other(first);
        REQUIRE(first == other);
    }
    SECTION("copying assignment")
    {
        first = {1, 2, 3, 4};
        first.join_dir(1, 3, 4);
        first.join_dir(2, 1);
        second = first;
        REQUIRE(first == second);
    }
}

TEST_CASE("Comparing graphs - false", "[COMP][FALSE]")
{
    Graph<int, int> first, second;
    SECTION("vertices only")
    {
        first = {1, 2, 3};
        second = {2, 3, 4};
        REQUIRE(first != second);
    }
    SECTION("edges only - no data")
    {
        first = second = {1, 2, 3, 4};
        first.join_dir(2, 3);
        second.join_dir(3, 2);
        REQUIRE(first != second);
    }
}

TEST_CASE("Move semantics")
{
    Graph<int, int> first = {1, 2, 3, 4};

    first.join_dir(1, 3);
    first.join_bidir(1, 2, 1);
    first.join_dir(2, 4, 3);

    string result_str = "1: (2: 1) 3\n\
2: (1: 1) (4: 3)\n\
3:\n\
4:\n";

    SECTION("move constructor")
    {
        auto other(std::move(first));

        REQUIRE(other.size() == 4);
        REQUIRE(other.edge_count() == 4);

        REQUIRE(other.is_vertex(1));
        REQUIRE(other.is_vertex(2));
        REQUIRE(other.is_vertex(3));
        REQUIRE(other.is_vertex(4));

        REQUIRE(other.is_dir(1, 3));
        REQUIRE(other.is_dir(2, 4));
        REQUIRE(other.is_bidir(1, 2));

        REQUIRE_FALSE(other.has_data(1, 3));
        REQUIRE(other.has_data(1, 2));
        REQUIRE(other.has_data(2, 1));
        REQUIRE(other.has_data(2, 4));

        REQUIRE(other.data(1, 2) == 1);
        REQUIRE(other.data(2, 1) == 1);
        REQUIRE(other.data(2, 4) == 3);

        REQUIRE(other.to_string() == result_str);
    }
    SECTION("move assignment")
    {
        Graph<int, int> other;
        other = std::move(first);

        REQUIRE(other.size() == 4);
        REQUIRE(other.edge_count() == 4);

        REQUIRE(other.is_vertex(1));
        REQUIRE(other.is_vertex(2));
        REQUIRE(other.is_vertex(3));
        REQUIRE(other.is_vertex(4));

        REQUIRE(other.is_dir(1, 3));
        REQUIRE(other.is_dir(2, 4));
        REQUIRE(other.is_bidir(1, 2));

        REQUIRE_FALSE(other.has_data(1, 3));
        REQUIRE(other.has_data(1, 2));
        REQUIRE(other.has_data(2, 1));
        REQUIRE(other.has_data(2, 4));

        REQUIRE(other.data(1, 2) == 1);
        REQUIRE(other.data(2, 1) == 1);
        REQUIRE(other.data(2, 4) == 3);

        REQUIRE(other.to_string() == result_str);
    }
}