#include <optional>

class VertexWithoutValueException : public std::bad_optional_access
{
    const char *what_str_;

public:
    VertexWithoutValueException() = default;
    VertexWithoutValueException(const char *);
    VertexWithoutValueException(const VertexWithoutValueException &) noexcept;
    const char *what() const noexcept;
};