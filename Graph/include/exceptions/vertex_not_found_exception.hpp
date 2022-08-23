#ifndef __VERTEX_NOT_FOUND_EXCEPTION_CHRISPS__
#define __VERTEX_NOT_FOUND_EXCEPTION_CHRISPS__
#include <stdexcept>
class VertexNotFoundException : public std::out_of_range
{
public:
    using std::out_of_range::out_of_range;
};
#endif