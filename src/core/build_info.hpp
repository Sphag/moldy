#pragma once

#include <string_view>

namespace core {

std::string_view build_configuration() noexcept;
std::string_view compiler_id() noexcept;
long language_standard() noexcept;

} // namespace core
