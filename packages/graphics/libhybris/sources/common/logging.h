
/*
 * Copyright (c) 2013 Thomas Perl <m@thp.io>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef HYBRIS_LOGGING_H
#define HYBRIS_LOGGING_H

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

#define DEBUG

enum hybris_log_level {
    /* Most verbose logging level */
    HYBRIS_LOG_DEBUG = 0,

    /* Normal logging levels */
    HYBRIS_LOG_INFO,
    HYBRIS_LOG_WARN,
    HYBRIS_LOG_ERROR,

    /**
     * "Fake" level at which no messages are logged.
     * Can be used with hybris_set_log_level().
     **/
    HYBRIS_LOG_DISABLED,
};

enum hybris_log_format {
    HYBRIS_LOG_FORMAT_NORMAL,
    HYBRIS_LOG_FORMAT_SYSTRACE
};

/**
 * Returns nonzero if messages at level "level" should be logged.
 * Only used by the HYBRIS_LOG() macro, no need to call it manually.
 **/
int
hybris_should_log(enum hybris_log_level level);


/**
 * Sets the minimum log level that is logged, for example a minimum
 * log level of HYBRIS_LOG_DEBUG would print all log messages, a
 * minimum log level of HYBRIS_LOG_WARN would only print warnings and
 * errors. The default log level is HYBRIS_LOG_WARN.
 **/
void
hybris_set_log_level(enum hybris_log_level level);

void *
hybris_get_thread_id();

double
hybris_get_thread_time();

enum hybris_log_format hybris_logging_format();

int hybris_should_trace(const char *module, const char *tracepoint);

extern pthread_mutex_t hybris_logging_mutex;

#ifdef __cplusplus
}
#endif

extern FILE *hybris_logging_target;

#if defined(DEBUG)
#    define HYBRIS_LOG_(level, module, message, ...) do { \
          if (hybris_should_log(level)) { \
              pthread_mutex_lock(&hybris_logging_mutex); \
              if (hybris_logging_format() == HYBRIS_LOG_FORMAT_NORMAL) \
              { \
                fprintf(hybris_logging_target, "%s %s:%d (%s) %s: " message "\n", \
                       module, __FILE__, __LINE__, __PRETTY_FUNCTION__, \
                       #level + 11 /* + 11 = strip leading "HYBRIS_LOG_" */, \
                       ##__VA_ARGS__); \
                fflush(hybris_logging_target); \
              } else if (hybris_logging_format() == HYBRIS_LOG_FORMAT_SYSTRACE) { \
                fprintf(hybris_logging_target, "B|%i|%.9f|%s(%s) %s:%d (%s) " message "\n", \
                      getpid(), hybris_get_thread_time(), module, __PRETTY_FUNCTION__, __FILE__, __LINE__, \
                      #level + 11 /* + 11 = strip leading "HYBRIS_LOG_" */, \
                      ##__VA_ARGS__); \
                fflush(hybris_logging_target); \
                fprintf(hybris_logging_target, "E|%i|%.9f|%s(%s) %s:%d (%s) " message "\n", \
                      getpid(), hybris_get_thread_time(), module, __PRETTY_FUNCTION__, __FILE__, __LINE__, \
                      #level + 11 /* + 11 = strip leading "HYBRIS_LOG_" */, \
                      ##__VA_ARGS__); \
                fflush(hybris_logging_target); \
             } \
             pthread_mutex_unlock(&hybris_logging_mutex); \
          } \
     } while(0)

#define HYBRIS_TRACE_RECORD(module, what, tracepoint, message, ...) do { \
          if (hybris_should_trace(module, tracepoint)) { \
              pthread_mutex_lock(&hybris_logging_mutex); \
              if (hybris_logging_format() == HYBRIS_LOG_FORMAT_NORMAL) \
              { \
                fprintf(hybris_logging_target, "PID: %i TTIME: %.9f Tracepoint-%c/%s::%s" message "\n", \
                      getpid(), hybris_get_thread_time(), what, tracepoint, module, \
                      ##__VA_ARGS__); \
                fflush(hybris_logging_target); \
              } else if (hybris_logging_format() == HYBRIS_LOG_FORMAT_SYSTRACE) { \
                if (what == 'B') \
                   fprintf(hybris_logging_target, "B|%i|%.9f|%s::%s" message "", \
                      getpid(), hybris_get_thread_time(), tracepoint, module, ##__VA_ARGS__); \
                else if (what == 'E') \
                   fprintf(hybris_logging_target, "E"); \
                else \
                   fprintf(hybris_logging_target, "C|%i|%.9f|%s::%s-%i|" message "", \
                      getpid(), hybris_get_thread_time(), tracepoint, module, getpid(), ##__VA_ARGS__); \
                fflush(hybris_logging_target); \
              } \
             pthread_mutex_unlock(&hybris_logging_mutex); \
          } \
      } while(0)
#    define HYBRIS_TRACE_BEGIN(module, tracepoint, message, ...) HYBRIS_TRACE_RECORD(module, 'B', tracepoint, message, ##__VA_ARGS__)
#    define HYBRIS_TRACE_END(module, tracepoint, message, ...) HYBRIS_TRACE_RECORD(module, 'E', tracepoint, message, ##__VA_ARGS__)
#    define HYBRIS_TRACE_COUNTER(module, tracepoint, message, ...) HYBRIS_TRACE_RECORD(module, 'C', tracepoint, message, ##__VA_ARGS__)
#else
#    define HYBRIS_LOG_(level, module, message, ...) while (0) {}
#    define HYBRIS_TRACE_BEGIN(module, tracepoint, message, ...) while (0) {}
#    define HYBRIS_TRACE_END(module, tracepoint, message, ...) while (0) {}
#    define HYBRIS_TRACE_COUNTER(module, tracepoint, message, ...) while (0) {}
#endif


/* Generic logging functions taking the module name and a message */
#define HYBRIS_DEBUG_LOG(module, message, ...) HYBRIS_LOG_(HYBRIS_LOG_DEBUG, #module, message, ##__VA_ARGS__)
#define HYBRIS_WARN_LOG(module, message, ...) HYBRIS_LOG_(HYBRIS_LOG_WARN, #module, message, ##__VA_ARGS__)
#define HYBRIS_INFO_LOG(module, message, ...) HYBRIS_LOG_(HYBRIS_LOG_INFO, #module, message, ##__VA_ARGS__)
#define HYBRIS_ERROR_LOG(module, message, ...) HYBRIS_LOG_(HYBRIS_LOG_ERROR, #module, message, ##__VA_ARGS__)


/* Module-specific logging functions can be defined like this */
#define HYBRIS_DEBUG(message, ...) HYBRIS_DEBUG_LOG(HYBRIS, message, ##__VA_ARGS__)
#define HYBRIS_WARN(message, ...) HYBRIS_WARN_LOG(HYBRIS, message, ##__VA_ARGS__)
#define HYBRIS_INFO(message, ...) HYBRIS_INFO_LOG(HYBRIS, message, ##__VA_ARGS__)
#define HYBRIS_ERROR(message, ...) HYBRIS_ERROR_LOG(HYBRIS, message, ##__VA_ARGS__)

/* for compatibility reasons */
#define TRACE(message, ...) HYBRIS_DEBUG_LOG(EGL, message, ##__VA_ARGS__)

#endif /* HYBRIS_LOGGING_H */
// vim: noai:ts=4:sw=4:ss=4:expandtab
