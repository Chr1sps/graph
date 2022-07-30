#include <iostream>
#include "Graph_config.hpp"
#include "Graph.hpp"
using namespace std;
class Empty
{
};
int main()
{
    cout << "Main, version "
         << GRAPH_VERSION_MAJOR << '.' << GRAPH_VERSION_MINOR << endl;
    Graph<Empty, Empty> *g = new Graph<Empty, Empty>(5);
    g->edge_dir(0, 1);
    cout << g->to_string();
    delete g;
    return 0;
}