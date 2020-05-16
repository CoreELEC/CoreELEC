
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


#include "logging.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

FILE *hybris_logging_target = NULL;

pthread_mutex_t hybris_logging_mutex;

static enum hybris_log_level
hybris_minimum_log_level = HYBRIS_LOG_WARN;

static enum hybris_log_format _hybris_logging_format = HYBRIS_LOG_FORMAT_NORMAL;

static int _hybris_should_trace = 0;

static int
hybris_logging_initialized = 0;

static void
hybris_logging_initialize()
{
    const char *env = getenv("HYBRIS_LOGGING_LEVEL");

    if (env == NULL) {
        /* Nothing to do - use default level */
    } else if (strcmp(env, "debug") == 0) {
        hybris_minimum_log_level = HYBRIS_LOG_DEBUG;
    } else if (strcmp(env, "info") == 0) {
        hybris_minimum_log_level = HYBRIS_LOG_INFO;
    } else if (strcmp(env, "warn") == 0) {
        hybris_minimum_log_level = HYBRIS_LOG_WARN;
    } else if (strcmp(env, "error") == 0) {
        hybris_minimum_log_level = HYBRIS_LOG_ERROR;
    } else if (strcmp(env, "disabled") == 0) {
        hybris_minimum_log_level = HYBRIS_LOG_DISABLED;
    }

    env = getenv("HYBRIS_LOGGING_TARGET");
    if (env != NULL)
    {
        hybris_logging_target = fopen(env, "a");
    }
    if (hybris_logging_target == NULL)
        hybris_logging_target = stderr;

    env = getenv("HYBRIS_LOGGING_FORMAT");
    if (env != NULL)
    {
	if (strcmp(env, "systrace") == 0) {
		_hybris_logging_format = HYBRIS_LOG_FORMAT_SYSTRACE;
	}
	else
		_hybris_logging_format = HYBRIS_LOG_FORMAT_NORMAL;
    }

    env = getenv("HYBRIS_TRACE");
    if (env != NULL)
    {
        if (strcmp(env, "1") == 0) {
               _hybris_should_trace = 1;
        }
    }
    pthread_mutex_init(&hybris_logging_mutex, NULL);
}

int
hybris_should_log(enum hybris_log_level level)
{
    /* Initialize logging level from environment */
    if (!hybris_logging_initialized) {
        hybris_logging_initialized = 1;
        hybris_logging_initialize();
    }

    return (level >= hybris_minimum_log_level);
}

void
hybris_set_log_level(enum hybris_log_level level)
{
    hybris_minimum_log_level = level;
}

void *
hybris_get_thread_id()
{
    return (void *)pthread_self();
}

double
hybris_get_thread_time()
{
    struct timespec now;
    if(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &now) == 0) {
	return (double)now.tv_sec + (double)now.tv_nsec / 1000000000.0;
    } else {
	return -1.0;
    }    
}

int
hybris_should_trace(const char *module, const char *tracepoint)
{
    return _hybris_should_trace;
}

enum hybris_log_format hybris_logging_format()
{
    return _hybris_logging_format;
}
