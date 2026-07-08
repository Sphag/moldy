#pragma once

#define MOLDY_DETAIL_SOURCE_LOCATION()                                                                                 \
    ::core::LogSourceLocation                                                                                          \
    {                                                                                                                  \
        __FILE__, __LINE__, __func__                                                                                   \
    }

#define MOLDY_LOG_TRACE(category, message)                                                                             \
    ::core::log_message(::core::ELogLevel::Trace, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#define MOLDY_LOG_DEBUG(category, message)                                                                             \
    ::core::log_message(::core::ELogLevel::Debug, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#define MOLDY_LOG_INFO(category, message)                                                                              \
    ::core::log_message(::core::ELogLevel::Info, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#define MOLDY_LOG_WARNING(category, message)                                                                           \
    ::core::log_message(::core::ELogLevel::Warning, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#define MOLDY_LOG_ERROR(category, message)                                                                             \
    ::core::log_message(::core::ELogLevel::Error, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#define MOLDY_LOG_CRITICAL(category, message)                                                                          \
    ::core::log_message(::core::ELogLevel::Critical, (category), (message), MOLDY_DETAIL_SOURCE_LOCATION())

#if defined(MOLDY_ENABLE_ASSERTS)
#define MOLDY_ASSERT(expr)                                                                                             \
    ((expr) ? (void)0 : ::core::handle_assertion_failure(#expr, "", MOLDY_DETAIL_SOURCE_LOCATION()))

#define MOLDY_ASSERT_MSG(expr, message)                                                                                \
    ((expr) ? (void)0 : ::core::handle_assertion_failure(#expr, (message), MOLDY_DETAIL_SOURCE_LOCATION()))

#define MOLDY_ASSERT_FAIL(message) ::core::handle_assertion_failure("false", (message), MOLDY_DETAIL_SOURCE_LOCATION())
#else
#define MOLDY_ASSERT(expr) ((void)0)
#define MOLDY_ASSERT_MSG(expr, message) ((void)0)
#define MOLDY_ASSERT_FAIL(message) ((void)0)
#endif
