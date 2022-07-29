#include <iostream>
#include "Graph_config.hpp"
#include "Graph.hpp"
using namespace std;
int main()
{
    cout << "Main, version "
         << GRAPH_VERSION_MAJOR << '.' << GRAPH_VERSION_MINOR << endl;
    Graph<int, int> *g = new Graph<int, int>;
    delete g;
    return 0;
}