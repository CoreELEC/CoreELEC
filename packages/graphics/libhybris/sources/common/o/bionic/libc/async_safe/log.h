#ifndef thatasyncsafeheaderthingy_replacement__
#define thatasyncsafeheaderthingy_replacement__

enum {
  ANDROID_LOG_UNKNOWN = 0,
  ANDROID_LOG_DEFAULT,    /* only for SetMinPriority() */

  ANDROID_LOG_VERBOSE,
  ANDROID_LOG_DEBUG,
  ANDROID_LOG_INFO,
  ANDROID_LOG_WARN,
  ANDROID_LOG_ERROR,
  ANDROID_LOG_FATAL,

  ANDROID_LOG_SILENT,     /* only for SetMinPriority(); must be last */
};

#define async_safe_fatal(...) { fprintf(stderr, __VA_ARGS__); abort(); }

#define async_safe_format_fd(...) dprintf(__VA_ARGS)

// Don't really care about verbose/debug logs for now
#define async_safe_format_log_va_list(loglevel, category, format, ...) { if (loglevel > ANDROID_LOG_INFO) { vfprintf(stderr, format, __VA_ARGS__); } }

#endif

