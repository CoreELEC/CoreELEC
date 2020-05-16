/*
 * Copyright (c) 2012 Carsten Munk <carsten.munk@gmail.com>
 * Copyright (c) 2008 The Android Open Source Project
 * Copyright (c) 2013 Simon Busch <morphis@gravedo.de>
 * Copyright (c) 2013 Canonical Ltd
 * Copyright (c) 2013 Jolla Ltd. <robin.burchell@jollamobile.com>
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
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#include <hybris/properties/properties.h>
#include "properties_p.h"

struct hybris_prop_value
{
	char *key;
	char *value;
};

/* surely enough for anyone */
#define MAX_PROPS 1000

/* the sorted prop array */
static struct hybris_prop_value prop_array[MAX_PROPS];

/* the current highest index in prop_array */
static int max_prop;

/* helpers */
static void cache_update();
static int prop_qcmp(const void *a, const void *b);
static struct hybris_prop_value *cache_find_internal(const char *key);
static void cache_add_internal(const char *key, const char *value);
static void cache_repopulate_internal(FILE *f);
static void cache_empty_internal();
static void cache_repopulate_cmdline_internal();

/* the inode/mtime of the prop cache, used for invalidation */
static ino_t static_prop_inode;
static time_t static_prop_mtime;


/* public:
 * find a prop value from the file cache.
 *
 * the return value is the value of the given property key, or NULL if the
 * property key is not found. the returned value is owned by the caller,
 * and must be freed.
 */
char *hybris_propcache_find(const char *key)
{
	char *ret = NULL;

	cache_update();

	/* then look up the key and do a copy if we get a result */
	struct hybris_prop_value *prop = cache_find_internal(key);
	if (prop)
		return prop->value;

	return ret;
}

void hybris_propcache_list(hybris_propcache_list_cb cb, void *cookie)
{
	int n;
	struct hybris_prop_value *current;

	if (!cb)
		return;

	cache_update();

	for (n = 0; n < max_prop; n++) {
		current = &prop_array[n];
		cb(current->key, current->value, cookie);
	}
}

static void cache_update()
{
	struct stat st;
	FILE *f = fopen("/system/build.prop", "r");

	if (!f)
		return;

	/* before searching, we must first determine whether our cache is valid. if
	 * it isn't, we must discard our results and re-create the cache.
	 *
	 * we use fstat here to avoid a race between stat and something else
	 * touching the file.
	 */
	if (fstat(fileno(f), &st) != 0) {
		perror("cache_find can't stat build.prop");
		goto out;
	}

	/* TODO: is there any better way to detect changes? */
	if (static_prop_inode != st.st_ino ||
		static_prop_mtime != st.st_mtime) {
		static_prop_inode = st.st_ino;
		static_prop_mtime = st.st_mtime;

		/* cache is stale. fill it back up with fresh data first. */
		cache_empty_internal();
		cache_repopulate_internal(f);
		cache_repopulate_cmdline_internal();

		/* sort by keys */
		qsort(prop_array, max_prop, sizeof(struct hybris_prop_value), prop_qcmp);
	}

out:
	fclose(f);
}

/* private:
 * empties the prop cache, ready for repopulation
 */
static void cache_empty_internal()
{
	int i;
	for (i = 0; i < max_prop; ++i) {
		free(prop_array[i].key);
		free(prop_array[i].value);
	}

	max_prop = 0;
}

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

/* private:
 * find a given key in the in-memory prop cache.
 *
 * returns the value of the given property key, or NULL if the property is not
 * found. Note that this does not pass ownership of the hybris_prop_value or the
 * data inside it.
 */
static struct hybris_prop_value *cache_find_internal(const char *key)
{
	struct hybris_prop_value prop_key;
	prop_key.key = (char*)key;

	return bsearch(&prop_key, prop_array, max_prop, sizeof(struct hybris_prop_value), prop_qcmp);
}

/* private:
 * add a given property to the in-memory prop cache for later retrieval.
 *
 * both `key' and `value' are copied from the caller.
 */
static void cache_add_internal(const char *key, const char *value)
{
	/* Skip values that can be bigger than value max */
	if (strlen(value) >= PROP_VALUE_MAX -1)
		return;

	/* preserve current behavior of first prop key => match */
	if (cache_find_internal(key))
		return;

	prop_array[max_prop].key = strdup(key);
	prop_array[max_prop++].value = strdup(value);

	if (max_prop >= MAX_PROPS) {
		fprintf(stderr, "libhybris: ran out of props, increase MAX_PROPS");
		exit(1);
	}
}

/* private:
 * repopulates the prop cache from a given file `f'.
 */
static void cache_repopulate_internal(FILE *f)
{
	char buf[1024];
	char *mkey, *value;

	while (fgets(buf, 1024, f) != NULL) {
		if (strchr(buf, '\r'))
			*(strchr(buf, '\r')) = '\0';
		if (strchr(buf, '\n'))
			*(strchr(buf, '\n')) = '\0';

		mkey = strtok(buf, "=");

		if (!mkey)
			continue;

		value = strtok(NULL, "=");
		if (!value)
			continue;

		cache_add_internal(mkey, value);
	}
}

/* private:
 * repopulate the prop cache from /proc/cmdline
 */
static void cache_repopulate_cmdline_internal()
{
	/* Find a key value from the kernel command line, which is parsed
	 * by Android at init (on an Android working system) */
	char cmdline[1024];
	char *ptr;
	int fd;

	fd = open("/proc/cmdline", O_RDONLY);
	if (fd >= 0) {
		int n = read(fd, cmdline, 1023);
		if (n < 0) n = 0;

		/* get rid of trailing newline, it happens */
		if (n > 0 && cmdline[n-1] == '\n') n--;

		cmdline[n] = 0;
		close(fd);
	} else {
		cmdline[0] = 0;
	}

	ptr = cmdline;

	while (ptr && *ptr) {
		char *x = strchr(ptr, ' ');
		if (x != 0) *x++ = 0;

		char *name = ptr;
		ptr = x;

		char *value = strchr(name, '=');
		int name_len = strlen(name);

		if (value == 0) continue;
		*value++ = 0;
		if (name_len == 0) continue;

		if (!strncmp(name, "androidboot.", 12) && name_len > 12) {
			const char *boot_prop_name = name + 12;
			char prop[PROP_NAME_MAX];
			snprintf(prop, sizeof(prop) -1, "ro.%s", boot_prop_name);

			cache_add_internal(prop, value);
		}
	}
}
