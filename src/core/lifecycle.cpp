module;

#include <string>

module moldy.core;

namespace core
{

EApplicationState ApplicationLifecycle::state() const noexcept
{
    return state_;
}

Status ApplicationLifecycle::start()
{
    if (state_ != EApplicationState::Created)
    {
        return Status::error(EErrorCode::InvalidArgument, "ApplicationLifecycle can only start from Created state.");
    }

    state_ = EApplicationState::Running;
    return Status::success();
}

Status ApplicationLifecycle::request_stop()
{
    if (state_ != EApplicationState::Running)
    {
        return Status::error(EErrorCode::InvalidArgument,
                             "ApplicationLifecycle can only request stop from Running state.");
    }

    state_ = EApplicationState::StopRequested;
    return Status::success();
}

Status ApplicationLifecycle::stop()
{
    if (state_ != EApplicationState::Running && state_ != EApplicationState::StopRequested)
    {
        return Status::error(EErrorCode::InvalidArgument,
                             "ApplicationLifecycle can only stop from Running or StopRequested state.");
    }

    state_ = EApplicationState::Stopped;
    return Status::success();
}

} // namespace core
