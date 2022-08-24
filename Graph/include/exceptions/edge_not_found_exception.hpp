#ifndef __EDGE_NOT_FOUND_EXCEPTION_HPP_CHRISPS__
#define __EDGE_NOT_FOUND_EXCEPTION_HPP_CHRISPS__
#include <stdexcept>
class EdgeNotFoundException : public std::out_of_range
{
public:
    EdgeNotFoundException();
    using std::out_of_range::out_of_range;
};
#endif // __EDGE_NOT_FOUND_EXCEPTION_HPP_CHRISPS__