#include "vertex_without_value_exception.hpp"

VertexWithoutValueException::VertexWithoutValueException(const char *what_arg)
    : what_str_(what_arg)
{
}

VertexWithoutValueException::VertexWithoutValueException(
    const VertexWithoutValueException &other) noexcept
    : what_str_(other.what_str_)
{
}

const char *VertexWithoutValueException::what() const noexcept
{
    return what_str_;
}