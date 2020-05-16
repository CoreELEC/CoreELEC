/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 * Copyright (c) 2008 The Android Open Source Project
 * Copyright (c) 2013 Simon Busch <morphis@gravedo.de>
 * Copyright (c) 2013 Canonical Ltd
 * Copyright (c) 2013 Jolla Ltd. <robin.burchell@jollamobile.com>
 * Copyright (c) 2015 Jolla Ltd. <mikko.harju@jollamobile.com>
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

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <pthread.h>


#define HYBRIS_PROPERTY_CACHE_DEFAULT_TIMEOUT_SECS 10

/** Maximum allowed time to return stale data from the cache. Override
	with HYBRIS_PROPERTY_CACHE_TIMEOUT_SECS environment variable.
*/
static time_t runtime_cache_timeout_secs = HYBRIS_PROPERTY_CACHE_DEFAULT_TIMEOUT_SECS;


/** Key, value pair and the time of previous update (in seconds) */
struct hybris_prop_value
{
	char *key;
	char *value;
	time_t last_update;
};

static struct hybris_prop_value * prop_array = 0;
static int num_prop = 0;
static int num_alloc = 0;

/** Protect access to statics */
static pthread_mutex_t array_mutex = PTHREAD_MUTEX_INITIALIZER;

/* private:
 * compares two hybris_prop_value by key, so as to maintain a qsorted array of
 * props, and search the array.
 */
static int prop_qcmp(const void *a, const void *b)
{
	struct hybris_prop_value *aa = (struct hybris_prop_value *)a;
	struct hybris_prop_value *bb = (struct hybris_prop_value *)b;

	return strcmp(aa->key, bb->key);
}

static struct hybris_prop_value *cache_find_internal(const char *key)
{
	struct hybris_prop_value prop_key;
	prop_key.key = (char*)key;

	return bsearch(&prop_key, prop_array, num_prop, sizeof(struct hybris_prop_value), prop_qcmp);
}

static void runtime_cache_init()
{
	num_alloc = 8;
	prop_array = malloc(num_alloc * sizeof(struct hybris_prop_value));

	const char *timeout_str = getenv("HYBRIS_PROPERTY_CACHE_TIMEOUT_SECS");
	if (timeout_str) {
		runtime_cache_timeout_secs = atoi(timeout_str);
	}
}

static void runtime_cache_ensure_initialized()
{
	if (!prop_array) {
		runtime_cache_init();
	}
}

/** Invalidate an entry in the cache
  *
  * Cache will never shrink. Instead, assume that the same key
  * will be queried soon after invalidation and reuse the entry.
  */
static void runtime_cache_invalidate_entry(struct hybris_prop_value *entry)
{
	free(entry->value);
	entry->value = NULL;
}

static int runtime_cache_get_impl(const char *key, char *value)
{
	int ret = -ENOENT;

	struct hybris_prop_value *entry = cache_find_internal(key);
	if (entry != NULL && entry->value != NULL) {
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
		time_t delta_secs = now.tv_sec - entry->last_update;
		if (delta_secs > runtime_cache_timeout_secs) {
			// assume the data in cache is stale, and force refresh
			runtime_cache_invalidate_entry(entry);
		} else {
			// success, return value from cache
			strcpy(value, entry->value);
			ret = 0;
		}
	}

	return ret;
}

static void runtime_cache_insert_impl(const char *key, char *value)
{
	struct timespec now;
	clock_gettime(CLOCK_MONOTONIC_COARSE, &now);

	struct hybris_prop_value *entry = cache_find_internal(key);
	if (entry) {
		assert(entry->value == NULL);
		// key,value pair was invalidated earlier,
		// reuse entry in the property array
		entry->value = strdup(value);
		entry->last_update = now.tv_sec;
	} else {
		if (num_alloc == num_prop) {
			num_alloc = 3 * num_alloc / 2;
			prop_array = realloc(prop_array, num_alloc * sizeof(struct hybris_prop_value));
		}

		struct hybris_prop_value new_entry = { strdup(key), strdup(value), now.tv_sec };
		prop_array[num_prop++] = new_entry;

		qsort(prop_array, num_prop, sizeof(struct hybris_prop_value), prop_qcmp);
	}
}


void runtime_cache_lock()
{
	pthread_mutex_lock(&array_mutex);
}

void runtime_cache_unlock()
{
	pthread_mutex_unlock(&array_mutex);
}

void runtime_cache_remove(const char *key)
{
	runtime_cache_ensure_initialized();
	struct hybris_prop_value *entry = cache_find_internal(key);
	if (entry) {
		runtime_cache_invalidate_entry(entry);
	}
}

int runtime_cache_get(const char *key, char *value)
{
	runtime_cache_ensure_initialized();
	return runtime_cache_get_impl(key, value);
}

void runtime_cache_insert(const char *key, char *value)
{
	runtime_cache_ensure_initialized();
	runtime_cache_insert_impl(key, value);
}
