#include "edge_not_found_exception.hpp"

EdgeNotFoundException::EdgeNotFoundException()
    : std::out_of_range::out_of_range("Edge was not found.")
{
}