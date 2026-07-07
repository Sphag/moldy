module;

#include <string>
#include <string_view>
#include <utility>

module moldy.core;

namespace core
{

Status::Status(EErrorCode code, std::string message) : code_(code), message_(std::move(message)) {}

bool Status::ok() const noexcept
{
    return code_ == EErrorCode::Ok;
}

EErrorCode Status::code() const noexcept
{
    return code_;
}

std::string_view Status::message() const noexcept
{
    return message_;
}

Status Status::success()
{
    return Status(EErrorCode::Ok, {});
}

Status Status::error(EErrorCode code, std::string message)
{
    if (code == EErrorCode::Ok)
    {
        code = EErrorCode::Internal;
    }

    return Status(code, std::move(message));
}

} // namespace core
