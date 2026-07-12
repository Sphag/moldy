#pragma once

#define MOLDY_DETAIL_SOURCE_LOCATION()                                                                                 \
    ::core::LogSourceLocation                                                                                          \
    {                                                                                                                  \
        __FILE__, __LINE__, __func__                                                                                   \
    }

#define MOLDY_LOG_TRACE(category, format, ...)                                                                         \
    ::core::log_formatted_message(::core::ELogLevel::Trace, (category), MOLDY_DETAIL_SOURCE_LOCATION(),                \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#define MOLDY_LOG_DEBUG(category, format, ...)                                                                         \
    ::core::log_formatted_message(::core::ELogLevel::Debug, (category), MOLDY_DETAIL_SOURCE_LOCATION(),                \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#define MOLDY_LOG_INFO(category, format, ...)                                                                          \
    ::core::log_formatted_message(::core::ELogLevel::Info, (category), MOLDY_DETAIL_SOURCE_LOCATION(),                 \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#define MOLDY_LOG_WARNING(category, format, ...)                                                                       \
    ::core::log_formatted_message(::core::ELogLevel::Warning, (category), MOLDY_DETAIL_SOURCE_LOCATION(),              \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#define MOLDY_LOG_ERROR(category, format, ...)                                                                         \
    ::core::log_formatted_message(::core::ELogLevel::Error, (category), MOLDY_DETAIL_SOURCE_LOCATION(),                \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#define MOLDY_LOG_CRITICAL(category, format, ...)                                                                      \
    ::core::log_formatted_message(::core::ELogLevel::Critical, (category), MOLDY_DETAIL_SOURCE_LOCATION(),             \
                                  (format)__VA_OPT__(, ) __VA_ARGS__)

#if defined(MOLDY_ENABLE_ASSERTS)
#define MOLDY_ASSERT(expr)                                                                                             \
    ((expr) ? (void)0 : ::core::handle_assertion_failure(#expr, "", MOLDY_DETAIL_SOURCE_LOCATION()))

#define MOLDY_ASSERT_MSG(expr, format, ...)                                                                            \
    ((expr) ? (void)0                                                                                                  \
            : ::core::handle_formatted_assertion_failure(#expr, MOLDY_DETAIL_SOURCE_LOCATION(),                        \
                                                         (format)__VA_OPT__(, ) __VA_ARGS__))

#define MOLDY_ASSERT_FAIL(format, ...)                                                                                 \
    ::core::handle_formatted_assertion_failure("false", MOLDY_DETAIL_SOURCE_LOCATION(),                                \
                                               (format)__VA_OPT__(, ) __VA_ARGS__)
#else
#define MOLDY_ASSERT(expr) ((void)0)
#define MOLDY_ASSERT_MSG(expr, format, ...) ((void)0)
#define MOLDY_ASSERT_FAIL(format, ...) ((void)0)
#endif
