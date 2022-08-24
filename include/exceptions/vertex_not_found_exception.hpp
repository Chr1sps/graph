#ifndef __VERTEX_NOT_FOUND_EXCEPTION_HPP_CHRISPS__
#define __VERTEX_NOT_FOUND_EXCEPTION_HPP_CHRISPS__
#include <stdexcept>
class VertexNotFoundException : public std::out_of_range
{
public:
    VertexNotFoundException();
    using std::out_of_range::out_of_range;
};
#endif // __VERTEX_NOT_FOUND_EXCEPTION_HPP_CHRISPS__