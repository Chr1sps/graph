#ifndef __VERTEX_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__
#define __VERTEX_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__
#include <optional>

class VertexWithoutValueException : public std::bad_optional_access
{
    const char *what_str_;

public:
    VertexWithoutValueException();
    VertexWithoutValueException(const char *);
    VertexWithoutValueException(const VertexWithoutValueException &) noexcept;
    const char *what() const noexcept;
};
#endif // __VERTEX_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__