#pragma once

#define MOLDY_DETAIL_SOURCE_LOCATION()                                                                                 \
    ::core::LogSourceLocation                                                                                          \
    {                                                                                                                  \
        __FILE__, __LINE__, __func__                                                                                   \
    }

#define MOLDY_LOG_TRACE(category, ...)                                                                                 \
    ::core::log_formatted_message(::core::ELogLevel::Trace, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#define MOLDY_LOG_DEBUG(category, ...)                                                                                 \
    ::core::log_formatted_message(::core::ELogLevel::Debug, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#define MOLDY_LOG_INFO(category, ...)                                                                                  \
    ::core::log_formatted_message(::core::ELogLevel::Info, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#define MOLDY_LOG_WARNING(category, ...)                                                                               \
    ::core::log_formatted_message(::core::ELogLevel::Warning, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#define MOLDY_LOG_ERROR(category, ...)                                                                                 \
    ::core::log_formatted_message(::core::ELogLevel::Error, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#define MOLDY_LOG_CRITICAL(category, ...)                                                                              \
    ::core::log_formatted_message(::core::ELogLevel::Critical, (category), MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)

#if defined(MOLDY_ENABLE_ASSERTS)
#define MOLDY_ASSERT(expr)                                                                                             \
    ((expr) ? (void)0 : ::core::handle_assertion_failure(#expr, "", MOLDY_DETAIL_SOURCE_LOCATION()))

#define MOLDY_ASSERT_MSG(expr, ...)                                                                                    \
    ((expr) ? (void)0 : ::core::handle_formatted_assertion_failure(#expr, MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__))

#define MOLDY_ASSERT_FAIL(...)                                                                                         \
    ::core::handle_formatted_assertion_failure("false", MOLDY_DETAIL_SOURCE_LOCATION(), __VA_ARGS__)
#else
#define MOLDY_ASSERT(expr) ((void)0)
#define MOLDY_ASSERT_MSG(expr, ...) ((void)0)
#define MOLDY_ASSERT_FAIL(...) ((void)0)
#endif
