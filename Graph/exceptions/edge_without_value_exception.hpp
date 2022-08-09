#include <optional>

class EdgeWithoutValueException : public std::bad_optional_access
{
    const char *what_str_;

public:
    EdgeWithoutValueException() = default;
    EdgeWithoutValueException(const char *);
    EdgeWithoutValueException(const EdgeWithoutValueException &) noexcept;
    const char *what() const noexcept;
};