#include "edge_without_value_exception.hpp"

EdgeWithoutValueException::EdgeWithoutValueException()
    : what_str_("Edge has no value.")
{
}

EdgeWithoutValueException::EdgeWithoutValueException(const char *what_arg)
    : what_str_(what_arg)
{
}

EdgeWithoutValueException::EdgeWithoutValueException(
    const EdgeWithoutValueException &other) noexcept
    : what_str_(other.what_str_)
{
}
const char *EdgeWithoutValueException::what() const noexcept
{
    return what_str_;
}