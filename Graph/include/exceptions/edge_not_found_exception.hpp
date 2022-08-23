#ifndef __EDGE_NOT_FOUND_EXCEPTION_CHRISPS__
#define __EDGE_NOT_FOUND_EXCEPTION_CHRISPS__
#include <stdexcept>
class EdgeNotFoundException : public std::out_of_range
{
public:
    using std::out_of_range::out_of_range;
};
#endif