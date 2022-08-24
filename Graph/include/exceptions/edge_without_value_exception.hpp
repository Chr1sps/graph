#ifndef __EDGE_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__
#define __EDGE_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__
#include <optional>

class EdgeWithoutValueException : public std::bad_optional_access
{
    const char *what_str_;

public:
    EdgeWithoutValueException();
    EdgeWithoutValueException(const char *);
    EdgeWithoutValueException(const EdgeWithoutValueException &) noexcept;
    const char *what() const noexcept;
};
#endif // __EDGE_WITHOUT_VALUE_EXCEPTION_HPP_CHRISPS__