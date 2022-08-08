#include "edge_without_value_exception.hpp"

EdgeWithoutValueException::EdgeWithoutValueException(const char *what_arg)
    : what_str_(what_arg)
{
}

EdgeWithoutValueException::EdgeWithoutValueException(const EdgeWithoutValueException &other)
    : what_str_(other.what_str_)
{
}