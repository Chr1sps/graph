#include "vertex_without_value_exception.hpp"

VertexWithoutValueException::VertexWithoutValueException(const char *what_arg)
    : what_str_(what_arg)
{
}

VertexWithoutValueException::VertexWithoutValueException(const VertexWithoutValueException &other)
    : what_str_(other.what_str_)
{
}