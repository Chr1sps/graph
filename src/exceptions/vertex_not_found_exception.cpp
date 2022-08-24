#include "vertex_not_found_exception.hpp"

VertexNotFoundException::VertexNotFoundException()
    : std::out_of_range::out_of_range("Vertex was not found.")
{
}