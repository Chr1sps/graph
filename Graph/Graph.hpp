#include <iostream>

template <class V, class E>
class Graph
{
public:
    Graph()
    {
        std::cout << "Graph constructor." << std::endl;
    }
    ~Graph()
    {
        std::cout << "Graph destructor." << std::endl;
    }
};
